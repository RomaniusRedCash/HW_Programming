from PIL import Image
import numpy as np
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
    tx, ty = x - x0, y - y0
    wx = np.array([1.0 - tx, tx])
    wy = np.array([1.0 - ty, ty])
    tmp = wy @ Q
    res = wx @ tmp
    return tuple(res.astype(int))
