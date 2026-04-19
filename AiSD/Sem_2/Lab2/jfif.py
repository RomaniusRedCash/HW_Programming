from PIL import Image
import numpy as np
import struct

def read_channel(filename):
    data=[]
    with open(filename, 'rb') as f:
        raw_data = f.read()
        data = raw_data.replace(b'\xff', b'\xff\x00')
    return data

def write_jfif(filename, size, channel_format, is_color, quality):

    width, height = size
    with open(filename, 'wb') as f:
        f.write(b'\xff\xd8')  # SOI

        f.write(b'\xff\xc0')
        header = struct.pack('>HHHB', width, height, quality, 3 if is_color else 1)
        f.write(header)

        file_list = ["Y", "Cb", "Cr"] if is_color else ["Y"]
        for label in file_list:
            f.write(b'\xff\xda')  # SOS
            f.write(read_channel(f"{label}.{channel_format}"))

        f.write(b'\xff\xd9')  # EOI

def read_jfif(filename, channel_format):
    with open(filename, 'rb') as f:
        content = f.read()
    if not content.startswith(b'\xff\xd8'):
        raise ValueError("Отсутствует SOI)")
    sof_idx = content.find(b'\xff\xc0')
    if sof_idx == -1:
        raise ValueError("SOF0 не найден")
    header_data = content[sof_idx + 2 : sof_idx + 9]
    width, height, quality, channels = struct.unpack('>HHHB', header_data)
    parts = content.split(b'\xff\xda')
    extracted_channels = []
    for i in range(1, len(parts)):
        raw_part = parts[i]
        if b'\xff\xd9' in raw_part:
            raw_part = raw_part.split(b'\xff\xd9')[0]
        clean_data = raw_part.replace(b'\xff\x00', b'\xff')
        extracted_channels.append(clean_data)
    if channels == 3:
        labels = ['Y', 'Cb', 'Cr']
    else:
        labels = ['Y']
    for i in range(len(labels)):
        filename = f'{labels[i]}.{channel_format}'
        with open(filename, 'wb') as f:
            f.write(extracted_channels[i])

    return width, height, channels, quality
