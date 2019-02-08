#! /usr/bin/python
# Read in a WAV and find the freq's
import wave
import numpy as np
import sys

chunk = 2048
chunkNum = 0

print "DEBUG: Chunk size is %d" % (chunk)

#  AAN-2019-02-01.wav
#  AAN.beep-old1.wav
#  AAN.beep.wav		-- sample width is 2 bytes
#  AAN.LONGbeep.wav	- 375 Hz CQ "K" (dash-dot-dash) 44.1 KHz sample rate, STEREO file
##   wf = wave.open('AAN-2019-02-01.wav', 'rb')

#
#  news.beep-old1.wav	-- 440 Hz tones, 16,000 Hz sample rate, MONO file
#  news.beep.wav	-- 440 Hz tones, 16,000 Hz sample rate, MONO file
#  news.LONGbeep.wav	-- 440 Hz tones, 16,000 Hz sample rate, MONO file
#  news.wav		-- 440 Hz tones, 16,000 Hz sample rate, MONO file
##   wf = wave.open('news.wav', 'rb')

# open up a wavefile
wf = wave.open(sys.argv[1], 'rb')

swidth = wf.getsampwidth()
RATE = wf.getframerate()
CH = wf.getnchannels()
print "DEBUG: Sample Width is %d Rate is %d Hz Num Channels is %d" % (swidth,RATE,CH)
# use a Blackman window
window = np.blackman(chunk)

# read some data
data = wf.readframes(chunk)
print "DEBUG: len(data) is %d" % (len(data))
if (CH == 2):
    data2 = data[0:4096:1]
    data = data2
# find the frequency of each chunk
while len(data) == chunk*swidth:
    chunkNum = chunkNum + 1
    # unpack the data and times by the hamming window
    indata = np.array(wave.struct.unpack("%dh"%(len(data)/swidth),\
                                         data))*window
    # Take the fft and square each value
    fftData=abs(np.fft.rfft(indata))**2
    # find the maximum
    which = fftData[1:].argmax() + 1
    # use quadratic interpolation around the max
    if which != len(fftData)-1:
        y0,y1,y2 = np.log(fftData[which-1:which+2:])
        x1 = (y2 - y0) * .5 / (2 * y1 - y2 - y0)
        # find the frequency and output it
        thefreq = (which+x1)*RATE/chunk
        print "Chunk %03d freq is %f Hz, Sample Rate is %d Hz" % (chunkNum, thefreq, RATE)
    else:
        thefreq = which*RATE/chunk
        print "Chunk %03d freq is %f Hz, Sample Rate is %d Hz" % (chunkNum, thefreq, RATE)
    # read some more data
    data = wf.readframes(chunk)
    if (CH == 2):
        data2 = data[0:4096:1]
        data = data2


