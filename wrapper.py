#! /usr/bin/env python3
import os
from numpy import *
prgpath="/home/zpj/code/build-percolation-Desktop-Release/percolation"
def genfname(width):
    '''Generate filename for a width'''
    return "data/%d.dat"%(width)
def gendata(width, times, prob):
    '''Generate classification data for given percolation configuration'''
    assert((width>5) and (times>0) and (0<prob<1))
    os.system('echo "%d %d %f"|%s > %s'%(width, times, prob, prgpath, genfname(width)))
    return 0
def getdata(width, i=-1):
    '''Retrieve data from data file and extract a portion'''
    A=loadtxt(genfname(width), skiprows=4).T
    if i>=0:
        return A[i]
    return A

if __name__=="__main__":
    w=array([16,24,32,48,64,96,128])
    #for i in w:
        #gendata(i,1000,0.5)
    for i in w:
        print(getdata(i, 1))
