"""Verify documentation screenshots follow the website theme."""

import os
import re
import sys
from pathlib import Path
from urllib.parse import urljoin

from playwright.sync_api import expect, sync_playwright


base = (sys.argv[1] if len(sys.argv) > 1 else 'http://127.0.0.1:4173/EUI/').rstrip('/') + '/'
artifacts = Path(__file__).resolve().parents[1] / 'artifacts' / 'screenshot-theme'
artifacts.mkdir(parents=True, exist_ok=True)


def check_images(page, language, dark, count):
    images = page.locator('.vp-doc img[data-preview-theme]:visible, .screenshot-grid a[data-preview-theme]:visible img')
    expect(images).to_have_count(count)
    for image in images.all():
        source = image.get_attribute('src')
        assert source.endswith(f'-{language}.png'), source
        assert ('-dark-' in source) == dark, f'Wrong screenshot theme: {source}'
        image.scroll_into_view_if_needed()
        image.evaluate('image => image.decode()')
        assert image.evaluate('image => image.naturalWidth > 0 && image.naturalHeight > 0'), source
        brightness = image.evaluate('''image => {
            const canvas = document.createElement('canvas');
            canvas.width = canvas.height = 1;
            const context = canvas.getContext('2d');
            context.drawImage(image, 20, 400, 1, 1, 0, 0, 1, 1);
            const [r, g, b] = context.getImageData(0, 0, 1, 1).data;
            return (r + g + b) / 3;
        }''')
        assert (brightness < 128) == dark, f'Wrong rendered screenshot colors: {source} ({brightness})'
    if count > 1:
        links = page.locator('.screenshot-grid a[data-preview-theme]:visible')
        expect(links).to_have_count(count)
        for link in links.all():
            source = link.locator('img').get_attribute('src')
            assert urljoin(page.url, link.get_attribute('href')) == urljoin(page.url, source), source
        source_link = links.last
        source = source_link.locator('img').get_attribute('src')
        with page.expect_popup() as popup_info:
            source_link.click()
        popup = popup_info.value
        popup.wait_for_load_state('load')
        assert popup.url == urljoin(page.url, source), popup.url
        expect(popup.locator('img')).to_be_visible()
        popup.close()


with sync_playwright() as playwright:
    browser = playwright.chromium.launch(headless=True, executable_path=os.environ.get('CHROMIUM_EXECUTABLE'))
    for language, prefix, dark_title, light_title in [
        ('zh', '', '切换到深色模式', '切换到浅色模式'),
        ('en', 'en/', 'Switch to dark theme', 'Switch to light theme'),
    ]:
        context = browser.new_context(viewport={'width': 1440, 'height': 1000}, color_scheme='light', reduced_motion='reduce')
        page = context.new_page()
        for route, count in [('gallery', 1), ('features', 1), ('screenshots', 19)]:
            page.goto(base + prefix + f'guide/{route}.html')
            page.wait_for_load_state('networkidle')
            check_images(page, language, False, count)
            page.get_by_role('switch', name=dark_title).click()
            expect(page.locator('html')).to_have_class(re.compile(r'\bdark\b'))
            check_images(page, language, True, count)
            if route == 'gallery':
                page.evaluate('scrollTo(0, 0)')
                page.screenshot(path=str(artifacts / f'gallery-dark-{language}.png'))
                page.reload()
                page.wait_for_load_state('networkidle')
                check_images(page, language, True, count)
            page.get_by_role('switch', name=light_title).click()
            check_images(page, language, False, count)
        context.close()
        context = browser.new_context(viewport={'width': 1440, 'height': 1000}, color_scheme='dark', reduced_motion='reduce')
        page = context.new_page()
        page.goto(base + prefix + 'guide/gallery.html')
        page.wait_for_load_state('networkidle')
        check_images(page, language, True, 1)
        page.get_by_role('switch', name=light_title).click()
        check_images(page, language, False, 1)
        context.close()
    browser.close()

print('PASS: Chinese and English screenshots switch both ways, retain dark mode after reload, load real theme colors, and open matching originals')
