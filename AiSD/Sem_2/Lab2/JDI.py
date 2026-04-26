from PIL import Image
import numpy as np
import threading
import struct
import subprocess
from multiprocessing import Process

from ZigZag import *
from DCT import *
from ColorConvert import *
from jfif import *

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


Q_nLuminance = np.array([
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000],
        [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000]
    ])

def process_channel(channel_data, q_matrix, filename, size, layer):
    width, height = size
    D = np.array([])
    for y_block in range((height + 7) // 8):
        for x_block in range((width + 7) // 8):
            D = np.append(D, zigzag(quantize(DCT(x_block, y_block, channel_data),q_matrix)))

    int_data = [int(round(x)) for x in D]

    if not int_data:
        print(f"Предупреждение: Массив для {filename} пуст!")
    else:
        with open(filename, 'wb') as f:
            binary_data = struct.pack(f'{len(int_data)}h', *int_data)
            f.write(binary_data)

    subprocess.run(["./exe", f"--itu={layer}", "-i", filename, "-o", f"{filename}.cmpr", "--log-file"], check=True)



def deprocess_channel(q_matrix, filename, size, layer):
    width, height = size
    width = (width + 7) // 8 * 8
    height = (height + 7) // 8 * 8
    subprocess.run(["./exe", f"--de-itu={layer}", "-o", filename, "-i", f"{filename}.cmpr", "--log-file"], check=True)
    data = np.fromfile(filename, dtype=np.int16)
    channel_data = np.zeros((height, width))
    block_idx = 0
    num_blocks_x = width // 8
    num_blocks_y = height // 8
    for y_block in range(num_blocks_y):
        for x_block in range(num_blocks_x):
            channel_data[y_block*8 : (y_block+1)*8, x_block*8 : (x_block+1)*8] = deDCT(dequantize(deZigzag(data[block_idx * 64 : (block_idx + 1) * 64], 8, 8), q_matrix))
            block_idx += 1

    return channel_data.astype(np.uint8)


def test_pipeline_full(channel_data, q_matrix, size):
    channel_data = np.array(channel_data)
    width, height = size
    num_blocks_x = (width + 7) // 8
    num_blocks_y = (height + 7) // 8
    reconstructed_channel = np.zeros((num_blocks_y * 8, num_blocks_x * 8), dtype=np.float64)
    for y_block in range(num_blocks_y):
        for x_block in range(num_blocks_x):
            # block = channel_data[y_block*8 : (y_block+1)*8, x_block*8 : (x_block+1)*8]
            dct_coeffs = DCT(x_block, y_block, channel_data)
            q_coeffs = quantize(dct_coeffs, q_matrix)
            zz_vector = zigzag(q_coeffs)
            zz_vector = zz_vector.astype(np.int16)
            # --- ОБРАТНЫЙ ХОД ---
            rd_q_coeffs = deZigzag(zz_vector, 8, 8)
            rd_dct_coeffs = dequantize(rd_q_coeffs, q_matrix)
            rd_block = deDCT(rd_dct_coeffs)
            reconstructed_channel[y_block*8 : (y_block+1)*8, x_block*8 : (x_block+1)*8] = rd_block
    return np.clip(reconstructed_channel, 0, 255).astype(np.uint8)



def compress1(img, quality, file_name):
    if img.mode == 'RGB':
        img = img_rgb_to_ycbcr(img)
        is_color = True
    elif img.mode == 'L':
        is_color = False
    elif img.mode == '1':
        img = img.convert(mode = 'L')
        is_color = False
    size = img.size
    width, height = size
    if is_color:
        Y,Cb,Cr = img.split()
        process_channel(Y, get_scaled_quantization_table(Q_Luminance, quality), "Y.tmp", size, "nC")
        process_channel(Cb, get_scaled_quantization_table(Q_CLuminance, quality), "Cb.tmp", size, 'C')
        process_channel(Cr, get_scaled_quantization_table(Q_CLuminance, quality), "Cr.tmp", size, 'C')
    else:
        Y = img.split()[0]
        process_channel(Y, get_scaled_quantization_table(Q_Luminance, quality), "Y.tmp", size, "nC")

    write_jfif(file_name, size, "tmp.cmpr", is_color, quality)



def decompress1(file_name):
    width, height, channels, quality = read_jfif(file_name, "tmp.cmpr")
    size = [width, height]
    if channels == 3:
        Y = deprocess_channel(get_scaled_quantization_table(Q_Luminance, quality), "Y.tmp", size, "nC")
        Cb = deprocess_channel(get_scaled_quantization_table(Q_CLuminance, quality), "Cb.tmp", size, 'C')
        Cr = deprocess_channel(get_scaled_quantization_table(Q_CLuminance, quality), "Cr.tmp", size, 'C')
        final_img = Image.fromarray(np.stack((Y, Cb, Cr), axis=-1), mode='YCbCr')
    else:
        ChB = deprocess_channel(get_scaled_quantization_table(Q_CLuminance, quality), "ChB.tmp", size, 'C')
        final_img = Image.fromarray(np.stack((ChB), axis=-1), mode='1')
    return final_img.crop((0, 0, width, height))

def OnlyTestColor(img, quality):
    is_color = True
    if img.mode == 'RGB':
        img = img_rgb_to_ycbcr(img)
    elif img.mode == 'L':
        is_color = False
    elif img.mode == '1':
        img = img.convert(mode = 'L')
        is_color = False
    size = img.size
    width, height = size
    if is_color:
        Y,Cb,Cr = img.split()
        Y = test_pipeline_full(Y, get_scaled_quantization_table(Q_Luminance, quality), size)
        Cb = test_pipeline_full(Cb, get_scaled_quantization_table(Q_CLuminance, quality), size)
        Cr = test_pipeline_full(Cr, get_scaled_quantization_table(Q_CLuminance, quality), size)

        # Y = test_pipeline_full(Y, Q_nLuminance, size)
        # Cb = test_pipeline_full(Cb, Q_nLuminance, size)
        # Cr = test_pipeline_full(Cr, Q_nLuminance, size)


        final_img = Image.fromarray(np.stack((Y, Cb, Cr), axis=-1), mode='YCbCr')
    else:
        size = height, width
        Y = np.array(img.split()[0]).T
        Y = test_pipeline_full(Y, get_scaled_quantization_table(Q_Luminance, quality), size)
        final_img = Image.fromarray(np.stack((Y), axis=-1), img.mode)
    return final_img.crop((0, 0, width, height))

