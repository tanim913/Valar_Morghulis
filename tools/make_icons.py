"""Renders the pictures that are made from SVG.

    pip install cairosvg pillow
    python3 tools/make_icons.py

  tools/icons/{heart,star,skull}.svg  ->  assets/images/ui_<name>.png      (in-game HUD)
  tools/icons/app.svg                 ->  packaging/linux/icon-256.png      (Ubuntu app menu)
                                          packaging/windows/game.ico        (Windows .exe and shortcuts)
"""
import glob
import os
import cairosvg
from PIL import Image

here = os.path.dirname(os.path.abspath(__file__))
root = os.path.join(here, "..")

for svg in sorted(glob.glob(os.path.join(here, "icons", "*.svg"))):
    name = os.path.splitext(os.path.basename(svg))[0]
    if name == "app":
        continue
    out = os.path.join(root, "assets", "images", "ui_%s.png" % name)
    cairosvg.svg2png(url=svg, write_to=out, output_width=128, output_height=128)
    print(out)

app = os.path.join(here, "icons", "app.svg")
png = os.path.join(root, "packaging", "linux", "icon-256.png")
cairosvg.svg2png(url=app, write_to=png, output_width=256, output_height=256)
print(png)

# One PNG per size, so small icons are drawn sharp instead of shrunk afterwards.
sizes = [16, 24, 32, 48, 64, 128, 256]
frames = []
for s in sizes:
    p = os.path.join(here, "_ico_%d.png" % s)
    cairosvg.svg2png(url=app, write_to=p, output_width=s, output_height=s)
    frames.append(Image.open(p).convert("RGBA"))
ico = os.path.join(root, "packaging", "windows", "game.ico")
frames[-1].save(ico, format="ICO", sizes=[(s, s) for s in sizes], append_images=frames[:-1])
for s in sizes:
    os.remove(os.path.join(here, "_ico_%d.png" % s))
print(ico)
