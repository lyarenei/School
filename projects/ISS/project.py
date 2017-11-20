#!/usr/bin/env python

# Python 3.6 script for ISS project
# Author: Dominik Křivohlávek <xkrivo02>

import sys
import wave
# pylint: disable-msg=C0103

# Open passed file for analysis
inputFile = wave.open(sys.argv[1], "rb")

# TODO: check for errors

# Fetch input file parameters
inputParams = inputFile.getparams()

# [Task 1] Write down sampling frequency, length in samples and time:
print("Task 1:")
print("\tSampling frequency: ", inputParams.framerate, "Hz")
print("\tNumber of samples: ", inputParams.nframes)
print("\tTime: ", inputParams.framerate / inputParams.nframes, "s")

# Finally, close file
inputFile.close()
