"""Run an existing CTest inventory without showing windows on the user's desktop."""

import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path

from run import RUNTIME_ERRORS

ROOT = Path(__file__).resolve().parents[2]


def execute(test, args, environment):
    name = test["name"]
    command = list(test["command"])
    properties = {item["name"]: item["value"] for item in test.get("properties", [])}
    env = dict(environment)
    for entry in properties.get("ENVIRONMENT", []):
        key, _, value = entry.partition("=")
        if key not in ("QT_QUICK_CONTROLS_STYLE", "QT_QPA_PLATFORM"):
            env[key] = value
    log = args.output / (name + ".txt")
    log.unlink(missing_ok=True)
    qml = "-input" in command
    if qml and "-platform" in command and command[command.index("-platform") + 1] == "windows":
        isolated = ROOT / "tests/run_isolated_qml.py"
        if not isolated.is_file():
            return {"name": name, "passed": False, "errors": ["Isolated desktop runner is unavailable"]}
        command = [sys.executable, str(isolated), command[0], command[command.index("-input") + 1],
            command[command.index("-import") + 1]]
    elif qml or Path(command[0]).suffix == ".exe" and Path(command[0]).name.startswith("eui_"):
        command += ["-o", str(log) + ",txt"]
    started = time.monotonic()
    try:
        completed = subprocess.run(command, cwd=ROOT, env=env, capture_output=True,
            timeout=max(330, properties.get("TIMEOUT", 0)),
            creationflags=subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0)
        console = (completed.stdout + completed.stderr).decode("utf-8", errors="replace")
        report = log.read_text(encoding="utf-8", errors="replace") if log.exists() else ""
        text = report + console
        log.write_text(text, encoding="utf-8")
        totals = re.search(r"Totals: (\d+) passed, (\d+) failed, (\d+) skipped", text)
        errors = [line for line in text.splitlines() if RUNTIME_ERRORS.search(line)]
        result = {"name": name, "code": completed.returncode,
            "seconds": round(time.monotonic() - started, 2),
            "counts": list(map(int, totals.groups())) if totals else None,
            "errors": errors, "warnings": [line for line in text.splitlines() if "QWARN" in line],
            "passed": completed.returncode == 0 and not errors and (not qml or totals is not None)}
    except subprocess.TimeoutExpired:
        result = {"name": name, "passed": False, "errors": ["Timeout"],
            "seconds": round(time.monotonic() - started, 2)}
    (args.output / (name + ".json")).write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"{'PASS' if result['passed'] else 'FAIL'} {name} {result.get('counts')}", flush=True)
    return result


def run(args):
    args.output = args.output.resolve()
    args.output.mkdir(parents=True, exist_ok=True)
    inventory = json.loads(subprocess.check_output(["ctest", "--test-dir", str(args.build.resolve()),
        "--show-only=json-v1"], encoding="utf-8"))
    (args.output / "ctest-inventory.json").write_text(json.dumps(inventory, indent=2), encoding="utf-8")
    tests = [test for test in inventory["tests"] if re.search(args.only, test["name"])]
    if not tests:
        raise RuntimeError("No matching tests were registered")
    environment = dict(os.environ, PATH=str(args.qt_bin.resolve()) + os.pathsep + os.environ["PATH"],
        QT_QUICK_BACKEND="software", QT_QUICK_CONTROLS_STYLE="" if args.qt5_interpreter else "Basic",
        QT_QPA_PLATFORM="offscreen", QT_QPA_FONTDIR="C:/Windows/Fonts", QT_FORCE_STDERR_LOGGING="1")
    if args.qt5_interpreter:
        environment["QV4_FORCE_INTERPRETER"] = "1"
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        results = list(pool.map(lambda test: execute(test, args, environment), tests))
    (args.output / "summary.json").write_text(json.dumps(results, ensure_ascii=False, indent=2), encoding="utf-8")
    print("TOTAL", len(results), "FAILED", sum(not result["passed"] for result in results), flush=True)
    return 0 if all(result["passed"] for result in results) else 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--qt-bin", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--only", default=".*")
    parser.add_argument("--jobs", type=int, default=1)
    parser.add_argument("--qt5-interpreter", action="store_true")
    sys.exit(run(parser.parse_args()))
