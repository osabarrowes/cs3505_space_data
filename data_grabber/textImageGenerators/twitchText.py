import os
import sys
import time

inputFile = open(sys.argv[1],'r')
lines = inputFile.readlines()

outpath = '/dataStorage/'
tempTextFilePath = outpath + 'twitchInstance.txt'
tempTextFile = open(tempTextFilePath, 'w')

for line in lines:
    outFileName = str(time.time()).split('.')[0] + ".png"

    ffmpegLine = "yes | ffmpeg -i /textImageGenerators/blankTwitch.png -vf drawtext=\"fontsize=28:fontcolor=white:textfile='" + tempTextFilePath \
    + ":x=(w-text_w)/2:y=(h-text_h + 20)/2:fontfile='Modenine-2OPd.ttf':font='ModeNine'\" -pix_fmt rgba " + outpath + outFileName

    tempTextFile = open(tempTextFilePath, 'w')
    tempTextFile.truncate()
    tempTextFile.write("   Number of Viewers: " + line.split(' ')[0] + "\n")
    tempTextFile.close()
    os.system(ffmpegLine)
    



