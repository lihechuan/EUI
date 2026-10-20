"""Run component regressions in the background and reject QML runtime errors."""

import argparse
import csv
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
RUNTIME_ERRORS = re.compile(
    r"TypeError:|ReferenceError:|SyntaxError:|RangeError:|Binding loop detected|Unable to assign|Cannot assign"
    r"|exception occurred during delayed|Uncaught exception|Injection of parameters into signal handlers|FAIL!"
)


def inventory():
    module = ROOT / "source/qt5/imports/eui"
    types = []
    for name, source in re.findall(r"^(E\w+) 1\.0 (.+)$", (module / "qmldir").read_text(encoding="utf-8"), re.M):
        content = (module / source).read_text(encoding="utf-8")
        models = re.findall(r"^    property var (model|data|items|steps|values|avatars|options|events|tasks|columns):", content, re.M)
        types.append({"name": name, "source": source, "models": models})
    types.extend({"name": name, "source": "native", "models": []}
                 for name in ("EAdvancedRectangle", "EComponentStateObject"))
    return types


def run(args):
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    types = inventory()
    (output / "PublicTypes.js").write_text(
        ".pragma library\nvar types = " + json.dumps([item["name"] for item in types])
        + "\nvar models = " + json.dumps({item["name"]: item["models"] for item in types if item["models"]}) + "\n",
        encoding="utf-8")
    environment = dict(os.environ, PATH=str(args.qt_bin.resolve()) + os.pathsep + os.environ["PATH"],
        QT_QPA_PLATFORM="offscreen", QT_QUICK_BACKEND="software", QT_QUICK_CONTROLS_STYLE="Basic",
        QT_QPA_FONTDIR=str(Path(os.environ.get("WINDIR", "C:/Windows")) / "Fonts"), QT_FORCE_STDERR_LOGGING="1")
    if args.qt5_interpreter:
        environment["QV4_FORCE_INTERPRETER"] = "1"
    suites = [HERE / "tst_public_components.qml", HERE / "tst_slot_regressions.qml",
        HERE / "tst_form_regressions.qml", HERE / "tst_window_regressions.qml",
        HERE.parent / "primevue/tst_gallery_interactions.qml"]
    if args.gallery_runner:
        suites.extend([HERE.parent / "primevue/tst_primevue_components.qml",
            HERE / "tst_desktop_gallery.qml", HERE / "tst_gallery_navigation.qml", HERE / "tst_gallery_matrix.qml"])
        (ROOT / "build/primevue-completion").mkdir(parents=True, exist_ok=True)
        (ROOT / "build/library-validation/gallery-images").mkdir(parents=True, exist_ok=True)
    else:
        print("Gallery resource and page suites require --gallery-runner; running the standalone suites", flush=True)
    results = []
    for suite in suites:
        source = suite
        if suite.name == "tst_public_components.qml":
            source = output / suite.name
            source.write_text(suite.read_text(encoding="utf-8"), encoding="utf-8")
        log = output / (suite.stem + ".txt")
        log.unlink(missing_ok=True)
        runner = args.gallery_runner if suite.name in ("tst_primevue_components.qml",
            "tst_desktop_gallery.qml", "tst_gallery_navigation.qml", "tst_gallery_matrix.qml") else args.runner
        isolated = os.name == "nt" and suite.name in ("tst_gallery_navigation.qml", "tst_gallery_matrix.qml")
        if isolated:
            command = [sys.executable, str(HERE / "run_isolated_qml.py"), str(runner.resolve()),
                str(source), str(args.imports.resolve())]
        else:
            command = [str(runner.resolve()), "-input", str(source), "-import", str(args.imports.resolve()),
                "-platform", "offscreen", "-o", str(log) + ",txt"]
        environment["EUI_QML_TIMEOUT"] = "890"
        try:
            completed = subprocess.run(command, cwd=ROOT, env=environment, capture_output=True, timeout=900,
                creationflags=subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0)
            console = (completed.stdout + completed.stderr).decode("utf-8", errors="replace")
            if isolated:
                log.write_text(console, encoding="utf-8")
            text = log.read_text(encoding="utf-8", errors="replace") if log.exists() else ""
            (output / (suite.stem + "-console.txt")).write_text(console, encoding="utf-8")
            totals = re.search(r"Totals: (\d+) passed, (\d+) failed, (\d+) skipped", text)
            errors = [line for line in (text if isolated else text + console).splitlines() if RUNTIME_ERRORS.search(line)]
            result = {"suite": suite.name, "exit_code": completed.returncode,
                "totals": list(map(int, totals.groups())) if totals else None, "errors": errors,
                "passed": completed.returncode == 0 and totals is not None and not errors}
        except subprocess.TimeoutExpired:
            result = {"suite": suite.name, "passed": False, "errors": ["Timeout after 900 seconds"]}
        results.append(result)
        print(json.dumps(result, ensure_ascii=False), flush=True)
    (output / "results.json").write_text(json.dumps(results, ensure_ascii=False, indent=2), encoding="utf-8")
    public_log = (output / "tst_public_components.txt").read_text(encoding="utf-8", errors="replace")
    with (output / "component-coverage.csv").open("w", encoding="utf-8-sig", newline="") as stream:
        writer = csv.writer(stream)
        writer.writerow(["component", "source", "passed_states", "expected_states", "failed_states"])
        for item in types:
            prefix = "::test_components(" + item["name"] + "-"
            lines = [line for line in public_log.splitlines() if prefix in line]
            writer.writerow([item["name"], item["source"], sum(line.startswith("PASS") for line in lines), 4,
                sum(line.startswith("FAIL") for line in lines)])
        animation = [line for line in public_log.splitlines() if "::test_animation(" in line]
        writer.writerow(["EAnimation", "native singleton", sum(line.startswith("PASS") for line in animation), 83,
            sum(line.startswith("FAIL") for line in animation)])
    return 0 if all(result["passed"] for result in results) else 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runner", type=Path, required=True)
    parser.add_argument("--imports", type=Path, required=True, help="Parent of the built EUI module")
    parser.add_argument("--gallery-runner", type=Path, help="eui_gallery_tests executable required for Gallery resources and pages")
    parser.add_argument("--qt-bin", type=Path, required=True)
    parser.add_argument("--output", type=Path, default=ROOT / "build/library-validation/public")
    parser.add_argument("--qt5-interpreter", action="store_true", help="Qt 5.15.2 Win64 QTBUG-89513 workaround")
    sys.exit(run(parser.parse_args()))
