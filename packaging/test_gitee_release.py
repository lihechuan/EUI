import base64
import hashlib
import json
import os
import tarfile
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import gitee_release as release


class GiteeReleaseTests(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory(prefix='eui-gitee-test-')
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        self.commit = 'a' * 40
        self.version = '1.1.0'
        destination = self.root / 'dist'
        destination.mkdir()
        self.files = {name: name.encode() for name in release.package_names(self.version)}
        manifest = ''.join(hashlib.sha256(data).hexdigest() + '  ' + name + '\n'
                           for name, data in self.files.items())
        self.files['SHA256SUMS.txt'] = manifest.encode()
        for name, data in self.files.items():
            (destination / name).write_bytes(data)
        (destination / 'release-notes.md').write_text('1. Animation presets.\n')
        self.remote = {}
        self.item = None
        self.promoted = False
        self.uploads = []

    def assets(self):
        return [{'id': index, 'name': name, 'size': len(data)}
                for index, (name, data) in enumerate(self.remote.items(), 1)]

    def api(self, path, data=None, missing=False):
        if path.startswith('/tags?'):
            return []
        if path.startswith('/releases/tags/'):
            return self.item
        if path.endswith('/attach_files?per_page=100'):
            return self.assets()
        self.assertEqual(path, '/releases')
        self.assertTrue(data['prerelease'])
        self.assertEqual(data['target_commitish'], self.commit)
        self.item = dict(data, id=7)
        return self.item

    def transfer(self, url, output=None, upload=None):
        if upload is not None:
            self.assertNotIn(upload.name, self.remote)
            self.uploads.append(upload.name)
            self.remote[upload.name] = upload.read_bytes()
            return self.assets()[-1]
        identifier = int(url.split('/')[-2])
        asset = next(asset for asset in self.assets() if asset['id'] == identifier)
        output.write_bytes(self.remote[asset['name']])

    def promote(self, request, timeout):
        self.assertEqual(request.method, 'PATCH')
        self.assertEqual(json.loads(request.data), {'prerelease': False})
        self.assertEqual(self.remote, self.files)
        self.promoted = True
        response = unittest.mock.MagicMock()
        response.__enter__.return_value.read.return_value = b'{"prerelease": false}'
        return response

    def publish(self):
        with patch.object(release, 'api', side_effect=self.api), \
                patch.object(release, 'transfer', side_effect=self.transfer), \
                patch.object(release, 'urlopen', side_effect=self.promote), \
                patch.dict(os.environ, {'GITEE_TOKEN': 'test-token'}):
            release.publish(self.root, self.commit, self.version)

    def test_publish_and_resume_without_replacing_assets(self):
        self.publish()
        self.assertTrue(self.promoted)
        self.assertEqual(self.remote, self.files)
        self.item['prerelease'] = True
        self.uploads.clear()
        self.promoted = False
        self.publish()
        self.assertTrue(self.promoted)
        self.assertEqual(self.uploads, [])

    def test_partial_release_is_completed_and_verified(self):
        self.item = {'id': 7, 'prerelease': True}
        name = next(iter(self.files))
        self.remote[name] = self.files[name]
        self.publish()
        self.assertNotIn(name, self.uploads)
        self.assertTrue(self.promoted)

    def test_remote_mismatch_prevents_promotion_and_overwrite(self):
        self.item = {'id': 7, 'prerelease': True}
        name = next(iter(self.files))
        self.remote[name] = b'wrong bytes'
        with self.assertRaisesRegex(ValueError, 'checksum mismatch'):
            self.publish()
        self.assertFalse(self.promoted)
        self.assertEqual(self.remote[name], b'wrong bytes')
        self.assertEqual(self.uploads, [])

    def test_local_corruption_prevents_any_remote_action(self):
        (self.root / 'dist' / next(iter(self.files))).write_bytes(b'corrupt')
        with patch.object(release, 'api') as api:
            with self.assertRaisesRegex(ValueError, 'Local package checksum mismatch'):
                release.publish(self.root, self.commit, self.version)
            api.assert_not_called()

    def test_complete_version_skips_build_and_keeps_packages(self):
        self.item = {'id': 7, 'prerelease': False}
        self.remote = self.files.copy()
        with patch.object(release, 'api', side_effect=self.api), \
                patch.object(release, 'transfer', side_effect=self.transfer):
            self.assertEqual(release.prepare(self.root, self.commit, self.version), (False, self.commit))
        self.assertFalse((self.root / 'gitee-source').exists())

    def test_prepare_pins_existing_tag_and_checks_archive_version(self):
        source = self.root / 'archive-source'
        source.mkdir()
        (source / 'CMakeLists.txt').write_text('project(EUI VERSION 1.1.0)\n')
        archive = self.root / 'archive.tar.gz'
        with tarfile.open(archive, 'w:gz') as output:
            output.add(source, arcname='EUI')
        pinned = 'b' * 40
        def download(url, output):
            self.assertIn('ref=' + pinned, url)
            output.write_bytes(archive.read_bytes())
        with patch.object(release, 'api', return_value=None), \
                patch.object(release, 'find_tag', return_value={'commit': {'sha': pinned}}), \
                patch.object(release, 'transfer', side_effect=download):
            self.assertEqual(release.prepare(self.root, self.commit, self.version), (True, pinned))

    def test_invalid_inputs_and_manifest(self):
        for commit, version in [('master', self.version), (self.commit, '1.1.0;echo bad'),
                                ('a' * 39, self.version), (self.commit, '1.1')]:
            with self.assertRaises(ValueError):
                release.validate_inputs(commit, version)
        manifest = self.root / 'dist/SHA256SUMS.txt'
        original = manifest.read_text()
        for value in [original + original, original.replace('  EUI-', '  ../EUI-'),
                      'wrong manifest', original.replace(original[:64], 'z' * 64)]:
            manifest.write_text(value)
            with self.assertRaises(ValueError):
                release.checksums(manifest, self.version)

    def test_poll_pins_master_version_and_skips_complete_release(self):
        def api(path, data=None, missing=False):
            if path == '/commits/master':
                return {'sha': self.commit}
            if path.startswith('/contents/'):
                self.assertEqual(path, '/contents/CMakeLists.txt?ref=' + self.commit)
                return {'content': base64.b64encode(b'project(EUI VERSION 1.1.0)\n').decode()}
            return self.api(path, data, missing)
        with patch.object(release, 'api', side_effect=api), patch.object(release, 'transfer') as transfer:
            self.assertEqual(release.plan(), (True, self.commit, self.version))
            self.item = {'id': 7, 'prerelease': True}
            self.remote = self.files.copy()
            self.assertTrue(release.plan()[0])
            self.item['prerelease'] = False
            self.assertEqual(release.plan(), (False, self.commit, self.version))
            self.remote.pop('SHA256SUMS.txt')
            self.assertTrue(release.plan()[0])
            transfer.assert_not_called()

    def test_poll_rejects_invalid_source(self):
        with patch.object(release, 'api', return_value={'sha': 'master'}):
            with self.assertRaises(ValueError):
                release.plan()
        responses = [{'sha': self.commit}, {'content': base64.b64encode(b'bad cmake').decode()}]
        with patch.object(release, 'api', side_effect=responses):
            with self.assertRaises(ValueError):
                release.plan()

    def test_missing_token_fails_before_network_request(self):
        with patch.dict(os.environ, {'GITEE_TOKEN': ''}), patch.object(release, 'urlopen') as call:
            with self.assertRaisesRegex(RuntimeError, 'GITEE_TOKEN'):
                release.api('/commits/master')
            call.assert_not_called()


if __name__ == '__main__':
    unittest.main()
