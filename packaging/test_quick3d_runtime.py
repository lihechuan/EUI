import importlib.util
import json
import tempfile
import unittest
from pathlib import Path


spec = importlib.util.spec_from_file_location('quick3d_runtime', Path(__file__).with_name('prepare-quick3d-runtime.py'))
runtime = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runtime)


class RuntimeManifestTests(unittest.TestCase):
    def test_bundle_permissions_links_and_identity(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            bundle = root / 'EUI Gallery.app'
            executable = bundle / 'Contents/MacOS/example'
            executable.parent.mkdir(parents=True)
            executable.write_bytes(b'gallery')
            executable.chmod(0o755)
            (bundle / 'Current').symlink_to('Contents')
            manifest = runtime.runtime_manifest(root, dict(galleryVersion='1.2.0', galleryRevision='revision'), 'x64', '6.8.3')
            self.assertEqual(manifest['galleryRevision'], 'revision')
            self.assertEqual(manifest['entry'], 'EUI Gallery.app/Contents/MacOS/example')
            files = {entry['path']: entry for entry in manifest['files']}
            self.assertTrue(files[manifest['entry']]['executable'])
            self.assertEqual(files['EUI Gallery.app/Current']['link'], 'Contents')
            self.assertEqual(files[manifest['entry']]['size'], 7)
            json.dumps(manifest)

    def test_rejects_external_and_absolute_links(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            bundle = root / 'EUI Gallery.app'
            bundle.mkdir()
            link = bundle / 'external'
            for destination in ('..', str(bundle)):
                with self.subTest(destination=destination):
                    link.symlink_to(destination)
                    with self.assertRaises(ValueError):
                        runtime.runtime_manifest(root, {}, 'x64', '6.8.3')
                    link.unlink()


if __name__ == '__main__':
    unittest.main()
