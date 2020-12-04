#!/bin/bash
PICFILE=$(python /pyfiles/ISSstream.py);
ffmpeg -i $PICFILE -vf scale=640:360 out.png;
rm $PICFILE;
mv out.png $PICFILE