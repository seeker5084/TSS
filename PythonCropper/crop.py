import sys
import pyvips
from os import path
from glob import glob

threshold = 10
croppedName = " (cropped)"

for filename in sys.argv[1:]:

    print(filename)

    im = pyvips.Image.new_from_file(filename)

    background = im(0, 0)
    mask = (im.median(3) - background).abs() > threshold
    columns, rows = mask.project()

    left = columns.profile()[1].min()
    right = columns.width - columns.fliphor().profile()[1].min()
    top = rows.profile()[0].min()
    bottom = rows.height - rows.flipver().profile()[0].min()

    im = im.crop(left, top, right - left, bottom - top)

    filename = path.splitext(filename)[0] + croppedName + path.splitext(filename)[1]
    im.write_to_file(filename)
