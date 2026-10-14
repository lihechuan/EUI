import argparse
import base64
import hashlib
import json
import os
import re
import subprocess
import tarfile
import tempfile
from pathlib import Path
from urllib.error import HTTPError
from urllib.request import Request, urlopen

from github_release import read_version, package_names, collect_packages


REPOSITORY = 'xiaojiaz/EUI'
API = 'https://gitee.com/api/v5/repos/' + REPOSITORY


def validate_inputs(commit, version):
    if not re.fullmatch(r'[0-9a-f]{40}', commit):
        raise ValueError('A full Gitee commit SHA is required.')
    if not re.fullmatch(r'\d+\.\d+\.\d+(?:\.\d+)?', version):
        raise ValueError('A numeric release version is required.')


def api(path, data=None, missing=False):
    if not os.environ.get('GITEE_TOKEN'):
        raise RuntimeError('Configure the GitHub repository secret GITEE_TOKEN before running this workflow.')
    headers = {'Authorization': 'Bearer ' + os.environ['GITEE_TOKEN'],
               'Accept': 'application/json', 'User-Agent': 'EUI-release'}
    if data is not None:
        headers['Content-Type'] = 'application/json'
    request = Request(API + path, headers=headers,
                      data=json.dumps(data).encode() if data is not None else None)
    try:
        with urlopen(request, timeout=60) as response:
            return json.load(response)
    except HTTPError as error:
        if missing and error.code == 404:
            return None
        raise RuntimeError(f'Gitee API failed with HTTP {error.code}.') from None


def transfer(url, output=None, upload=None):
    arguments = ['curl', '--silent', '--show-error', '--fail', '--location',
                 '--proto', '=https', '--proto-redir', '=https',
                 '--connect-timeout', '30', '--max-time', '900', '--config', '-']
    if output is not None:
        arguments += ['--output', str(output)]
    if upload is not None:
        arguments += ['--form', 'file=@' + str(upload)]
    token = os.environ['GITEE_TOKEN']
    if any(character in token for character in '\r\n"\\'):
        raise ValueError('Invalid Gitee token format.')
    result = subprocess.run(arguments + [url], capture_output=True, text=True, encoding='utf-8',
                            input='header = "Authorization: Bearer ' + token + '"\n')
    if result.returncode:
        raise RuntimeError(f'Gitee file transfer failed (curl exit {result.returncode}).')
    return json.loads(result.stdout) if upload is not None else None


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def release_packages(version, assets):
    names = package_names(version)
    if set(names + ['SHA256SUMS.txt']).issubset(assets):
        return names
    legacy = names[:2]
    if version == '1.1.0' and set(legacy + ['SHA256SUMS.txt']) == set(assets):
        return legacy
    return []


def attachments(release):
    values = api(f"/releases/{release['id']}/attach_files?per_page=100")
    result = {item['name']: item for item in values}
    if len(result) != len(values):
        raise ValueError('Duplicate Gitee release attachment names.')
    return result


def find_tag(version):
    page = 1
    while True:
        tags = api(f'/tags?per_page=100&page={page}')
        for tag in tags:
            if tag['name'] == 'v' + version:
                return tag
        if len(tags) < 100:
            return None
        page += 1


def checksums(manifest, version, names=None):
    result = {}
    for line in manifest.read_text(encoding='ascii').splitlines():
        checksum, name = line.split('  ', 1)
        if name in result or not re.fullmatch(r'[0-9a-fA-F]{64}', checksum):
            raise ValueError('Invalid or duplicate package checksum.')
        result[name] = checksum.lower()
    if set(result) != set(names if names is not None else package_names(version)):
        raise ValueError('Checksums must cover exactly the configured desktop packages.')
    return result


def download(release, asset, destination):
    transfer(API + f"/releases/{release['id']}/attach_files/{asset['id']}/download", destination)
    if destination.stat().st_size != asset['size']:
        raise ValueError('Downloaded attachment size mismatch: ' + destination.name)


def verify_release(release, version):
    assets = attachments(release)
    names = release_packages(version, assets)
    if not names:
        return False
    with tempfile.TemporaryDirectory(prefix='eui-gitee-verify-') as directory:
        root = Path(directory)
        download(release, assets['SHA256SUMS.txt'], root / 'SHA256SUMS.txt')
        expected = checksums(root / 'SHA256SUMS.txt', version, names)
        for name in names:
            download(release, assets[name], root / name)
            if digest(root / name) != expected[name]:
                raise ValueError('Downloaded Gitee checksum mismatch: ' + name)
    return True


