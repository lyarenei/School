#!/usr/bin/env python

# Python 3.6 script for ISS project
# Author: Dominik Křivohlávek <xkrivo02>

import sys
import wave
# pylint: disable-msg=C0103

# Open passed file for analysis
inputFile = wave.open(sys.argv[1], "rb")

# TODO: check for errors

# Finally, close file
inputFile.close()
