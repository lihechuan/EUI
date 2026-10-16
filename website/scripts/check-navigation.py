"""Verify catalog typography and shared navigation against a running docs site."""

import os
import sys
from pathlib import Path

from playwright.sync_api import expect, sync_playwright


base_url = (sys.argv[1] if len(sys.argv) > 1 else "http://127.0.0.1:4173/EUI/").rstrip("/") + "/"
artifacts = Path(__file__).resolve().parents[1] / "artifacts"
artifacts.mkdir(exist_ok=True)
failures = []


def visit(page, route):
    response = page.goto(base_url + route)
    assert response and response.ok, route
    page.wait_for_load_state("networkidle")
    page.evaluate("document.fonts.ready")


def check_font(page, selector):
    expect(page.locator(selector).first).to_be_visible()
    assert page.locator(selector).evaluate_all("elements => elements.length > 0 && elements.every(element => getComputedStyle(element).fontFamily === getComputedStyle(document.body).fontFamily)"), f"Index text must use the body font: {selector}"


def check_navigation(page):
    expect(page.locator(".VPDocAsideOutline a").first).to_be_attached()
    if page.viewport_size["width"] >= 960:
        group = page.locator(".VPSidebarItem.level-0").filter(has=page.locator('a[href$="/EApp.html"]'))
        if "collapsed" in (group.get_attribute("class") or "").split():
            group.locator(":scope > .item").click()
    assert page.locator(".VPNavBarTitle .title").evaluate("element => getComputedStyle(element).flexDirection") == "row", "Navigation logo and title must remain side by side"
    assert page.locator(".VPSidebarItem.level-1 .link").evaluate_all("elements => elements.length > 0 && elements.every(element => getComputedStyle(element).flexDirection === 'row' && getComputedStyle(element).borderBottomWidth === '0px')"), "Catalog styles must not alter sidebar rows"
    assert page.locator(".VPDocAsideOutline a").evaluate_all("elements => elements.length > 0 && elements.every(element => getComputedStyle(element).borderBottomWidth === '0px')"), "Catalog styles must not add borders to outline links"
    assert page.evaluate("document.documentElement.scrollWidth <= innerWidth + 1"), page.url
    return page.locator('.VPSidebarItem.level-1 a[href$="/EApp.html"]').evaluate("element => { const style = getComputedStyle(element); return { direction: style.flexDirection, padding: style.padding, height: element.getBoundingClientRect().height, border: style.borderBottomWidth, font: style.fontFamily } }")


with sync_playwright() as playwright:
    browser = playwright.chromium.launch(headless=True, executable_path=os.environ.get("CHROMIUM_EXECUTABLE"))
    context = browser.new_context(viewport={"width": 1440, "height": 1000}, reduced_motion="reduce")
    page = context.new_page()
    page.on("pageerror", lambda error: failures.append(str(error)))
    for language in ["", "en/"]:
        visit(page, language)
        check_font(page, "#component-list a > span")
        page.locator('.intro-actions a[href$="/components/"]').click()
        page.wait_for_url(base_url + language + "components/")
        check_font(page, ".component-links a span, .component-links small")
        catalog_style = check_navigation(page)
        page.screenshot(path=str(artifacts / ("catalog-type-" + ("en" if language else "zh") + ".png")))
        page.locator('.component-links a[href="./EApp.html"]').click()
        page.wait_for_url("**/components/EApp.html")
        assert check_navigation(page) == catalog_style, "Sidebar row styles must match between the catalog and component pages"
        page.get_by_role("link", name="All components" if language else "全部组件", exact=True).click()
        page.wait_for_url(base_url + language + "components/")
        assert check_navigation(page) == catalog_style
        page.reload()
        page.wait_for_load_state("networkidle")
        assert check_navigation(page) == catalog_style
        for route in ["components/", "components/EApp.html"]:
            visit(page, language + route)
            for dark in [False, True]:
                if page.locator("html").evaluate("element => element.classList.contains('dark')") != dark:
                    page.locator(".VPNavBarAppearance button:visible").click()
                for width in [768, 1024, 1440]:
                    page.set_viewport_size({"width": width, "height": 1000})
                    check_navigation(page)
                page.set_viewport_size({"width": 390, "height": 844})
                page.get_by_role("button", name="Menu" if language else "菜单", exact=True).click()
                expect(page.locator(".VPSidebar.open")).to_be_visible()
                assert page.locator(".VPSidebarItem.level-1 .link").evaluate_all("elements => elements.every(element => getComputedStyle(element).flexDirection === 'row')")
                page.locator(".VPBackdrop").click(position={"x": 380, "y": 400})
                expect(page.locator(".VPSidebar.open")).to_have_count(0)
                assert page.evaluate("document.documentElement.scrollWidth <= innerWidth + 1")
                page.set_viewport_size({"width": 1440, "height": 1000})
    assert not failures, failures
    browser.close()

print("PASS: body fonts in both indexes; matching sidebar, header, and outline styles after direct loads and navigation; both languages, themes, and mobile layouts.")
