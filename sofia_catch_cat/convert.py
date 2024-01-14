import numpy as np
import cv2
import argparse

parser = argparse.ArgumentParser(description='Convert pic to special color pixels')
parser.add_argument('filename')
parser.add_argument('-wd', '--width', type=int, help='reduced to width')
parser.add_argument('-ht', '--height', type=int, help='reduced to height')

args = parser.parse_args()

name = args.filename.split(".", 1)[0]

img=cv2.imread(args.filename)

img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

res=cv2.resize(img, dsize=(args.width, args.height), interpolation=cv2.INTER_CUBIC)

cv2.imwrite(name + "_reduce_" + str(args.width) + "_" + str(args.height) + ".jpg", res)

new_pic = []

for line in res:
    for pixel in line:
        R = pixel[0]
        G = pixel[1]
        B = pixel[2]
        R_5b = round(R / 255 * 31)
        G_6b = round(G / 255 * 63)
        B_5b = round(B / 255 * 31)
        new_pixel = ((R_5b & 0x1f) << 11) | ((G_6b & 0x3f) << 5) | (B_5b & 0x1f)  # 5bit R + 6bit G + 5bit B_5b'
        new_pic.append(new_pixel)


f = open(name + "array.txt", "w")
f.write("static const uint16_t PROGMEM " + name + "[] = {\n")
for i in new_pic:
    f.write(hex(i) + ",\n")

f.write("};\n")
f.close()
        
