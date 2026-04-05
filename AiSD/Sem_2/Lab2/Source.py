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
##############################################################
# Downsampling ##############################################################
def down2(img):
    width, height = img.size
    if width % 2 != 0 or height % 2 != 0:
        print("Размер должен быть кратен 2")
        return None
    new_size = (width // 2, height // 2)
    newImg   = Image.new(img.mode, new_size)
    data = []
    pixels = img.load()
    for y in range(0, height, 2):
        for x in range(0, width, 2):
            p1 = pixels[x, y]
            p2 = pixels[x+1, y]
            p3 = pixels[x, y+1]
            p4 = pixels[x+1, y+1]
            avg = tuple(
                int((p1[i] + p2[i] + p3[i] + p4[i]) / 4)
                for i in range(len(p1))
            )
            data.append(avg)
    newImg.putdata(data)
    return newImg
# Upsampling ##############################################################
def up2(img):
    width, height = img.size
    new_size = (width * 2, height * 2)
    newImg   = Image.new(img.mode, new_size)
    data = []
    pixels = img.load()
    for y in range(height):
        row=[]
        for x in range(width):
            row.append(pixels[x, y])
            row.append(pixels[x, y])
        data.extend(row)
        data.extend(row)
    newImg.putdata(data)
    return newImg
def smartUp2(img):
    pixels = np.array(img)
    height, width, channels = pixels.shape
    new_size = (width * 2, height * 2)
    newImg = Image.new(img.mode, new_size)
    data = []
    for y in range(new_size[0]):
        for x in range(new_size[1]):
            pixel = biLineInterp(pixels, x/2, y/2)
            data.append(pixel)
    newImg.putdata(data)
    return newImg
##############################################################
# line interp ##############################################################
def lineInterp(pixel1,pixel2, t):
    return tuple(int(pixel1[i] * (1 - t) + pixel2[i] * t) for i in range(len(pixel1)))
def splineLineInterp(pixels, y, x):
    if x < 0:
        return pixels[0, y]
    elif x >= pixels.shape[0] - 1:
        return pixels[-1, y]
    x_tmp = int(np.floor(x))
    return lineInterp(pixels[x_tmp, y], pixels[x_tmp + 1, x%1])
def biLineInterp(pixels, x, y):
    x0 = int(np.floor(x))
    y0 = int(np.floor(y))
    x1 = min(x0 + 1, pixels.shape[0] - 1)
    y1 = min(y0 + 1, pixels.shape[1] - 1)
    Q = np.array([
        [pixels[y0, x0], pixels[y1, x0]],
        [pixels[y0, x1], pixels[y1, x1]]
    ])
    tx, ty = x%1, y%1
    wx = np.array([1.0 - tx, tx])
    wy = np.array([1.0 - ty, ty])
    tmp = wy @ Q
    res = wx @ tmp
    return tuple(res.astype(int))
##############################################################
# DCT ##############################################################
def Cuv(x):
    if x == 0:
        return 1/np.sqrt(2)
    else: return 1
def simpleDCT(startXPos, startYPos, pixels):
    Acos = np.zeros((8,8))
    for u in range(8):
        for x in range(8):
            Acos[u,x]=np.cos((2*x + 1) * u * np.pi / 16)
    x0=startXPos * 8
    y0=startYPos * 8
    block = np.zeros((8, 8))
    for y in range(8):
        for x in range(8):
            block[y, x] = pixels[x0 + x, y0 + y]

    S = np.zeros((8,8))
    for v in range(8):
        for u in range(8):
            sum_val = 0
            for y in range(8):
                for x in range(8):
                    sum_val += block[y, x] * Acos[u,x] * Acos[v,y]
            S[v, u] = 0.25 * Cuv(u) * Cuv(v) * sum_val
    return S
def simpleDeDCT(S):
    Acos = np.zeros((8,8))
    for u in range(8):
        for x in range(8):
            Acos[u,x]=np.cos((2*x + 1) * u * np.pi / 16)
    s = np.zeros((8,8))
    for y in range(8):
        for x in range(8):
            sum_val = 0
            for v in range(8):
                for u in range(8):
                    sum_val += S[v, u] * Acos[u,x] * Acos[v,y] * Cuv(u) * Cuv(v)
            s[y, x] = 0.25 * sum_val
    return np.clip(np.round(s), 0, 255).astype(np.uint8)

def simpleDCT_Generic(pixels, startXPos, startYPos, N, M):
    x0, y0 = startXPos * M, startYPos * N
    Acos_u = np.zeros((M, M))
    for u in range(M):
        for x in range(M):
            Acos_u[u, x] = np.cos((2 * x + 1) * u * np.pi / (2 * M))
    Acos_v = np.zeros((N, N))
    for v in range(N):
        for y in range(N):
            Acos_v[v, y] = np.cos((2 * y + 1) * v * np.pi / (2 * N))
    block = np.zeros((N, M))
    for y in range(N):
        for x in range(M):
            block[y, x] = pixels[x0 + x, y0 + y]
    S = np.zeros((N, M))
    for v in range(N):
        for u in range(M):
            sum_val = 0
            for y in range(N):
                for x in range(M):
                    sum_val += block[y, x] * Acos_u[u, x] * Acos_v[v, y]
            S[v, u] = Cuv_scaled(u, M) * Cuv_scaled(v, N) * sum_val
    return S
def simpleDeDCT_Generic(S, N, M):
    Acos_u = np.zeros((M, M))
    for u in range(M):
        for x in range(M):
            Acos_u[u, x] = np.cos((2 * x + 1) * u * np.pi / (2 * M))
    Acos_v = np.zeros((N, N))
    for v in range(N):
        for y in range(N):
            Acos_v[v, y] = np.cos((2 * y + 1) * v * np.pi / (2 * N))
    s_reconstructed = np.zeros((N, M))
    for y in range(N):
        for x in range(M):
            sum_val = 0
            for v in range(N):
                for u in range(M):
                    sum_val += Cuv_scaled(u, M) * Cuv_scaled(v, N) * S[v, u] * Acos_u[u, x] * Acos_v[v, y]
            s_reconstructed[y, x] = sum_val
    return np.clip(np.round(s_reconstructed), 0, 255).astype(np.uint8)

def DCT(startXPos, startYPos, pixels):
    C = np.zeros((8, 8))
    for i in range(8):
        C[0,i]=1/np.sqrt(8)
    for i in range(1, 8):
        for j in range(8):
            C[i, j] = np.sqrt(0.25) * np.cos((2 * j + 1) * i * np.pi / (16))
    x0=startXPos * 8
    y0=startYPos * 8
    block = np.zeros((8, 8))
    for y in range(8):
        for x in range(8):
            block[y, x] = pixels[x0 + x, y0 + y]
    return C @ block @ C.T
def deDCT(S):
    C = np.zeros((8, 8))
    for i in range(8):
        C[0,i]=1/np.sqrt(8)
    for i in range(1, 8):
        for j in range(8):
            C[i, j] = np.sqrt(0.25) * np.cos((2 * j + 1) * i * np.pi / (16))
    return C.T @ S @ C

def quantize(dct_coefficients, q_table):
    return np.round(dct_coefficients / q_table).astype(np.int32)
def dequantize(quantized_coefficients, q_table):
    return (quantized_coefficients * q_table).astype(float)
##############################################################

Lenna = Image.open('250px-Lenna.png')
# LennaChB = Lenna.convert(mode = "1", dither=Image.Dither.NONE)
# LennaChBD = Lenna.convert(mode = "1")
# smartUp2(down2(Lenna)).show()
# up2(down2(Lenna)).show()


def test_dct_pipeline(img):
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
    width, height = img.size
    width = (width // 8) * 8
    height = (height // 8) * 8
    img = img.crop((0, 0, width, height))
    A,B,C = img.split()
    A, B, C = A.load(), B.load(), C.load()
    res_img = Image.new(img.mode, (width, height))
    res_pixels = res_img.load()
    print(f"Обработка изображения {width}x{height}...")
    for y_block in range(height // 8):
        for x_block in range(width // 8):
            A1 = deDCT(dequantize(quantize(DCT(x_block, y_block, A),Q_Luminance), Q_Luminance))
            B1 = deDCT(dequantize(quantize(DCT(x_block, y_block, B),Q_Luminance), Q_Luminance))
            C1 = deDCT(dequantize(quantize(DCT(x_block, y_block, C),Q_Luminance), Q_Luminance))
            for y in range(8):
                for x in range(8):
                    res_pixels[x_block * 8 + x, y_block * 8 + y] = (int(A1[y, x]), int(B1[y, x]), int(C1[y, x]))

        if y_block % 10 == 0:
            print(f"Готово строк блоков: {y_block}")
    print(f"Готово!")
    return res_img

test_dct_pipeline(Lenna).show()




