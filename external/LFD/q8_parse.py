import os, sys
import numpy as np
import struct
# filename = "./q8_table"
q8_file = open('q8_table')
q8_data = q8_file.read()

s = struct.Struct()