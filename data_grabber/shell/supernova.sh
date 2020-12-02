#!/bin/bash
# I changed supernova.py to output the file name for its text output
# The next line takes that output as input to the script that converts
# the text to images.
python /textImageGenerators/superNovaText.py $(python /pyfiles/supernova.py)