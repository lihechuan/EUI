"""Exercise the compiled QML runtime through the production website UI."""

import io
import os
import re
import sys
from pathlib import Path

from PIL import Image
from playwright.sync_api import expect, sync_playwright


base = (sys.argv[1] if len(sys.argv) > 1 else 'http://127.0.0.1:4173/EUI/').rstrip('/') + '/'
artifacts = Path(__file__).resolve().parents[1] / 'artifacts'
artifacts.mkdir(exist_ok=True)
failures = []
imports = 'import EUI 1.0\nimport QtQuick 2.15\n\n'


def run(page, source):
    page.get_by_role('textbox', name='QML 代码').fill(source)
    page.get_by_role('textbox', name='QML 代码').press('Control+Enter')


def output(page, text):
    try:
        expect(page.locator('.qml-console pre')).to_contain_text(text, timeout=10000)
    except AssertionError:
        page.screenshot(path=str(artifacts / 'playground-failure.png'), full_page=True)
        raise


def canvas(page):
    return page.frame_locator('.qml-preview iframe').locator('canvas')


def background(page):
    return Image.open(io.BytesIO(canvas(page).screenshot())).convert('RGB').getpixel((5, 5))


with sync_playwright() as playwright:
    browser = playwright.chromium.launch(headless=True, executable_path=os.environ.get('CHROMIUM_EXECUTABLE'), args=['--enable-unsafe-swiftshader'])
    context = browser.new_context(viewport={'width': 1440, 'height': 1100})
    page = context.new_page()
    page.on('pageerror', lambda error: failures.append(str(error)))
    page.on('response', lambda response: failures.append(f'{response.status}: {response.url}') if response.status >= 400 else None)
    page.goto(base + 'playground.html')
    expect(page.locator('.qml-empty')).to_be_visible()
    expect(page.locator('.qml-highlight .shiki')).to_be_visible()
    assert not any('/playground/eui-playground.wasm' in request['name'] for request in page.evaluate('performance.getEntriesByType("resource")')), 'Runtime must be lazy-loaded'
    assert page.locator('.qml-playground').bounding_box()['width'] > 1000, 'Standalone editor should use the available page width'

    page.get_by_role('button', name='运行', exact=True).first.click()
    expect(page.locator('.qml-status')).to_contain_text('已运行', timeout=60000)
    expect(page.locator('[role="alert"]')).to_have_count(0)
    assert background(page) == (255, 255, 255)
    page.screenshot(path=str(artifacts / 'playground-light.png'))

    run(page, imports + '''Item {
    property int count: 0

    EButton {
        x: 40
        y: 40
        width: 200
        height: 42
        text: "测试按钮"
        styleType: EStyle.Primary

        onClicked: parent.count += 1
    }
    EText {
        y: 100
        text: "计数：" + parent.count

        onTextChanged: console.log("bound", text)
    }
}''')
    output(page, 'bound 计数：0')
    canvas(page).click(position={'x': 100, 'y': 84})
    output(page, 'bound 计数：1')
    page.keyboard.press('Space')
    output(page, 'bound 计数：2')

    run(page, imports + 'Item {\n    nonexistentProperty: true\n}')
    expect(page.locator('[role="alert"]')).to_contain_text('5:', timeout=10000)
    expect(page.locator('[role="alert"]')).to_contain_text('nonexistentProperty')
    canvas(page).click(position={'x': 100, 'y': 84})
    page.keyboard.press('Space')
    output(page, 'bound 计数：3')

    run(page, imports + '''Item {
    EInput {
        x: 40
        y: 40
        width: 260
        clearable: true
        placeholderText: "请输入姓名"

        onTextChanged: console.log("input", text)
    }
}''')
    expect(page.locator('[role="alert"]')).to_have_count(0)
    canvas(page).click(position={'x': 90, 'y': 79})
    page.keyboard.type('Alice')
    output(page, 'input Alice')
    page.keyboard.insert_text('中文')
    output(page, 'input Alice中文')
    page.frame_locator('.qml-preview iframe').locator('#ime').evaluate('''element => {
        element.dispatchEvent(new CompositionEvent('compositionstart', {data: ''}))
        element.dispatchEvent(new CompositionEvent('compositionupdate', {data: '测'}))
        element.dispatchEvent(new CompositionEvent('compositionend', {data: '测试'}))
    }''')
    output(page, 'input Alice中文测试')
    page.keyboard.press('Control+a')
    page.keyboard.type('Bob')
    output(page, 'input Bob')
    page.keyboard.press('Backspace')
    output(page, 'input Bo')
    canvas(page).click(position={'x': 305, 'y': 79})
    expect(page.locator('.qml-console pre')).to_have_text(re.compile(r'.*input\s*$', re.S))

    run(page, imports + '''Item {
    EFloatLabel {
        x: 40
        y: 40
        width: 260
        text: "姓名"

        EInput {
            width: parent.width

            onTextChanged: console.log("floating", text)
            onActiveFocusChanged: console.log("focus", activeFocus)
        }
    }
}''')
    expect(page.locator('[role="alert"]')).to_have_count(0)
    canvas(page).click(position={'x': 90, 'y': 99})
    output(page, 'focus true')
    page.keyboard.type('EUI')
    output(page, 'floating EUI')
    page.screenshot(path=str(artifacts / 'playground-float-label.png'))

    page.get_by_role('switch').click()
    page.wait_for_function('document.documentElement.classList.contains("dark")')
    expect(page.locator('.qml-highlight pre')).to_have_class(re.compile('github-dark'))
    assert background(page) == (23, 28, 36)
    page.frame_locator('.qml-preview iframe').locator('#ime').focus()
    page.keyboard.type('2')
    output(page, 'floating EUI2')
    page.screenshot(path=str(artifacts / 'playground-dark.png'))
    page.get_by_role('button', name='重启预览').click()
    expect(page.locator('.qml-status')).to_contain_text('已运行', timeout=60000)
    assert background(page) == (23, 28, 36)

    run(page, '')
    expect(page.locator('[role="alert"]')).to_have_text('请输入 QML 代码。')
    run(page, 'x' * 65537)
    expect(page.locator('[role="alert"]')).to_have_text('代码不能超过 64 KB。')
    run(page, 'import QtQml 2.15\nQtObject {}')
    expect(page.locator('[role="alert"]')).to_contain_text('visual QML item')

    page.get_by_role('button', name='恢复示例').click()
    expect(page.locator('[role="alert"]')).to_have_count(0)
    expect(page.get_by_role('textbox', name='QML 代码')).to_have_value(re.compile('.*EButton.*', re.S))
    page.get_by_role('textbox', name='QML 代码').fill('changed')
    page.get_by_role('button', name='恢复示例').click()
    expect(page.get_by_role('textbox', name='QML 代码')).to_have_value(re.compile('.*点击次数.*', re.S))

    for component in ['EButton', 'EInput', 'EFloatLabel', 'EIcon', 'ESpinBox', 'ETable']:
        page.goto(base + f'components/{component}.html')
        expect(page.locator('.qml-playground textarea').first).to_have_value(re.compile('.*' + component + '.*', re.S))
        expect(page.locator('.qml-status').first).to_contain_text('已运行', timeout=60000)
        expect(page.locator('.qml-error')).to_have_count(0)
        expect(page.locator('.component-demo, .demo-stage')).to_have_count(0)

    page.goto(base + 'en/playground.html')
    expect(page.get_by_role('heading', level=1)).to_have_text('QML playground')
    expect(page.get_by_role('textbox', name='QML source')).to_have_value(re.compile('.*Clicks:.*', re.S))
    page.get_by_role('button', name='Run', exact=True).first.click()
    expect(page.locator('.qml-status')).to_contain_text('Running', timeout=60000)
    expect(page.locator('[role="alert"]')).to_have_count(0)
    page.set_viewport_size({'width': 390, 'height': 844})
    assert page.evaluate('document.documentElement.scrollWidth <= innerWidth + 1')
    expect(canvas(page)).to_be_visible()
    page.screenshot(path=str(artifacts / 'playground-mobile.png'))
    browser.close()

assert not failures, '\n'.join(failures)
print('PASS: genuine EUI click/input/binding, Chinese text, errors and recovery, theme, reset/restart, lazy loading, locales, component pages, responsive layout.')
