import importlib.util
import json
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


spec = importlib.util.spec_from_file_location('quick3d_runtime', Path(__file__).with_name('prepare-quick3d-runtime.py'))
runtime = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runtime)


class RuntimeToolTests(unittest.TestCase):
    def test_gui_path_finds_qt_tools(self):
        with tempfile.TemporaryDirectory() as directory:
            home = Path(directory)
            for name, folder in [('ninja', 'qt/Tools/Ninja'), ('cmake', 'qt/Tools/CMake/CMake.app/Contents/bin')]:
                tool = home / folder / name
                tool.parent.mkdir(parents=True)
                tool.write_text('#!/bin/sh\nexit 0\n')
                tool.chmod(0o755)
                with patch.object(runtime.Path, 'home', return_value=home), patch.dict(os.environ, PATH='/usr/bin:/bin'):
                    self.assertEqual(runtime.build_tool(name), str(tool))
                    tool.chmod(0o644)
                    self.assertNotEqual(runtime.build_tool(name), str(tool))

    def test_path_takes_priority(self):
        with tempfile.TemporaryDirectory() as directory:
            tool = Path(directory) / 'cmake'
            tool.write_text('#!/bin/sh\nexit 0\n')
            tool.chmod(0o755)
            with patch.dict(os.environ, PATH=directory):
                self.assertEqual(runtime.build_tool('cmake'), str(tool))

    def test_missing_tool(self):
        self.assertIsNone(runtime.build_tool('eui-nonexistent-build-tool'))


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
