import argparse
import json
import os
import subprocess
import sys
import tempfile
import threading
import unittest
from pathlib import Path
from unittest.mock import patch

import run as library
import run_full


class RunnerTests(unittest.TestCase):
    def test_gallery_resources_and_native_matrix_use_the_matching_runner(self):
        commands = []

        def complete(command, **options):
            commands.append(command)
            summary = "Totals: 1 passed, 0 failed, 0 skipped\n"
            if "-o" in command:
                Path(command[command.index("-o") + 1][:-4]).write_text(summary, encoding="utf-8")
            return subprocess.CompletedProcess(command, 0, summary.encode(), b"")

        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            arguments = argparse.Namespace(output=root / "results", qt_bin=root,
                runner=root / "qmltestrunner.exe", gallery_runner=root / "eui_gallery_tests.exe",
                imports=root, qt5_interpreter=False)
            with patch.object(library, "ROOT", root), patch.object(library, "inventory", return_value=[]), \
                    patch.object(library.subprocess, "run", side_effect=complete):
                self.assertEqual(library.run(arguments), 0)
            primevue = next(command for command in commands if any(
                str(value).endswith("tst_primevue_components.qml") for value in command))
            self.assertEqual(primevue[0], str(arguments.gallery_runner))
            for suite in ("tst_gallery_navigation.qml", "tst_gallery_matrix.qml"):
                matrix = next(command for command in commands if any(
                    str(value).endswith(suite) for value in command))
                if os.name == "nt":
                    self.assertEqual(Path(matrix[1]).name, "run_isolated_qml.py")
                    self.assertEqual(matrix[2], str(arguments.gallery_runner.resolve()))
                    self.assertNotIn("offscreen", matrix)
                else:
                    self.assertEqual(matrix[0], str(arguments.gallery_runner))
            self.assertTrue((root / "build/primevue-completion").is_dir())
            self.assertTrue((root / "build/library-validation/gallery-images").is_dir())

    def test_ctest_failure_patterns_working_directory_and_timeout_are_preserved(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)
            arguments = argparse.Namespace(output=output)
            test = {"name": "failure-pattern", "command": [sys.executable, "helper.py"], "properties": [
                {"name": "FAIL_REGULAR_EXPRESSION", "value": ["CMake failure"]},
                {"name": "WORKING_DIRECTORY", "value": directory}, {"name": "TIMEOUT", "value": 20}]}
            completed = subprocess.CompletedProcess(test["command"], 0, b"CMake failure\n", b"")
            with patch.object(run_full.subprocess, "run", return_value=completed) as invoke:
                result = run_full.execute(test, arguments, {})
            self.assertFalse(result["passed"])
            self.assertEqual(result["errors"], ["CMake failure"])
            self.assertEqual(invoke.call_args.kwargs["cwd"], directory)
            self.assertEqual(invoke.call_args.kwargs["timeout"], 20)
            self.assertEqual(invoke.call_args.kwargs["env"]["EUI_QML_TIMEOUT"], "15")

    def test_native_qml_keeps_test_filters_on_the_isolated_desktop(self):
        with tempfile.TemporaryDirectory() as directory:
            arguments = argparse.Namespace(output=Path(directory))
            test = {"name": "native", "command": ["runner.exe", "-input", "suite.qml", "-import", "imports",
                "-platform", "windows", "Suite::test_hover"]}
            completed = subprocess.CompletedProcess([], 0, b"Totals: 1 passed, 0 failed, 0 skipped\n", b"")
            with patch.object(run_full.subprocess, "run", return_value=completed) as invoke:
                result = run_full.execute(test, arguments, {})
            self.assertTrue(result["passed"])
            command = invoke.call_args.args[0]
            self.assertEqual(Path(command[1]), Path(run_full.__file__).with_name("run_isolated_qml.py"))
            self.assertEqual(command[-1], "Suite::test_hover")
            self.assertNotIn("offscreen", command)

    def test_run_serial_waits_for_parallel_tests(self):
        active = set()
        lock = threading.Lock()
        first_batch = threading.Barrier(2)
        serial_completed = threading.Event()

        def execute(test, *unused):
            name = test["name"]
            with lock:
                if name == "serial":
                    self.assertFalse(active)
                    serial_completed.set()
                elif name == "after":
                    self.assertTrue(serial_completed.is_set())
                active.add(name)
            if name.startswith("parallel"):
                first_batch.wait(timeout=5)
            with lock:
                active.remove(name)
            return {"name": name, "passed": True}

        tests = [{"name": "parallel-1"}, {"name": "parallel-2"}, {"name": "serial", "properties": [
            {"name": "RUN_SERIAL", "value": True}]}, {"name": "after"}]
        with tempfile.TemporaryDirectory() as directory:
            arguments = argparse.Namespace(output=Path(directory), build=Path(directory), qt_bin=Path(directory),
                only=".*", jobs=2, qt5_interpreter=False)
            with patch.object(run_full.subprocess, "check_output", return_value=json.dumps({"tests": tests})), \
                    patch.object(run_full, "execute", side_effect=execute):
                self.assertEqual(run_full.run(arguments), 0)
            results = json.loads((arguments.output / "summary.json").read_text(encoding="utf-8"))
            self.assertEqual([result["name"] for result in results], [test["name"] for test in tests])


if __name__ == "__main__":
    unittest.main()
