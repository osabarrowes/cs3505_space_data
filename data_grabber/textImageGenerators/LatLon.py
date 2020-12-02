#!/usr/bin/python3
import os
import sys
import time

'''
USAGE NOTES:
this will take in a text file as a command line argument
the text must have the format 51,7 S 87,1 E
This script will output a png image file with RGBA pixel 
format with the current UNIX time for a name. 
The output destination can be adjusted by changing the outpath variable below.

IMPORTANT PREREQUISITES:
ffmpeg must be compiled with --enable-libfontconfig and --enable-libfreetype

the following files must be present in the directory where the python script is executed:
    Modenine-2OPd.ttf   (the font we use to draw the text)
    isspos.png  (black box that is 640x36)
'''

inputFile = open(sys.argv[1],'r')
line = inputFile.readline().strip()

outpath = 'efs/latlon/'

#outFileName = sys.argv[1].split('.tx')[0] + ".png"
#outFileName = outFileName.replace("/", "-")
#outFileName = outFileName.replace(" ", "-")

outFileName = str(time.time()).split('.')[0] + ".png"

ffmpegLine = "yes | ffmpeg -i isspos.png -vf drawtext=\"fontsize=24:fontcolor=white:text='Latitude\: " +  ' '.join(line.split(' ')[0:1]) + " Longitude\: " + ' '.join(line.split(' ')[2:3]) \
+ "':x=(w-text_w)/2:y=(h-text_h + 2)/2:fontfile='Modenine-2OPd.ttf':font='ModeNine'\" -pix_fmt rgba " + outpath + outFileName

os.system(ffmpegLine)
