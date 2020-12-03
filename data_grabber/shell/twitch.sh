#!/bin/bash
TEXTFILE=$(/pyfiles/twitch.out);
python /textImageGenerators/twitchText.py $TEXTFILE;
rm /dataStorage/twitchInstance.txt;
rm $TEXTFILE