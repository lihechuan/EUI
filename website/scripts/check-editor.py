"""Check real caret placement and edits with native and overlay scrollbars."""

import os
import re
import sys

from playwright.sync_api import expect, sync_playwright


base = (sys.argv[1] if len(sys.argv) > 1 else 'http://127.0.0.1:4173/EUI/').rstrip('/') + '/'


def wait_highlight(editor, source):
    editor.page.wait_for_function(
        r'([element, source]) => element.querySelector("code")?.textContent === source + "\n"',
        arg=[editor.locator('.qml-highlight').element_handle(), source],
    )


def edit_at_visible_text(editor, source, line_index, column, key, *, bottom=False, right=False):
    textarea = editor.locator('textarea')
    textarea.fill(source)
    wait_highlight(editor, source)
    textarea.scroll_into_view_if_needed()
    textarea.evaluate('''(element, options) => {
        element.scrollTop = options.bottom ? element.scrollHeight
            : Math.max(0, options.line * parseFloat(getComputedStyle(element).lineHeight) - 160);
        element.scrollLeft = options.right ? element.scrollWidth : 0;
        element.dispatchEvent(new Event('scroll'));
    }''', {'line': line_index, 'bottom': bottom, 'right': right})
    point = editor.locator('.qml-highlight .line').nth(line_index).evaluate('''(line, column) => {
        const walker = document.createTreeWalker(line, NodeFilter.SHOW_TEXT);
        let remaining = column;
        let node;
        while ((node = walker.nextNode())) {
            if (remaining <= node.length) {
                const range = document.createRange();
                range.setStart(node, remaining - 1);
                range.setEnd(node, remaining);
                const rect = range.getBoundingClientRect();
                return {x: rect.right - 0.5, y: rect.y + rect.height / 2};
            }
            remaining -= node.length;
        }
        throw new Error('Cannot find the visible character');
    }''', column)
    bounds = textarea.bounding_box()
    assert bounds['x'] <= point['x'] < bounds['x'] + textarea.evaluate('e => e.clientWidth'), point
    assert bounds['y'] <= point['y'] < bounds['y'] + textarea.evaluate('e => e.clientHeight'), point
    editor.page.mouse.click(point['x'], point['y'])
    expected = sum(len(line) + 1 for line in source.split('\n')[:line_index]) + column
    actual = textarea.evaluate('e => e.selectionStart')
    assert actual == expected, f'Visible line {line_index + 1}, column {column}: expected offset {expected}, got {actual}'
    textarea.press(key)
    changed = source[:expected - 1] + source[expected:] if key == 'Backspace' else source[:expected] + source[expected + 1:]
    expect(textarea).to_have_value(changed)
    wait_highlight(editor, changed)


with sync_playwright() as playwright:
    for native_scrollbars in [True, False]:
        browser = playwright.chromium.launch(
            headless=True,
            executable_path=os.environ.get('CHROMIUM_EXECUTABLE'),
            ignore_default_args=['--hide-scrollbars'] if native_scrollbars else [],
        )
        page = browser.new_page(viewport={'width': 1440, 'height': 1100})
        # Isolate the editor from the independent WebAssembly runtime during this fast regression.
        page.route('**/playground/runtime.html?*', lambda route: route.fulfill(content_type='text/html', body=''))
        for route, dark, width in [
            ('components/EIcon.html', False, 1440),
            ('en/components/EIcon.html', True, 555),
            ('playground.html', True, 1440),
            ('en/playground.html', False, 555),
        ]:
            page.set_viewport_size({'width': width, 'height': 1100})
            page.emulate_media(color_scheme='dark' if dark else 'light')
            page.goto(base + route)
            editor = page.locator('.qml-playground').first
            expect(editor.locator('.qml-highlight code')).to_be_attached()
            expect(editor.locator('.qml-highlight pre')).to_have_class(re.compile('github-dark' if dark else 'github-light'))
            page.evaluate('document.fonts.ready')
            source = editor.locator('textarea').input_value()
            if 'components/' in route:
                line_index = next(i for i, line in enumerate(source.split('\n')) if 'text: modelData.label' in line)
                edit_at_visible_text(editor, source, line_index, len(source.split('\n')[line_index]), 'Backspace', bottom=True)

            lines = ['import EUI 1.0', 'import QtQuick 2.15', '', 'Item {']
            lines += [f'    property int value{i}: {i}' for i in range(45)]
            lines += ['', '\t\ttext: "中文 tab 测试"', '    anchors.verticalCenter: parent.verticalCenter',
                      '    property string note: "' + 'long code => != === '.join(['test'] * 12) + '"', '}', '', '']
            fixture = '\n'.join(lines)
            for index, column, key, options in [
                (0, 10, 'Delete', {}),
                (25, 17, 'Backspace', {}),
                (50, len(lines[50]), 'Backspace', {'bottom': True}),
                (51, 12, 'Delete', {'bottom': True}),
                (52, len(lines[52]) - 1, 'Backspace', {'bottom': True, 'right': True}),
            ]:
                edit_at_visible_text(editor, fixture, index, column, key, **options)
            # Appending at the final blank line also refreshes the highlighted DOM while scrolled.
            textarea = editor.locator('textarea')
            textarea.fill(fixture)
            textarea.press('Control+End')
            textarea.press('Enter')
            textarea.press('Tab')
            textarea.type('text: "end"')
            wait_highlight(editor, textarea.input_value())
            print(f'PASS: {route}, {"native" if native_scrollbars else "overlay"} scrollbars, {"dark" if dark else "light"}, width {width}')
        browser.close()
