import hashlib
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import github_release as release


class ReleaseAutomationTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory(prefix='eui-release-test-')
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.source = self.root / 'CMakeLists.txt'
        self.source.write_text('project(EUI VERSION 1.0.0)\n', encoding='utf-8')
        subprocess.run(['git', 'init', '--quiet', str(self.root)], check=True)
        self.before = self.commit()
        self.source.write_text('project(EUI VERSION 1.0.1)\n', encoding='utf-8')
        self.commit()
        (self.root / 'unrelated.txt').write_text('another commit in the same push', encoding='utf-8')
        self.current = self.commit()

    def commit(self):
        subprocess.run(['git', 'add', '.'], cwd=self.root, check=True)
        subprocess.run(['git', '-c', 'user.name=Release test', '-c', 'user.email=test@example.invalid',
                        'commit', '--quiet', '-m', 'test fixture'], cwd=self.root, check=True)
        return subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=self.root, text=True).strip()

    def plan(self, event_name='push', ref='refs/heads/master', before=None):
        return release.release_plan(self.root, event_name, ref, {'before': before or self.before})

    def packages(self):
        destination = self.root / 'dist'
        destination.mkdir()
        self.files = {}
        for name in release.package_names('1.0.1'):
            self.files[name] = ('test package ' + name).encode()
            (destination / name).write_bytes(self.files[name])
        checksum = ''.join(hashlib.sha256(data).hexdigest() + '  ' + name + '\n'
                           for name, data in self.files.items())
        self.files['SHA256SUMS.txt'] = checksum.encode('ascii')
        (destination / 'SHA256SUMS.txt').write_bytes(self.files['SHA256SUMS.txt'])
        self.remote = {}
        self.published = False

    def api(self, path, data=None, paginate=False):
        item = {'tag_name': 'v1.0.1', 'draft': not self.published, 'target_commitish': self.current,
                'id': 1, 'assets': [{'name': name} for name in self.remote], 'html_url': 'test release'}
        if paginate:
            return [[item]] if self.created else [[]]
        self.assertEqual(set(self.remote), set(self.files))
        self.assertEqual(data, {'draft': False, 'make_latest': 'legacy'})
        self.published = True
        item['draft'] = False
        return item

    def command(self, *arguments):
        if arguments[:3] == ('gh', 'release', 'create'):
            self.assertIn('--draft', arguments)
            self.created = True
        elif arguments[:3] == ('gh', 'release', 'upload'):
            self.assertNotIn('--clobber', arguments)
            path = Path(arguments[4])
            self.remote[path.name] = path.read_bytes()
        elif arguments[:3] == ('gh', 'release', 'download'):
            name = arguments[arguments.index('--pattern') + 1]
            directory = Path(arguments[arguments.index('--dir') + 1])
            (directory / name).write_bytes(self.remote[name])
        else:
            self.fail(arguments)
        return ''

    def publish(self):
        with patch.object(release, 'api', side_effect=self.api), patch.object(release, 'run', side_effect=self.command):
            release.publish_release(self.root, '1.0.1', 'owner/project', self.current)

    def test_version_change_across_multiple_commits(self):
        self.assertEqual(self.plan(), ('1.0.1', True))
        self.assertEqual(self.plan(before=self.current), ('1.0.1', False))
        self.assertEqual(self.plan(before='0' * 40), ('1.0.1', True))

    def test_equivalent_versions_and_non_release_events(self):
        self.source.write_text('project(EUI VERSION 1.0)\n', encoding='utf-8')
        self.assertEqual(self.plan(), ('1.0.0', False))
        for event, ref in (('pull_request', 'refs/pull/1/merge'), ('workflow_dispatch', 'refs/heads/master'),
                           ('push', 'refs/heads/feature')):
            self.assertFalse(self.plan(event, ref)[1])

    def test_invalid_versions_and_mismatched_tags(self):
        self.assertEqual(self.plan(ref='refs/tags/v1.0.1'), ('1.0.1', True))
        with self.assertRaises(ValueError):
            self.plan(ref='refs/tags/v9.0.0')
        with self.assertRaises(ValueError):
            self.plan(before='not a commit')
        for source in ('project(Other VERSION 1.0.1)', 'project(EUI VERSION 1..0)',
                       'project(EUI VERSION 1.0.1-beta)', 'project(EUI VERSION 1.0.1.0.0)'):
            with self.assertRaises(ValueError):
                release.read_version(source)

    def test_publish_only_after_all_downloads_match(self):
        self.packages()
        self.created = False
        self.publish()
        self.assertTrue(self.published)
        self.assertEqual(self.remote, self.files)

    def test_resume_partial_draft_without_replacing_assets(self):
        self.packages()
        self.created = True
        name = next(iter(self.files))
        self.remote[name] = self.files[name]
        self.publish()
        self.assertTrue(self.published)

    def test_checksum_mismatch_keeps_draft_unpublished(self):
        self.packages()
        self.created = True
        name = next(iter(self.files))
        self.remote[name] = b'wrong package'
        with self.assertRaisesRegex(ValueError, 'Downloaded release checksum mismatch'):
            self.publish()
        self.assertFalse(self.published)
        self.assertEqual(self.remote[name], b'wrong package')

    def test_existing_release_is_untouched(self):
        item = {'tag_name': 'v1.0.1', 'draft': False}
        with patch.object(release, 'api', return_value=[[item]]), patch.object(release, 'run') as command:
            release.publish_release(self.root, '1.0.1', 'owner/project', self.current)
            command.assert_not_called()

    def test_conflicting_tag_and_draft_are_rejected(self):
        subprocess.run(['git', 'tag', 'v1.0.1', self.before], cwd=self.root, check=True)
        with patch.object(release, 'api', return_value=[[]]), patch.object(release, 'run') as command:
            with self.assertRaisesRegex(ValueError, 'existing tag'):
                release.publish_release(self.root, '1.0.1', 'owner/project', self.current)
            command.assert_not_called()
        subprocess.run(['git', 'tag', '-d', 'v1.0.1'], cwd=self.root, check=True, capture_output=True)
        item = {'tag_name': 'v1.0.1', 'draft': True, 'target_commitish': self.before}
        with patch.object(release, 'api', return_value=[[item]]), patch.object(release, 'run') as command:
            with self.assertRaisesRegex(ValueError, 'existing draft'):
                release.publish_release(self.root, '1.0.1', 'owner/project', self.current)
            command.assert_not_called()

    def test_local_checksum_mismatch_prevents_upload(self):
        self.packages()
        name = next(iter(self.files))
        (self.root / 'dist' / name).write_bytes(b'wrong local package')
        with patch.object(release, 'api', return_value=[[]]), patch.object(release, 'run') as command:
            with self.assertRaisesRegex(ValueError, 'Local package checksum mismatch'):
                release.publish_release(self.root, '1.0.1', 'owner/project', self.current)
            command.assert_not_called()

    def test_collect_requires_all_platforms_and_valid_checksums(self):
        self.packages()
        destination = self.root / 'dist'
        for index, name in enumerate(release.package_names('1.0.1')):
            directory = destination / str(index // 2)
            directory.mkdir(exist_ok=True)
            (destination / name).rename(directory / name)
            with (directory / 'SHA256SUMS.txt').open('a', encoding='ascii') as output:
                output.write(hashlib.sha256(self.files[name]).hexdigest() + '  ' + name + '\n')
        release.collect_packages(self.root, '1.0.1')
        self.assertEqual(set((destination / 'SHA256SUMS.txt').read_text().splitlines()),
                         set(self.files['SHA256SUMS.txt'].decode().splitlines()))
        manifest = destination / str(len(release.package_names('1.0.1')) // 2 - 1) / 'SHA256SUMS.txt'
        content = manifest.read_text()
        manifest.write_text(content.splitlines()[0] + '\n')
        with self.assertRaisesRegex(ValueError, 'Missing packages'):
            release.collect_packages(self.root, '1.0.1')
        manifest.write_text(content.replace(content[:64], '0' * 64, 1))
        with self.assertRaisesRegex(ValueError, 'checksum mismatch'):
            release.collect_packages(self.root, '1.0.1')

    def test_packages_cover_only_windows_and_macos_architectures(self):
        suffixes = {
            'windows-x86-setup.exe', 'windows-x86-portable.zip',
            'windows-x64-setup.exe', 'windows-x64-portable.zip',
            'macos-x64.dmg', 'macos-x64-portable.zip',
            'macos-arm64.dmg', 'macos-arm64-portable.zip',
        }
        names = release.package_names('1.0.1')
        self.assertEqual(len(names), 8)
        self.assertEqual(set(names), {'EUI-Gallery-1.0.1-' + suffix for suffix in suffixes})


if __name__ == '__main__':
    unittest.main()
