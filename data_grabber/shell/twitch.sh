#!/bin/bash
TEXTFILE=$(./pyfiles/twitch.out);
python /textImageGenerators/twitchText.py $TEXTFILE;
rm /dataSource/twitchInstance.txt;
rm $TEXTFILE