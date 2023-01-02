# -*- coding: utf-8 -*-
 
 
import os
from PIL import Image
 
def getAllName(file_dir, tail_list = ['.jpg','.ppm']):
    L=[]
    for root, dirs, files in os.walk(file_dir):
        for file in files:
            if os.path.splitext(file)[1] in tail_list:
                L.append(os.path.join(root, file))
    return L
 
# names = getAllName("") #这里是读取ppm文件的文件夹路径
# save_dir = "" #保存jpg文件的文件夹路径

img = Image.open("./build/image.ppm")
# print(img)
# img.show()
img.save("./image.png")