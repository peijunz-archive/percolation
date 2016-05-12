#! /usr/bin/env python3
import os
from numpy import *
from time import time
prgpath="/home/zpj/code/build-percolation-Desktop-Release/percolation"
def gettime():
    return int(time()%86400)
rtime=gettime()
def genfname(width):
    '''Generate filename for a width'''
    return "data/%d-%d.dat"%(width, rtime)
def gendata(width, times, prob):
    '''Generate classification data for given percolation configuration'''
    assert((width>5) and (times>0) and (0<prob<1))
    os.system('echo "%d %d %f"|%s > %s'%(width, times, prob, prgpath, genfname(width)))
    print("Completed! "+genfname(width))
def analyse(width):
    '''Extract data from data file'''
    A=loadtxt(genfname(width), skiprows=4)
    m=mean(A, axis=0)
    s=std(A, axis=0)
    m[:4]/=2*width**2
    s[:4]/=2*width**2
    return [m, s]
def analysel(w):
    L=array([analyse(wid) for wid in w])
    return L.transpose(2, 1, 0)
def linearfit(x, y):
    if(len(x)!=len(y)):
        return
    n=len(x)
    xy=dot(x,y)
    xx=dot(x,x)
    yy=dot(y,y)
    k=xy/xx
    b=mean(y)-k*mean(x)
    r=xy/sqrt(xx*yy)
    return k,b,r
if __name__=="__main__":
    w=array([16,24,32,48,64,96,128])
    #for i in w:
        #gendata(i,1000,0.5)
    for i in w:
        print(getdata(i, 1))
