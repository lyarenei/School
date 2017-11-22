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


def GetParams(filename):
    "Gets wav file parameters"
    inputFile = wave.open(filename, "rb")
    # TODO: check for open error if needed
    inputParams = inputFile.getparams()
    inputFile.close()
    return inputParams


def PlotGraph(x_axis, y_axis, x_label, y_label, title):
    "Plots and shows graph"
    # TODO: Add labels to graph
    gplot.plot(x_axis, y_axis)
    gplot.show()


def task_1(filename):
    "Takes care of Task 1"
    inputParams = GetParams(filename)
    print("Task 1:")
    print("\tSampling frequency:", inputParams.framerate, "Hz")
    print("\tNumber of samples:", inputParams.nframes)
    print("\tTime: ", inputParams.framerate / inputParams.nframes, "s")


def task_2(filename):
    "Takes care of Task 2"
    inputParams = GetParams(filename)
    sampleRate, inputData = wavfile.read(filename)

    signal_len = inputParams.framerate / inputParams.nframes

    # Get all samples into x_input variable
    x_input = linspace(0.0, inputParams.nframes *
                       signal_len, inputParams.nframes)

    # Evaluate fourier coefficients (and normalize them)
    spectrum_coeff = fftpack.fft(inputData) / inputParams.nframes

    # Plot frequency spectrum
    print("Task 2: \n\tPlotting frequency spectrum...")
    PlotGraph(x_input[range(0, int(inputParams.nframes / 2))],
              abs(spectrum_coeff)[range(0, int(inputParams.nframes / 2))], "frequency [Hz]", "module", "Task 2")


def task_3(filename):
    "Takes care of Task 3"
    inputParams = GetParams(filename)
    sampleRate, inputData = wavfile.read(filename)
    spectrum_coeff = fftpack.fft(inputData) / inputParams.nframes
    print("Task 3: \n\tModule maximum is on:", argmax(spectrum_coeff), "Hz")


def task_4():
    "Takes care of Task 4"


def task_5():
    "Takes care of Task 5"


def task_6(filename, a_iir_coeff, b_iir_coeff):
    "Takes care of Task 6"
    zi = signal.lfiltic(a_iir_coeff, b_iir_coeff, y=[2.])

    inputParams = GetParams(filename)
    sampleRate, inputData = wavfile.read(filename)

    signal_len = inputParams.framerate / inputParams.nframes

    filtered_signal = signal.lfilter(
        a_iir_coeff, b_iir_coeff, inputData, zi=zi)

    filtered_spectrum = fftpack.fft(filtered_signal[0]) / inputParams.nframes

    # Get all samples into x_input variable
    x_input = linspace(0.0, inputParams.nframes *
                       signal_len, inputParams.nframes)

    PlotGraph(x_input[range(0, int(inputParams.nframes / 2))], abs(filtered_spectrum)
              [range(0, int(inputParams.nframes / 2))], "frequency [Hz]", "module", "Task 6")


def task_7(filename, a_iir_coeff, b_iir_coeff):
    "Takes care of Task 7"
    zi = signal.lfiltic(a_iir_coeff, b_iir_coeff, y=[2.])

    inputParams = GetParams(filename)
    sampleRate, inputData = wavfile.read(filename)

    filtered_signal = signal.lfilter(
        a_iir_coeff, b_iir_coeff, inputData, zi=zi)

    filtered_spectrum = fftpack.fft(filtered_signal[0]) / inputParams.nframes
    print("Task 7: \n\tModule maximum in filtered signal is on:",
          argmax(filtered_spectrum), "Hz")


# [Task 1] Write down sampling frequency, length in samples and time:
    task_1(sys.argv[1])

# [Task 2] Evaluate signal spectrum with DFT, plot spectrum module up to F_s / 2 frequency:
    task_2(sys.argv[1])

# [Task 3] Evaluate on which frequency is the maximum module value
    task_3(sys.argv[1])

# [Task 4] Given IIR filter, evaluate and plot poles and zeroes of its transfer function
    task_4()

# [Task 5] Plot frequency module of the IIR filter
    task_5()

# [Task 6] Filter input signal through IIR filter, plot frequency spectrum up to F_s / 2
    a_iir_coeff = array([1.0, 0.2289, 0.4662])
    b_iir_coeff = array([1.0, -0.2324, 0.4112, -0.2324])
    task_6(sys.argv[1], a_iir_coeff, b_iir_coeff)

# [Task 7] Evaluate on which frequency is the maximum module value in filtered signal
    task_7(sys.argv[1], a_iir_coeff, b_iir_coeff)
