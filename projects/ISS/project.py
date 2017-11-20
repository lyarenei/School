#!/usr/bin/env python

# Python 3.6 script for ISS project
# Author: Dominik Křivohlávek <xkrivo02>

import sys
import wave
import matplotlib.pyplot as gplot
from numpy import linspace
from scipy import fftpack
from scipy.io import wavfile

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

# [Task 2] Evaluate signal spectrum with DFT, plot spectrum module up to F_s / 2 frequency:

# Read input data
sampleRate, inputData = wavfile.read(sys.argv[1])

samples = inputParams.nframes
length = inputParams.framerate / inputParams.nframes

# Get all samples into x_input variable
x_input = linspace(0.0, samples*length, samples)

# Evaluate fourier coefficients (and normalize them)
dft_coefficients = fftpack.fft(inputData) / samples

# Plot frequency spectrum
print("Task 2: Plotting frequency spectrum...")
# TODO: Add labels to graph
gplot.plot(x_input[range(0, int(samples / 2))], abs(dft_coefficients)[range(0, int(samples / 2))])
gplot.show()

# Finally, close file
inputFile.close()
