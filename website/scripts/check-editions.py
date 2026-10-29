"""Verify edition guides and changelogs against a running production preview."""

import os
import sys
from pathlib import Path
from urllib.parse import urlsplit

from playwright.sync_api import expect, sync_playwright


base_url = (sys.argv[1] if len(sys.argv) > 1 else "http://127.0.0.1:4173/EUI/").rstrip("/") + "/"
artifacts = Path(__file__).resolve().parents[1] / "artifacts"
artifacts.mkdir(exist_ok=True)
failures = []


def visit(page, route=""):
    response = page.goto(base_url + route)
    assert response and response.ok, route
    page.wait_for_load_state("networkidle")
    page.evaluate("document.fonts.ready")


with sync_playwright() as playwright:
    browser = playwright.chromium.launch(headless=True, executable_path=os.environ.get("CHROMIUM_EXECUTABLE"))
    page = browser.new_page(viewport={"width": 1440, "height": 1000}, reduced_motion="reduce")
    page.on("pageerror", lambda error: failures.append(str(error)))
    page.on("response", lambda response: failures.append(f"{response.status}: {response.url}") if response.status >= 400 else None)
    for language in ["", "en/"]:
        visit(page, language)
        expect(page.locator('.docs-home a[href$="/changelog.html"]')).to_have_count(0)
        changelog = page.locator(".VPNavBar").get_by_role("link", name="Changelog" if language else "更新记录", exact=True)
        expect(changelog).to_have_count(1)
        page.screenshot(path=str(artifacts / ("editions-home-en.png" if language else "editions-home-zh.png")))
        changelog.click()
        page.wait_for_url(base_url + language + "changelog.html")
        expect(page.locator('.edition-switch a[aria-current="page"]')).to_have_text("C++ / Qt Quick")
        for index, edition in enumerate(["", "pyside6/"]):
            visit(page, language)
            card = page.locator(".version-card").nth(index)
            guide = card.locator(".version-card-actions a").nth(1)
            expect(guide).to_have_attribute("href", urlsplit(base_url).path + language + edition + "guide/getting-started.html")
            guide.click()
            page.wait_for_url(base_url + language + edition + "guide/getting-started.html")
            expect(page.locator('.edition-switch a[aria-current="page"]')).to_have_text("Python / PySide6" if edition else "C++ / Qt Quick")
        for route in ["guide/integration.html", "changelog.html"]:
            visit(page, language + route)
            cpp_content = page.locator(".vp-doc").inner_text()
            page.locator(".edition-switch").get_by_role("link", name="Python / PySide6", exact=True).click()
            page.wait_for_url(base_url + language + "pyside6/" + route)
            expect(page.locator('.edition-switch a[aria-current="page"]')).to_have_text("Python / PySide6")
            python_content = page.locator(".vp-doc").inner_text()
            assert python_content != cpp_content
            if route.startswith("guide/"):
                assert "from eui import initialize" in python_content and "CMakeLists.txt" not in python_content
                links = page.locator('.VPSidebar a[href*="/guide/"]').evaluate_all("elements => elements.map(element => element.getAttribute('href'))")
                assert links and all("/pyside6/guide/" in link for link in links), links
            else:
                assert "Python/PySide6" in python_content and "2026-10-20" not in python_content
                assert page.locator(".vp-doc li").count() == 5
                page.screenshot(path=str(artifacts / ("editions-python-log-en.png" if language else "editions-python-log-zh.png")))
            page.locator(".edition-switch").get_by_role("link", name="C++ / Qt Quick", exact=True).click()
            page.wait_for_url(base_url + language + route)
            expect(page.locator('.edition-switch a[aria-current="page"]')).to_have_text("C++ / Qt Quick")
            assert page.locator(".vp-doc").inner_text() == cpp_content, "Switching back must restore the C++ content"
        visit(page, language + "versions.html")
        page.locator('.version-option-actions a[href*="/pyside6/guide/"]').click()
        page.wait_for_url(base_url + language + "pyside6/guide/getting-started.html")
    for route in ["guide/integration.html", "changelog.html"]:
        visit(page, "pyside6/" + route)
        page.get_by_role("button", name="切换语言", exact=True).click()
        page.get_by_role("link", name="English", exact=True).click()
        page.wait_for_url(base_url + "en/pyside6/" + route)
        expect(page.locator('.edition-switch a[aria-current="page"]')).to_have_text("Python / PySide6")
        page.reload()
        expect(page.locator('.edition-switch a[aria-current="page"]')).to_have_text("Python / PySide6")
    for width in [320, 390, 768, 1440]:
        page.set_viewport_size({"width": width, "height": 900})
        for dark in [False, True]:
            page.evaluate("dark => document.documentElement.classList.toggle('dark', dark)", dark)
            for route in ["", "en/", "changelog.html", "pyside6/changelog.html", "en/pyside6/guide/integration.html"]:
                visit(page, route)
                page.evaluate("dark => document.documentElement.classList.toggle('dark', dark)", dark)
                assert page.evaluate("document.documentElement.scrollWidth <= innerWidth + 1"), (width, dark, page.url)
                if width == 390 and route == "pyside6/changelog.html":
                    page.screenshot(path=str(artifacts / f"editions-mobile-{'dark' if dark else 'light'}.png"))
    assert not failures, failures
    browser.close()

print("PASS: edition home links, on-site bilingual guides, matching sidebars, separate changelogs, chapter/language switching, reloads, themes, and 320–1440px layouts.")
