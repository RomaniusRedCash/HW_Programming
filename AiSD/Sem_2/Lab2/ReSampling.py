from PIL import Image
import numpy as np
from Interpolation import *
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
# resize ##############################################################
def resize_bilinear(img, new_size):
    new_w, new_h = new_size
    old_w, old_h = img.size
    scale_x = old_w / new_w
    scale_y = old_h / new_h
    newImg = Image.new(img.mode, (new_w, new_h))
    data = []
    pixels = np.array(img)
    for x in range(new_w):
        for y in range(new_h):
            data.append(biLineInterp(pixels, y * scale_y, x * scale_x))
    newImg.putdata(data)

    return newImg
