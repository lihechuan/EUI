"""Create every published browser example in the compiled Qt runtime."""

import html
import json
import os
import re
import sys
from pathlib import Path

from playwright.sync_api import expect, sync_playwright


website = Path(__file__).resolve().parents[1]
base = (sys.argv[1] if len(sys.argv) > 1 else 'http://127.0.0.1:4173/EUI/').rstrip('/') + '/'
failures = []
count = 0
native = set()
components = set()
warnings = re.compile(r'Error:|Binding loop|Unable to assign|Cannot assign|not a type|failed to load', re.I)

with sync_playwright() as playwright:
    browser = playwright.chromium.launch(headless=True, executable_path=os.environ.get('CHROMIUM_EXECUTABLE'), args=['--enable-unsafe-swiftshader'])
    page = browser.new_page(viewport={'width': 1440, 'height': 1100})
    page.on('pageerror', lambda error: failures.append(str(error)))
    page.add_init_script('window.qmlLogs = []; addEventListener("message", event => { if (event.data?.channel === "eui-qml" && event.data.type === "log") qmlLogs.push(event.data.message) })')
    for language, prefix, dark in [('zh', '', False), ('en', 'en/', True)]:
        page.goto(base + prefix + 'components/EIcon.html')
        expect(page.locator('.qml-status').first).to_contain_text('已运行' if language == 'zh' else 'Running', timeout=90000)
        runtime = next(frame for frame in page.frames if '/playground/runtime.html' in frame.url)
        directory = website / '.generated' / prefix / 'components'
        for file in sorted(directory.glob('E*.md')):
            encoded = re.findall(r':examples="([^"]+)"', file.read_text(encoding='utf-8'))
            samples = [sample for value in encoded for sample in json.loads(html.unescape(value))]
            for index, sample in enumerate(samples):
                if sample.get('reason'):
                    native.add(file.stem)
                    continue
                page.evaluate('qmlLogs = []')
                error = runtime.evaluate('(data) => Module.runQml(data.code, data.dark, data.language)', {'code': sample['code'], 'dark': dark, 'language': language})
                page.wait_for_timeout(40)
                logs = page.evaluate('qmlLogs')
                if error or any(warnings.search(message) for message in logs):
                    failures.append(f'{language}/{file.stem} [{index}: {sample["title"]}]: {error} {logs}')
                count += 1
                components.add(file.stem)
        print(f'Checked {language}: {count} browser examples', flush=True)
    browser.close()

assert native == {'EECharts', 'EMediaPlayer', 'EFilePicker', 'EFolderPicker', 'EUpload'}, native
assert len(components) == 147, len(components)
assert not failures, '\n'.join(failures)
print(f'PASS: {count} localized examples, {len(components)} browser components, light/dark rendering, 5 explicit desktop-only components.')
