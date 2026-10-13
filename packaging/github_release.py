import argparse
import hashlib
import json
import os
import re
import subprocess
import tempfile
from pathlib import Path


def read_version(source):
    match = re.search(r'project\(EUI VERSION (\d+\.\d+(?:\.\d+){0,2})\)', source)
    if not match:
        raise ValueError('Cannot read a numeric EUI version from CMakeLists.txt.')
    value = match.group(1)
    return value + '.0' if value.count('.') == 1 else value


def release_plan(root, event_name, ref, event):
    version = read_version((root / 'CMakeLists.txt').read_text(encoding='utf-8'))
    publish = False
    if event_name == 'push' and ref == 'refs/heads/master':
        before = event['before']
        if not re.fullmatch(r'[0-9a-f]{40}', before):
            raise ValueError('Invalid previous push commit.')
        if before == '0' * 40:
            publish = True
        else:
            previous = subprocess.check_output(
                ['git', 'show', before + ':CMakeLists.txt'], cwd=root, encoding='utf-8'
            )
            publish = version != read_version(previous)
    elif event_name == 'push' and ref.startswith('refs/tags/v'):
        if ref != 'refs/tags/v' + version:
            raise ValueError('The tag must match the version in CMakeLists.txt.')
        publish = True
    return version, publish


def run(*arguments):
    return subprocess.check_output(arguments, encoding='utf-8').strip()


def api(path, data=None, paginate=False):
    arguments = ['gh', 'api', path]
    if paginate:
        arguments += ['--paginate', '--slurp']
    if data is not None:
        arguments += ['--method', 'PATCH', '--input', '-']
    return json.loads(subprocess.check_output(
        arguments, input=json.dumps(data) if data is not None else None, encoding='utf-8'
    ))


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def publish_release(root, version, repository, commit):
    if version != read_version((root / 'CMakeLists.txt').read_text(encoding='utf-8')):
        raise ValueError('Release version differs from the checked-out source.')
    tag = 'v' + version
    prefix = 'repos/' + repository
    releases = api(prefix + '/releases?per_page=100', paginate=True)
    release = next((item for page in releases for item in page if item['tag_name'] == tag), None)
    if release and not release['draft']:
        print(f'{tag} is already published; its files are preserved.')
        return

    reference = subprocess.run(
        ['git', 'rev-parse', '--verify', f'refs/tags/{tag}^{{commit}}'],
        cwd=root, capture_output=True, text=True
    )
    if reference.returncode == 0 and reference.stdout.strip() != commit:
        raise ValueError('The existing tag points to a different source commit.')
    if release and release['target_commitish'] != commit:
        raise ValueError('The existing draft belongs to a different source commit.')

    files = [root / 'dist' / f'EUI-Gallery-{version}-windows-x64-{suffix}'
             for suffix in ('setup.exe', 'portable.zip')]
    checksums = root / 'dist/SHA256SUMS.txt'
    expected = {}
    for line in checksums.read_text(encoding='ascii').splitlines():
        checksum, name = line.split('  ', 1)
        if name in expected or not re.fullmatch(r'[0-9a-fA-F]{64}', checksum):
            raise ValueError('Invalid or duplicate package checksum.')
        expected[name] = checksum.lower()
    if set(expected) != {path.name for path in files}:
        raise ValueError('Checksums must cover exactly the installer and portable ZIP.')
    for path in files:
        if digest(path) != expected[path.name]:
            raise ValueError(f'Local package checksum mismatch: {path.name}')
    files.append(checksums)

    if not release:
        run('gh', 'release', 'create', tag, '--repo', repository, '--draft',
            '--target', commit, '--title', f'EUI Gallery {version}',
            '--notes-file', str(root / 'packaging/release-notes.md'))
        releases = api(prefix + '/releases?per_page=100', paginate=True)
        release = next(item for page in releases for item in page if item['tag_name'] == tag)

    assets = {asset['name']: asset for asset in release['assets']}
    with tempfile.TemporaryDirectory(prefix='eui-release-verify-') as directory:
        for path in files:
            if path.name not in assets:
                run('gh', 'release', 'upload', tag, str(path), '--repo', repository)
            run('gh', 'release', 'download', tag, '--repo', repository,
                '--pattern', path.name, '--dir', directory)
            if digest(Path(directory) / path.name) != digest(path):
                raise ValueError(f'Downloaded release checksum mismatch: {path.name}')
    published = api(prefix + f"/releases/{release['id']}", {'draft': False, 'make_latest': 'legacy'})
    if published['draft'] or not {path.name for path in files}.issubset(
        {asset['name'] for asset in published['assets']}
    ):
        raise ValueError('Release publication or asset verification failed.')
    print(published['html_url'])


def main():
    parser = argparse.ArgumentParser(description='Detect and publish EUI version releases.')
    parser.add_argument('command', choices=('plan', 'publish'))
    arguments = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    if arguments.command == 'plan':
        event = json.loads(Path(os.environ['GITHUB_EVENT_PATH']).read_text(encoding='utf-8'))
        version, publish = release_plan(root, os.environ['GITHUB_EVENT_NAME'], os.environ['GITHUB_REF'], event)
        with Path(os.environ['GITHUB_OUTPUT']).open('a', encoding='utf-8') as output:
            output.write(f'version={version}\npublish={str(publish).lower()}\n')
        print(f'Version: {version}; publish release: {publish}')
    else:
        publish_release(root, os.environ['EUI_RELEASE_VERSION'], os.environ['GITHUB_REPOSITORY'],
                        os.environ['GITHUB_SHA'])


if __name__ == '__main__':
    main()
