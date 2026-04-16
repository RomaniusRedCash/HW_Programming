from PIL import Image
import numpy as np
import threading
import struct
from multiprocessing import Process

from ZigZag import *
from DCT import *
from ColorConvert import *

Q_Luminance = np.array([
        [16, 11, 10, 16, 24, 40, 51, 61],
        [12, 12, 14, 19, 26, 58, 60, 55],
        [14, 13, 16, 24, 40, 57, 69, 56],
        [14, 17, 22, 29, 51, 87, 80, 62],
        [18, 22, 37, 56, 68, 109, 103, 77],
        [24, 35, 55, 64, 81, 104, 113, 92],
        [49, 64, 78, 87, 103, 121, 120, 101],
        [72, 92, 95, 98, 112, 100, 103, 99]
    ])
Q_CLuminance = np.array([
        [17, 18, 24, 47, 99, 99, 99, 99],
        [18, 21, 26, 66, 99, 99, 99, 99],
        [24, 56, 66, 99, 99, 99, 99, 99],
        [47, 66, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99]
    ])

def process_channel(channel_data, q_matrix, filename, size):
    width, height = size
    D = np.array([])
    for y_block in range(height // 8):
        for x_block in range(width // 8):
            D = np.append(D, zigzag(quantize(DCT(x_block, y_block, channel_data),q_matrix)))

    with open(filename, 'wb') as f:
        f.write(struct.pack(f'<{len(D)}i', *D))

    subprocess.run(f"./exe -i {filename} -o {filename}.cmpr --itu")

def compress1(img, quality):
    if img.mode != 'YCbCr':
        img = img_rgb_to_ycbcr(img)
    size = img.size
    width, height = size
    Y,Cb,Cr = img.split()
    Y,Cb,Cr = Y.load(), Cb.load(), Cr.load()

    # p1 = Process(target=process_channel, args=(Y, get_scaled_quantization_table(Q_Luminance, quality), "Y.tmp", size))
    # p2 = Process(target=process_channel, args=(Cb, get_scaled_quantization_table(Q_CLuminance, quality), "Cb.tmp", size))
    # p3 = Process(target=process_channel, args=(Cr, get_scaled_quantization_table(Q_CLuminance, quality), "Cr.tmp", size))
    # p1.start()
    # p2.start()
    # p3.start()
    # p1.join()
    # p2.join()
    # p3.join()



    process_channel(Y, get_scaled_quantization_table(Q_Luminance, quality), "Y.tmp", size)
    process_channel(Cb, get_scaled_quantization_table(Q_CLuminance, quality), "Cb.tmp", size)
    process_channel(Cr, get_scaled_quantization_table(Q_CLuminance, quality), "Cr.tmp", size)

