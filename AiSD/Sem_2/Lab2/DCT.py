from PIL import Image
import numpy as np
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
    actual_pixels = pixels[x0 : min(x0 + 8, width), y0 : min(y0 + 8, height)]
    avg_value = np.mean(actual_pixels)
    block.fill(avg_value)
    for x in range(min(8, width - x0)):
        for y in range(min(8, height - y0)):
            block[x, y] = pixels[x0 + x, y0 + y]
    block -= 128
    return C @ block @ C.T
def deDCT(S):
    С+=128
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

def get_scaled_quantization_table(base_table, quality):
    if quality < 1:
        quality = 1
    elif quality > 100:
        quality = 100

    if quality < 50:
        S = 5000 / quality
    else:
        S = 200 - 2 * quality
    scaled_table = np.ceil((base_table * S) / 100.0)
    scaled_table = np.clip(scaled_table, 1, 255)
    return scaled_table.astype(int)

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
    for y_block in range(height // 8):
        for x_block in range(width // 8):
            A1 = deDCT(dequantize(quantize(DCT(x_block, y_block, A),Q_Luminance), Q_Luminance))
            B1 = deDCT(dequantize(quantize(DCT(x_block, y_block, B),Q_Luminance), Q_Luminance))
            C1 = deDCT(dequantize(quantize(DCT(x_block, y_block, C),Q_Luminance), Q_Luminance))
            for y in range(8):
                for x in range(8):
                    res_pixels[x_block * 8 + x, y_block * 8 + y] = (int(A1[y, x]), int(B1[y, x]), int(C1[y, x]))

    return res_img
