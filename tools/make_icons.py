"""Renders tools/icons/*.svg to assets/images/ui_<name>.png (128x128).

    pip install cairosvg
    python3 tools/make_icons.py
"""
import glob
import os
import cairosvg

here = os.path.dirname(__file__)
for svg in sorted(glob.glob(os.path.join(here, "icons", "*.svg"))):
    name = os.path.splitext(os.path.basename(svg))[0]
    out = os.path.join(here, "..", "assets", "images", "ui_%s.png" % name)
    cairosvg.svg2png(url=svg, write_to=out, output_width=128, output_height=128)
    print(out)
