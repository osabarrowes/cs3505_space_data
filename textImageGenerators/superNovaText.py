#!/usr/bin/python3
import os
import datetime

'''
USAGE NOTES:
this will take in a text file as a command line argument
the text must have the format 2020/11/23.603 18.1
This script will output a png image file with RGBA pixel 
format for each line in the file with a UNIX time for a name.
The UNIX time is equivalent to the 
The output destination can be adjusted by changing the outpath variable below.

IMPORTANT PREREQUISITES:
ffmpeg must be compiled with --enable-libfontconfig and --enable-libfreetype

the following files must be present in the directory where the python script is executed:
    Modenine-2OPd.ttf   (the font we use to draw the text)
    isspos.png  (black box that is 640x36)
'''


inputFile = open('supernova.txt','r')
lines = inputFile.readlines()

outpath = 'efs/'
tempTextFilePath = outpath + 'novaInstance.txt'
tempTextFile = open(tempTextFilePath, 'w')

for line in lines:
    
    novaTime = datetime.datetime.strptime(line.split('.')[0],"%Y/%m/%d")
    decimalDay = line.split('.')[1].split(' ')[0]
    novaTime = novaTime.timestamp() + float(decimalDay)/(len(decimalDay) * 10) * 86400

    outFileName = str(int(novaTime)) + ".png"

    ffmpegLine = "yes | ffmpeg -i blankTwitch.png -vf drawtext=\"fontsize=28:fontcolor=white:textfile='" + tempTextFilePath \
    + ":x=(w-text_w)/2:y=(h-text_h + 20)/2:fontfile='Modenine-2OPd.ttf':font='ModeNine'\" -pix_fmt rgba " + outpath + outFileName

    tempTextFile = open(tempTextFilePath, 'w')
    tempTextFile.truncate()
    tempTextFile.write("   Last Supernova\nTime: " + line.split(' ')[0] + "\n   Magnitude: " + line.split(' ')[1])
    tempTextFile.close()
    os.system(ffmpegLine)
    



