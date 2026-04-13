from PIL import Image
import numpy as np
from DCT import *
from ColorConvert import *
from ReSampling import *
from Interpolation import *
from RAWer import *

##############################################################

Lenna = Image.open('250px-Lenna.png')
# LennaChB = Lenna.convert(mode = "1", dither=Image.Dither.NONE)
# LennaChBD = Lenna.convert(mode = "1")
# smartUp2(down2(Lenna)).show()
# up2(down2(Lenna)).show()
# test_dct_pipeline(Lenna).show()

IMGsome = resize_bilinear(Lenna, (5, 5))
IMGsome.show()
resize_bilinear(IMGsome, (512, 512)).show()



