"""Trim native captures and fill the matching preview background."""

import argparse
from pathlib import Path

from PIL import Image


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("directory", type=Path)
args = parser.parse_args()
files = sorted(args.directory.glob("*.png"))
if not files:
    parser.error("No PNG captures found")

for file in files:
    with Image.open(file) as source:
        image = source.convert("RGBA")
    bounds = image.getchannel("A").getbbox()
    if not bounds:
        raise ValueError(f"Empty native preview: {file}")
    image = image.crop(bounds)
    background = Image.new("RGBA", image.size, "#25282f" if "-dark-" in file.name else "#f8f9fc")
    background.alpha_composite(image)
    background.convert("RGB").save(file, optimize=True)

print(f"Prepared {len(files)} native previews")
