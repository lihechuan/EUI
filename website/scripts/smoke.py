"""Run against a running production preview: python scripts/smoke.py [base-url]."""

import os
import re
import sys
from pathlib import Path

from playwright.sync_api import expect, sync_playwright


base_url = (sys.argv[1] if len(sys.argv) > 1 else "http://127.0.0.1:4173/EUI/").rstrip("/") + "/"
artifacts = Path(__file__).resolve().parents[1] / "artifacts"
artifacts.mkdir(exist_ok=True)
failures = []


def visit(page, route=""):
    response = page.goto(base_url + route)
    assert response and response.ok, f"Cannot open {route}"
    page.wait_for_load_state("networkidle")
    page.evaluate("document.fonts.ready")


def check_width(page):
    assert page.evaluate("document.documentElement.scrollWidth <= innerWidth + 1"), page.url


def check_pager(page):
    footer = page.locator(".VPDocFooter .prev-next")
    footer.scroll_into_view_if_needed()
    for direction in ["prev", "next"]:
        title = footer.locator(f".pager-link.{direction} .title")
        name = title.locator(".sidebar-code").bounding_box()
        label = title.locator(".sidebar-label").bounding_box()
        bounds = title.bounding_box()
        if abs(label["y"] - name["y"]) < 2:
            assert label["x"] - name["x"] - name["width"] >= 7, "Pager component names need spacing"
        else:
            assert label["y"] >= name["y"] + name["height"] - 1, "Wrapped pager names must not overlap"
        if direction == "prev":
            assert abs(name["x"] - bounds["x"]) < 1, "Previous title must align left"
        else:
            assert abs(label["x"] + label["width"] - bounds["x"] - bounds["width"]) < 1, "Next title must align right"
        assert title.evaluate("e => e.scrollWidth <= e.clientWidth + 1"), "Pager title must fit its card"


def check_home_images(page):
    page.locator(".showcase").scroll_into_view_if_needed()
    expect(page.locator(".docs-home img")).to_have_count(2)
    expect(page.locator('.docs-home img[src*="/components/"]')).to_have_count(0)
    page.wait_for_function("[...document.querySelectorAll('.docs-home img')].filter(image => image.getClientRects().length).every(image => image.complete && image.naturalWidth > 0)")
    assert page.locator(".gallery-image img:visible").evaluate("image => Math.abs(image.clientWidth / image.clientHeight - image.naturalWidth / image.naturalHeight) < 0.01 && image.parentElement.clientHeight >= image.clientHeight - 1 && image.parentElement.clientWidth >= image.clientWidth - 1"), "Gallery screenshot must fit in full at its original aspect ratio"
    page.evaluate("scrollTo(0, 0)")


def check_original(page, link):
    with page.expect_popup() as popup_info:
        link.click()
    popup = popup_info.value
    popup.wait_for_load_state("load")
    response = page.request.get(popup.url)
    assert response.ok and response.headers.get("content-type", "").startswith("image/png"), f"Original image failed: {popup.url} ({response.status})"
    assert popup.url.startswith(base_url + "screenshots/"), popup.url
    expect(popup.locator("img")).to_be_visible()
    popup.close()


def observe(page):
    page.on("pageerror", lambda error: failures.append(str(error)))
    page.on("response", lambda response: failures.append(f"{response.status}: {response.url}") if response.status >= 400 else None)


