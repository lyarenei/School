#!/usr/bin/env python

# Python 3.6 script for ISS project
# Author: Dominik Křivohlávek <xkrivo02>

import sys
import wave
import matplotlib.pyplot as gplot
from numpy import linspace, argmax, array, ndarray
from scipy import fftpack
from scipy import signal as signal
from scipy.io import wavfile

# pylint: disable-msg=C0103

# Open passed file for analysis
inputFile = wave.open(sys.argv[1], "rb")

# TODO: check for errors

# Fetch input file parameters
inputParams = inputFile.getparams()


# [Task 1] Write down sampling frequency, length in samples and time:
print("Task 1:")
print("\tSampling frequency:", inputParams.framerate, "Hz")
print("\tNumber of samples:", inputParams.nframes)
print("\tTime: ", inputParams.framerate / inputParams.nframes, "s")


# [Task 2] Evaluate signal spectrum with DFT, plot spectrum module up to F_s / 2 frequency:

# Read input data
sampleRate, inputData = wavfile.read(sys.argv[1])

samples = inputParams.nframes
length = inputParams.framerate / inputParams.nframes

# Get all samples into x_input variable
x_input = linspace(0.0, samples * length, samples)

# Evaluate fourier coefficients (and normalize them)
dft_coefficients = fftpack.fft(inputData) / samples

# Plot frequency spectrum
print("Task 2: \n\tPlotting frequency spectrum...")
# TODO: Add labels to graph
gplot.plot(x_input[range(0, int(samples / 2))],
           abs(dft_coefficients)[range(0, int(samples / 2))])
gplot.show()


# [Task 3] Evaluate on which frequency is the maximum module value
print("Task 3: \n\tModule maximum is on:", argmax(dft_coefficients), "Hz")


# [Task 4] Given IIR filter, evaluate and plot poles and zeroes of its transfer function


# [Task 5] Plot frequency module of the IIR filter


# [Task 6] Filter input signal through IIR filter, plot frequency spectrum up to F_s / 2
a_iir_coeff = array([1.0, 0.2289, 0.4662])
b_iir_coeff = array([1.0, -0.2324, 0.4112, -0.2324])

zi = signal.lfiltic(a_iir_coeff, b_iir_coeff, y=[2.])

filtered_signal = signal.lfilter(a_iir_coeff, b_iir_coeff, inputData, zi=zi)

filtered_spectrum = fftpack.fft(filtered_signal[0]) / samples

gplot.plot(x_input[range(0, int(samples / 2))],
           abs(filtered_spectrum)[range(0, int(samples / 2))])
gplot.show()


# [Task 7] Evaluate on which frequency is the maximum module value in filtered signal
print("Task 7: \n\tModule maximum in filtered signal is on:",
      argmax(filtered_spectrum), "Hz")


# Finally, close file
inputFile.close()
