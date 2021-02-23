"""
Function: findcar
Input: x,y coordinates of the found blobs
Output: x,y coordinates of the middle of the car and direction they are facing
"""
import numpy as np
import struct
import sys
import bitstruct


def findcar(keypoints,width,height):
    x = 0
    y = 0
    xrel = 0
    yrel = 0
    for i in range(0,len(keypoints)):
        x += keypoints[i][0]
        y += keypoints[i][1]
    if(len(keypoints) != 0):
        print(str((x/len(keypoints)/width)) + " \\ " + str((y/len(keypoints))/height))
    if(len(keypoints) != 0):
        xrel = x/len(keypoints)/width
        yrel = y/len(keypoints)/height
    return xrel,yrel


def sendto(s,x,y):
    x = np.float32(x)
    y = np.float32(y)
    pos = bitstruct.pack('u8u32f32f32u32',5,285212672,x,y,16777216)
    s.sendall(pos)

def drawcar(frame):
    pass