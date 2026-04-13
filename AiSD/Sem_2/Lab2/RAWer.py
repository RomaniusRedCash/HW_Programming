from PIL import Image
import numpy as np
##############################################################
# raw ##############################################################
import struct
import os
def convert_to_myraw(input_path, output_path, mode='RGB'):
    img = Image.open(input_path).convert(mode)
    width, height = img.size
    mode_code = {'1': 1, 'L': 2, 'RGB': 3}.get(mode, 3)
    header = struct.pack('>BII', mode_code, width, height)
    raw_data = img.tobytes()
    with open(output_path, 'wb') as f:
        f.write(header)
        f.write(raw_data)
    return
def convert_from_myraw(input_path):
    f = open(input_path, 'rb')
    header_data = f.read(9)
    if not header_data:
        return None
    mode_code, width, height = struct.unpack('>BII', header_data)
    modes = {1: '1', 2: 'L', 3: 'RGB'}
    mode = modes.get(mode_code, 'RGB')
    raw_pixels = f.read()
    img = Image.frombytes(mode, (width, height), raw_pixels)
    return img
