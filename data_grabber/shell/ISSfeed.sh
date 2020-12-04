#!/bin/bash
PICFILE=$(python /pyfiles/ISSstream.py);
ffmpeg -i $PICFILE -vf scale=475:400 out.png;
rm $PICFILE;
mv out.png $PICFILE