with sync_playwright() as playwright:
    browser = playwright.chromium.launch(headless=True, executable_path=os.environ.get("CHROMIUM_EXECUTABLE"), args=["--enable-unsafe-swiftshader"])
    context = browser.new_context(viewport={"width": 1440, "height": 1000}, permissions=["clipboard-read", "clipboard-write"])
    page = context.new_page()
    observe(page)
    visit(page)
    expect(page.get_by_role("heading", level=1)).to_have_text("EUI 文档")
    expect(page.locator(".intro-art .brand-structure")).to_be_visible()
    expect(page.locator(".component-workbench, .preview-code, .preview-tabs")).to_have_count(0)
    assert page.evaluate("[...document.fonts].some(font => font.family === 'Noto Sans SC' && font.status === 'loaded')")
    page.emulate_media(reduced_motion="reduce")
    assert page.locator(".brand-emblem").evaluate("e => getComputedStyle(e).animationName") == "none"
    page.emulate_media(reduced_motion="no-preference")
    expect(page.locator(".gallery-image img:visible")).to_have_attribute("src", re.compile(r"/home-zh\.png$"))
    categories = page.locator('.category-selector input[type="radio"]')
    expect(categories).to_have_count(9)
    component_names = set()
    for category in categories.all():
        category_name = category.locator("..").locator("strong").inner_text()
        category.locator("..").click()
        expect(category).to_be_checked()
        expect(page.locator("#selected-group-title")).to_contain_text(category_name)
        component_names.update(page.locator(".home-component-list a > span").all_text_contents())
        assert page.locator(".home-component-list a").count() == int(page.locator("#selected-group-title span").inner_text())
    assert len(component_names) == 152, "Every documented component must be reachable from the homepage index"
    categories.first.focus()
    page.keyboard.press("ArrowDown")
    expect(categories.nth(1)).to_be_checked()
    expect(page.locator("#selected-group-title")).to_contain_text("配置组件")
    page.keyboard.press("ArrowUp")
    expect(categories.first).to_be_checked()
    check_home_images(page)
    page.screenshot(path=str(artifacts / "home-desktop.png"), full_page=True)
    check_width(page)

    page.get_by_role("link", name="浏览组件", exact=True).click()
    expect(page.get_by_role("heading", level=1)).to_contain_text("组件目录")
    page.locator('.vp-doc a[href="./EButton.html"]').click()
    expect(page.get_by_role("heading", level=1)).to_contain_text("EButton")
    assert page.locator(".api-property").count() > 10
    expect(page.locator(".qml-playground").first).to_be_visible()
    expect(page.locator(".qml-status").first).to_contain_text("已运行", timeout=60000)
    expect(page.locator(".qml-playground textarea").first).to_have_value(re.compile(".*EButton.*", re.S))
    page.screenshot(path=str(artifacts / "component-preview-desktop.png"))
    sidebar = page.locator(".VPSidebar").bounding_box()
    active = page.locator(".VPSidebarItem.is-active .sidebar-code").bounding_box()
    assert active["x"] + active["width"] < sidebar["x"] + sidebar["width"]
    source = page.locator(".qml-playground").first
    expect(page.locator(".qml-playground select")).to_have_count(0)
    editor = source.locator(".qml-editor").bounding_box()
    preview = source.locator(".qml-preview").bounding_box()
    assert preview['x'] >= editor['x'] + editor['width'] - 1
    assert abs(preview['y'] - editor['y']) < 1
    expect(source.locator(".qml-highlight .shiki")).to_be_visible()
    expect(source.locator("textarea")).to_have_value(re.compile(".*EButton.*", re.S))
    copy = source.get_by_role("button", name="复制代码", exact=True)
    copy.click()
    expect(source.get_by_role("button", name="已复制", exact=True)).to_be_visible()
    assert page.evaluate("navigator.clipboard.readText()").replace("\r\n", "\n") == source.locator("textarea").input_value()
    assert page.locator(".component-demo, .demo-stage").count() == 0
    page.screenshot(path=str(artifacts / "component-desktop.png"))
    edited = source.locator("textarea").input_value() + "\n"
    source.locator("textarea").fill(edited)
    another = page.locator(".qml-playground").nth(3)
    another.scroll_into_view_if_needed()
    expect(another.locator(".qml-status")).to_contain_text("已运行", timeout=60000)
    expect(source.locator("iframe")).to_have_count(0)
    assert page.locator(".qml-playground iframe").count() <= 3, "Only examples near the viewport should keep a runtime"
    source.scroll_into_view_if_needed()
    expect(source.locator(".qml-status")).to_contain_text("已运行", timeout=60000)
    expect(source.locator("textarea")).to_have_value(edited)

    visit(page, "components/EECharts.html")
    expect(page.locator(".qml-native-note").first).to_contain_text("Qt WebEngine")
    assert page.locator(".qml-playground").count() > 30
    expect(page.locator(".qml-playground select")).to_have_count(0)
    expect(page.locator(".qml-playground textarea").nth(35)).to_have_value(re.compile(".*EECharts.*", re.S))
    expect(page.locator(".qml-playground iframe")).to_have_count(0)
    check_width(page)

    visit(page, "components/EIcon.html#属性")
    expect(page.locator("#property-size .api-default-value")).to_have_text("EStyle.Medium")
    expect(page.locator("#property-type .api-details")).to_contain_text('"" / "primary"')
    assert page.locator("#property-font .api-details br").count() == 4
    page.screenshot(path=str(artifacts / "reference-desktop.png"))

    visit(page, "components/EList.html#信号")
    expect(page.locator("#signal-pageChanged")).to_contain_text("从 1 开始")
    expect(page.locator("#signal-itemClicked")).to_contain_text("从 0 开始")
    expect(page.locator("#method-setPage")).to_contain_text("禁用或页码无效时返回 false")
    expect(page.locator("#method-normalizePage")).to_contain_text("有效范围")
    expect(page.locator("#method-resolveColumns")).to_contain_text("列数")
    expect(page.locator(".api-interface a")).to_have_count(0)
    page.locator("#信号").scroll_into_view_if_needed()
    page.screenshot(path=str(artifacts / "interface-purpose-zh.png"))
    visit(page, "en/components/EList.html#signals")
    expect(page.locator("#signal-pageChanged")).to_contain_text("one-based")
    expect(page.locator("#method-setPage")).to_contain_text("false")
    expect(page.locator(".api-interface a")).to_have_count(0)
    page.screenshot(path=str(artifacts / "interface-purpose-en.png"))

    visit(page, "components/EFlex.html#属性")
    align = page.locator("#property-align")
    expect(align.locator(".api-property-values .api-details")).to_contain_text('"start" / "center"')
    expect(align.locator(".api-property-values .api-default-value")).to_have_text('"stretch"')
    description = align.locator(".api-property-body").bounding_box()
    values = align.locator(".api-property-values").bounding_box()
    assert values["x"] >= description["x"] + description["width"], "Property values must have their own column"
    assert abs(values["y"] - description["y"]) < 1
    page.screenshot(path=str(artifacts / "property-columns-desktop.png"))

    for query in ["任务看板", "ETaskBoard", "moveTask"]:
        page.get_by_role("button", name="搜索文档", exact=True).click()
        search = page.locator(".VPLocalSearchBox input")
        search.fill(query)
        assert search.evaluate("e => getComputedStyle(e).fontFamily === getComputedStyle(document.body).fontFamily"), "Search input must use the site font"
        result = page.locator('.VPLocalSearchBox a[href*="/components/ETaskBoard.html"]').first
        expect(result).to_be_visible()
        if query == "ETaskBoard":
            page.screenshot(path=str(artifacts / "search.png"))
        result.click()
        expect(page.get_by_role("heading", level=1)).to_contain_text("ETaskBoard")

    assert "taskMoved(" in page.locator(".vp-doc").inner_text()
    page.get_by_role("button", name="切换语言", exact=True).click()
    page.get_by_role("link", name="English", exact=True).click()
    page.wait_for_url("**/en/components/ETaskBoard.html*")
    expect(page.get_by_role("heading", level=1)).to_contain_text("Task board")
    expect(page.locator(".qml-status").first).to_contain_text("Running", timeout=60000)
    assert page.locator("html").get_attribute("lang") == "en"
    page.locator('button[title="Switch to dark theme"]:visible').click()
    expect(page.locator("html")).to_have_class("dark")
    page.reload()
    expect(page.locator("html")).to_have_class("dark")
    expect(page.locator(".qml-status").first).to_contain_text("Running", timeout=60000)
    page.locator(".qml-playground").first.scroll_into_view_if_needed()
    page.screenshot(path=str(artifacts / "component-dark-en.png"))

    for route in ["guide/build.html#23-只安装-qt-6-时构建", "guide/integration.html", "guide/features.html#45-图表选择", "en/guide/troubleshooting.html", "guide/screenshots.html", "changelog.html"]:
        visit(page, route)
        expect(page.locator(".vp-doc h1")).to_be_visible()
    for route in ["components/ETypography.html", "en/components/ETypography.html"]:
        visit(page, route)
        check_pager(page)
        page.locator(".VPDocFooter .prev-next").screenshot(path=str(artifacts / ("pager-desktop-en.png" if route.startswith("en/") else "pager-desktop-zh.png")))
    for language, title in [("", "6. 常见问题"), ("en/", "6. Troubleshooting")]:
        visit(page, language + "guide/troubleshooting.html")
        expect(page.locator(".vp-doc h1")).to_have_text(title)
        expect(page.locator('.VPSidebar a[href*="/guide/deployment"]')).to_have_count(0)
        page.screenshot(path=str(artifacts / ("guide-cleanup-en.png" if language else "guide-cleanup-zh.png")))
    visit(page, "en/components/EFlex.html#properties")
    expect(page.locator(".api-reference-head")).to_contain_text("Values and default")
    page.screenshot(path=str(artifacts / "property-columns-dark-en.png"))
    visit(page, "components/ETaskBoard.html#property-tasks")
    expect(page.locator("#property-tasks")).to_be_visible()
    visit(page, "en/")
    expect(page.get_by_role("heading", level=1)).to_have_text("EUI documentation")
    expect(page.locator(".intro-art .brand-structure")).to_be_visible()
    expect(page.locator(".gallery-image img:visible")).to_have_attribute("src", re.compile(r"/overview-dark-en\.png$"))
    check_home_images(page)
    page.screenshot(path=str(artifacts / "home-dark-en.png"), full_page=True)

    visit(page, "components/ESpinBox.html")
    expect(page.locator(".qml-status").first).to_contain_text("已运行", timeout=60000)
    expect(page.locator(".qml-playground textarea").first).to_have_value(re.compile(".*ESpinBox.*", re.S))
    page.screenshot(path=str(artifacts / "spinbox-dark.png"))
    page.locator('button[title="切换到浅色模式"]:visible').click()
    expect(page.locator("html")).not_to_have_class("dark")
    page.screenshot(path=str(artifacts / "spinbox-light.png"))

    visit(page, "changelog.html")
    expect(page.locator(".vp-doc h1")).to_have_text("更新记录")
    chinese_log = page.locator(".vp-doc").inner_text()
    assert "记录各版本的功能、修复和兼容性变化。" in chinese_log
    assert "English:" not in chinese_log and "Adds bundled local images" not in chinese_log
    versions = page.locator(".vp-doc h2").all_inner_texts()
    page.screenshot(path=str(artifacts / "changelog-zh.png"))
    page.get_by_role("button", name="切换语言", exact=True).click()
    page.get_by_role("link", name="English", exact=True).click()
    page.wait_for_url("**/en/changelog.html")
    expect(page.locator(".vp-doc h1")).to_have_text("Changelog")
    english_log = page.locator(".vp-doc").inner_text()
    assert "Features, fixes, and compatibility changes for each version." in english_log
    assert "Adds bundled local images" in english_log
    assert not re.search(r"[\u3400-\u9fff]", english_log), "English changelog must not contain Chinese text"
    assert page.locator(".vp-doc h2").all_inner_texts() == versions
    page.screenshot(path=str(artifacts / "changelog-en.png"))
    page.get_by_role("button", name="Change language", exact=True).click()
    page.get_by_role("link", name="简体中文", exact=True).click()
    page.wait_for_url(base_url + "changelog.html")
    expect(page.locator(".vp-doc h1")).to_have_text("更新记录")
    assert page.locator(".vp-doc").inner_text() == chinese_log

    mobile = browser.new_context(viewport={"width": 390, "height": 844}, is_mobile=True, has_touch=True)
    phone = mobile.new_page()
    observe(phone)
    visit(phone)
    check_width(phone)
    expect(phone.locator(".intro-art .brand-structure")).to_be_visible()
    expect(phone.locator(".component-workbench, .preview-code, .preview-tabs")).to_have_count(0)
    check_home_images(phone)
    phone.screenshot(path=str(artifacts / "home-mobile.png"), full_page=True)
    phone.get_by_role("button", name="mobile navigation", exact=True).click()
    phone.get_by_role("link", name="指南", exact=True).click()
    expect(phone.get_by_role("heading", level=1)).to_contain_text("快速开始")
    phone.get_by_role("button", name="菜单", exact=True).click()
    phone.get_by_role("link", name="接入应用", exact=True).click()
    expect(phone.get_by_role("heading", level=1)).to_contain_text("接入自己的")
    check_width(phone)
    visit(phone, "components/ETaskBoard.html")
    check_width(phone)
    expect(phone.locator(".qml-status").first).to_contain_text("已运行", timeout=60000)
    expect(phone.locator(".qml-playground textarea").first).to_have_value(re.compile(".*ETaskBoard.*", re.S))
    phone.screenshot(path=str(artifacts / "component-mobile.png"))
    visit(phone, "components/EButton.html")
    expect(phone.locator(".qml-playground select")).to_have_count(0)
    expect(phone.locator(".qml-playground textarea").nth(7)).to_have_value(re.compile(".*EButton.*", re.S))
    phone.locator(".qml-playground").nth(7).screenshot(path=str(artifacts / "code-mobile.png"))
    check_width(phone)
    visit(phone, "components/EIcon.html#属性")
    expect(phone.locator("#property-size")).to_contain_text("EStyle.Medium")
    assert phone.locator(".api-property").evaluate_all("entries => entries.every(e => e.scrollWidth <= e.clientWidth + 1)")
    phone.screenshot(path=str(artifacts / "reference-mobile.png"))
    visit(phone, "components/EFlex.html#属性")
    align = phone.locator("#property-align")
    description = align.locator(".api-property-body").bounding_box()
    values = align.locator(".api-property-values").bounding_box()
    assert values["y"] >= description["y"] + description["height"], "Mobile values must follow the description"
    assert phone.locator(".api-property").evaluate_all("entries => entries.every(e => e.scrollWidth <= e.clientWidth + 1)")
    phone.screenshot(path=str(artifacts / "property-columns-mobile.png"))
    check_width(phone)
    visit(phone, "guide/screenshots.html")
    assert phone.locator('.screenshot-grid img[src$="-zh.png"]').count() >= 20
    assert phone.locator('.screenshot-grid img[src$="-en.png"]').count() == 0
    check_original(phone, phone.locator(".screenshot-grid a").first)
    visit(phone, "en/guide/screenshots.html")
    assert phone.locator('.screenshot-grid img[src$="-en.png"]').count() >= 20
    assert phone.locator('.screenshot-grid img[src$="-zh.png"]').count() == 0
    check_original(phone, phone.locator(".screenshot-grid a").first)
    for width in [320, 768, 1024]:
        phone.set_viewport_size({"width": width, "height": 900})
        for route in ["components/ETypography.html", "en/components/ETypography.html"]:
            visit(phone, route)
            check_pager(phone)
            check_width(phone)
            if width == 320:
                phone.locator(".VPDocFooter .prev-next").screenshot(path=str(artifacts / ("pager-mobile-en.png" if route.startswith("en/") else "pager-mobile-zh.png")))
        visit(phone, "components/EIcon.html#属性")
        check_width(phone)
        visit(phone, "components/EFlex.html#属性")
        check_width(phone)
        visit(phone)
        check_width(phone)
    phone.set_viewport_size({"width": 390, "height": 844})
    visit(phone, "en/")
    expect(phone.get_by_role("heading", level=1)).to_have_text("EUI documentation")
    check_width(phone)
    mobile.close()
    context.close()
    browser.close()

assert not failures, "\n".join(failures)
print("PASS: bundled fonts, production navigation, Chinese/English/method search, code copying, language switching, persistent dark theme, deep links, images, and mobile navigation/layout.")
