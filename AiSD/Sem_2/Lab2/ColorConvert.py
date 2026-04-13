from PIL import Image
import numpy as np
# В YCbCr ##############################################################
def rgb_to_ycbcr(pixl):
    pixl = np.array(pixl)
    matrix = np.array([
        [ 0.299,     0.587,     0.114],
        [-0.168736, -0.331264,  0.5],
        [ 0.5,      -0.418688, -0.081312]
    ])
    offset = np.array([0, 128, 128])
    ycbcr = matrix @ pixl + offset
    return np.clip(ycbcr, 0, 255).astype(np.uint8)
def img_rgb_to_ycbcr(img):
    if img.mode != 'RGB':
        img.convert('RGB')
    new_data = [tuple(rgb_to_ycbcr(p)) for p in img.getdata()]
    out = Image.new('YCbCr', img.size)
    out.putdata(new_data)
    return out
# Обратно
def ycbcr_to_rgb(pixl):
    ycbcr = np.array(pixl, dtype=np.float32)
    matrix = np.array([
        [ 0.299,     0.587,     0.114],
        [-0.168736, -0.331264,  0.5],
        [ 0.5,      -0.418688, -0.081312]
    ])
    offset = np.array([0, 128, 128])
    inv_matrix = np.linalg.inv(matrix)
    rgb = inv_matrix @ (ycbcr - offset)
    return np.clip(rgb, 0, 255).astype(np.uint8)
def img_ycbcr_to_rgb(img):
    if img.mode != 'YCbCr':
        img = img.convert('YCbCr')
    new_data = [tuple(ycbcr_to_rgb(p)) for p in img.getdata()]
    out = Image.new('RGB', img.size)
    out.putdata(new_data)
    return out
