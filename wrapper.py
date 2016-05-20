#! /usr/bin/env python3
import os
from pylab import *
from time import time
prgpath="/home/zpj/code/build-percolation-Desktop-Release/percolation"
DIM=3
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
    return [m, s]
def analysel(w):
    L=array([analyse(wid) for wid in w])
    return L.transpose(2, 1, 0)
def linearfit(x, y, name=""):
    l=len(x)
    if(l!=len(y)):
        return
    n=len(x)
    mx=mean(x)
    my=mean(y)
    xy=dot(x,y)-n*mx*my
    xx=dot(x,x)-n*mx*mx
    yy=dot(y,y)-n*my*my
    k=xy/xx
    b=my-k*mx
    r=xy/sqrt(xx*yy)
    plot(x, y, 'o')
    plot(x, k*x+b, '-', label="D=%f"%(k))
    legend(loc="upper left")
    if(len(name)):
        savefig(name+'.pdf')
    return k,b,r
if __name__=="__main__":
    w=array([16,24,32,48,64,96,128])
    #for i in w:
        #gendata(i,1000,0.5)
    for i in w:
        print(getdata(i, 1))