def prepare(root, commit, version):
    validate_inputs(commit, version)
    release = api('/releases/tags/v' + version, missing=True)
    if release and not release['prerelease'] and verify_release(release, version):
        print('Gitee v' + version + ' is complete; packages are preserved.')
        return False, commit
    tag = find_tag(version)
    if tag:
        commit = tag['commit']['sha']
        validate_inputs(commit, version)
    destination = root / 'gitee-source'
    if destination.exists():
        raise ValueError('The Gitee source directory must not already exist.')
    with tempfile.TemporaryDirectory(prefix='eui-gitee-source-', dir=root) as directory:
        archive = Path(directory) / 'source.tar.gz'
        transfer(API + '/tarball?ref=' + commit, archive)
        with tarfile.open(archive) as source:
            source.extractall(Path(directory) / 'source', filter='data')
        entries = list((Path(directory) / 'source').iterdir())
        if len(entries) != 1 or not entries[0].is_dir():
            raise ValueError('Unexpected Gitee source archive layout.')
        entries[0].rename(destination)
    if read_version((destination / 'CMakeLists.txt').read_text(encoding='utf-8')) != version:
        raise ValueError('Gitee source version differs from the requested release.')
    print('Gitee build source: ' + commit)
    return True, commit


def publish(root, commit, version):
    validate_inputs(commit, version)
    destination = root / 'dist'
    expected = checksums(destination / 'SHA256SUMS.txt', version)
    files = [destination / name for name in package_names(version)]
    for path in files:
        if digest(path) != expected[path.name]:
            raise ValueError('Local package checksum mismatch: ' + path.name)
    files.append(destination / 'SHA256SUMS.txt')
    tag = find_tag(version)
    if tag and tag['commit']['sha'] != commit:
        raise ValueError('The Gitee tag points to a different source commit.')
    release = api('/releases/tags/v' + version, missing=True)
    if release and not release['prerelease'] and verify_release(release, version):
        print('Gitee v' + version + ' is complete; packages are preserved.')
        return
    if not release:
        release = api('/releases', {'tag_name': 'v' + version,
                      'name': 'EUI Gallery ' + version, 'target_commitish': commit,
                      'prerelease': True,
                      'body': (destination / 'release-notes.md').read_text(encoding='utf-8')})
    assets = attachments(release)
    with tempfile.TemporaryDirectory(prefix='eui-gitee-upload-') as directory:
        for path in files:
            asset = assets.get(path.name)
            if asset is None:
                asset = transfer(API + f"/releases/{release['id']}/attach_files", upload=path)
            downloaded = Path(directory) / path.name
            download(release, asset, downloaded)
            if digest(downloaded) != digest(path):
                raise ValueError('Downloaded Gitee checksum mismatch: ' + path.name)
            print('Gitee SHA-256 verified: ' + path.name)
    request = Request(API + f"/releases/{release['id']}", method='PATCH',
                      headers={'Authorization': 'Bearer ' + os.environ['GITEE_TOKEN'],
                               'Content-Type': 'application/json'},
                      data=json.dumps({'tag_name': 'v' + version, 'name': release['name'],
                                       'body': release['body'], 'prerelease': False}).encode())
    with urlopen(request, timeout=60) as response:
        if json.load(response)['prerelease']:
            raise ValueError('Gitee release is still a prerelease.')
    if not {path.name for path in files}.issubset(attachments(release)):
        raise ValueError('Gitee release is missing attachments.')
    print('https://gitee.com/' + REPOSITORY + '/releases/tag/v' + version)


def plan():
    commit = api('/commits/master')['sha']
    validate_inputs(commit, '0.0.0')
    source = api('/contents/CMakeLists.txt?ref=' + commit)
    version = read_version(base64.b64decode(source['content']).decode('utf-8'))
    validate_inputs(commit, version)
    release = api('/releases/tags/v' + version, missing=True)
    complete = bool(release and not release['prerelease']
                    and release_packages(version, attachments(release)))
    print(f'Gitee v{version}: ' + ('already published; skip build.' if complete else 'build required.'))
    return not complete, commit, version


def main():
    parser = argparse.ArgumentParser(description='Build and publish desktop packages from Gitee pushes.')
    parser.add_argument('command', choices=('plan', 'prepare', 'collect', 'publish'))
    arguments = parser.parse_args()
    root = Path.cwd()
    if arguments.command == 'plan':
        build, commit, version = plan()
        with Path(os.environ['GITHUB_OUTPUT']).open('a', encoding='utf-8') as output:
            output.write(f'build={str(build).lower()}\ncommit={commit}\nversion={version}\n')
    elif arguments.command == 'prepare':
        build, commit = prepare(root, os.environ['EUI_GITEE_COMMIT'], os.environ['EUI_RELEASE_VERSION'])
        with Path(os.environ['GITHUB_OUTPUT']).open('a', encoding='utf-8') as output:
            output.write(f'build={str(build).lower()}\ncommit={commit}\n')
    elif arguments.command == 'collect':
        collect_packages(root, os.environ['EUI_RELEASE_VERSION'])
    else:
        publish(root, os.environ['EUI_GITEE_COMMIT'], os.environ['EUI_RELEASE_VERSION'])


if __name__ == '__main__':
    main()
