#!/usr/bin/env python

import os

filename = "TempleFileNames.txt"
f = open(filename, "+w")
path = "C:\\Users\\Cavaughn\\Documents\\Visual Studio 2015\\Projects\\Graphics\\Island Project\\GraphicsProject3_ReadModels\\Objects\\Temple"


for i in os.listdir(path):
    str = i.split('.')
    print(str)
    if "obj" in str:
        f.write(i + "\n")


f.close()