"""Rebuild the committed web fonts from the upstream variable TTF files."""

import argparse
from pathlib import Path

from fontTools import subset
from fontTools.ttLib import TTFont
from fontTools.varLib.instancer import instantiateVariableFont


website = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--sans", type=Path, required=True)
parser.add_argument("--mono", type=Path, required=True)
args = parser.parse_args()

pages = list((website / ".generated").rglob("*.md"))
if not pages:
    parser.error("Run npm run generate before subsetting fonts")

theme = [website / ".vitepress" / "config.mjs"]
theme += [file for file in (website / ".vitepress" / "theme").rglob("*") if file.suffix in {".vue", ".css", ".js"}]
characters = set(range(0x20, 0x100))
for file in pages + theme:
    characters.update(map(ord, file.read_text(encoding="utf-8")))

fonts = website / ".vitepress" / "theme" / "fonts"
for source, name, license_file in [(args.sans, "noto-sans-sc", "NotoSansSC-OFL.txt"), (args.mono, "jetbrains-mono", "JetBrainsMono-OFL.txt")]:
    font = TTFont(source)
    font["name"].setName((fonts / license_file).read_text(encoding="utf-8"), 13, 3, 1, 0x409)
    options = subset.Options()
    options.name_IDs += [0, 13, 14]
    subsetter = subset.Subsetter(options=options)
    subsetter.populate(unicodes=characters)
    subsetter.subset(font)
    instantiateVariableFont(font, {"wght": (400, 700)}, inplace=True)
    font.flavor = "woff2"
    target = fonts / f"{name}.woff2"
    target.parent.mkdir(parents=True, exist_ok=True)
    font.save(target)
    print(f"{target.name}: {target.stat().st_size:,} bytes, {len(font.getBestCmap())} characters")
