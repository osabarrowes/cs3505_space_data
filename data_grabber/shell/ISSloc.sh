#!/bin/bash
# I changed ISSlatlong.py to output the file name for its text output
# The next line takes that output as input to the script that converts
# the text to images.
TEXTFILE=$(python /pyfiles/ISSlatlong.py);
python /textImageGenerators/LatLon.py $TEXTFILE;
rm $TEXTFILE