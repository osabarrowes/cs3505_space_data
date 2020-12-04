#!/bin/bash
PICFILE=$(python /pyfiles/lightning.py);
ffmpeg -i $PICFILE -vf scale=475:400 $PICFILE