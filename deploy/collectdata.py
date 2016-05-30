#!/usr/bin/env python3
##
# @file collectdata.py
#
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import leastsq
import glob, os
## Interval for formatted data
interval=np.array([24, 12, 12, 16, 12])
## Start/ending position for each block
pos=np.cumsum(interval)
## Second order statistics
class stat2:
    def __init__(self, N=0, S=0, S2=0):
        ## @brief counts of elements
        self.N=N
        ## @brief sum of `x`
        self.S=S
        ## @brief sum of `x^2`
        self.S2=S2
        if(self.N*self.S2<self.S**2):
            print('ERROR! sigma**2<0', N, S, S2)
            self.S2=self.S**2/self.N
    def __add__(self, other):
        return stat2(self.N+other.N, self.S+other.S, self.S2+other.S2)
    def __str__(self):
        return str([self.N, self.mean(), self.err()])
## Add a element into stat
    def append(self, elem):
        self.N+=1
        self.S+=elem
        self.S2+=elem**2
## Get the mean
    def mean(self):
        return self.S/self.N
## Get the mean of the 2nd order
    def mean2(self):
        return self.S2/self.N
## Get the standard deviation of the distribution
    def std(self):
        return np.sqrt(self.mean2()-self.mean()**2)
## Get the standard error of the mean
    def err(self):
        return self.std()/np.sqrt(self.N)
## simplest statistical results
    def result(self):
        return [self.mean(), self.err()]
## packing a list of data with its meta data
class datapack:
    ## Name for each elem in l
    name=['Leaf-Total', 'Junction-Total', 'Circuit-Total', 'Max-Clus',
          'Max-Leaf-Free-Clus', 'Max-Branch-Free-Clus', 'Max-Leaf-Clus']
    def __init__(self, meta, l):
        ## @brief Meta data for the data l
        self.meta=meta
        ## @brief A list for the statistical data
        #
        # Each element of l corresponding to elems in name list.
        self.l=l
    def __add__(self, rhs):
        if self.meta==rhs.meta:
            return datapack(self.meta, [self.l[i]+rhs.l[i] for i in range(len(self.name))])
        else:
            print("Unmatched metadata")
            return
    def __str__(self):
        s="--------\nMatadata: "+str(self.meta)+'\n--------\n'
        for i in range(len(self.name)):
            s+=self.name[i]+'\t'+str(self.l[i])+'\n'
        return s
    def unpack(self):
## Unpack the results to mean and err
        return [s.result() for s in self.l]
## Convert str to metadata
def str2meta(s):
    return [eval(k.split('=')[1]) for k in s.split(', ')]
## Convert str to stat2
def str2stat(ss, N):
    data=[eval(ss[pos[i]:pos[i+1]]) for i in range(4)]
    S=data[0]
    S2=N*data[1]**2+S**2/N
    return stat2(N, S, S2)
## Convert str to datapack
def str2pack(s):
    l=s.split('\n')
    metadata=str2meta(l[0])
    N=metadata[2]
    del metadata[2]
    l=l[4:-1]
    return datapack(metadata, [str2stat(k, N) for k in l])
## Read a single file to datapack list
def getfile(name):
    f=open(name, 'r')
    dstr=''.join(f.readlines()[1:-1])
    L=dstr.split('>>> ')[1:]
    return [str2pack(k) for k in L]
## Unpack list of packs to array
def toarray(f):
    return np.array([d.unpack() for d in f]).transpose([2,1,0])
## @brief Collect all data in data/ folder and conver it to array
def collectdata():
    lf=glob.glob("data/*.dat")
    f=getfile(lf[0])
    for ff in lf[1:]:
        f2=getfile(ff)
        for i in range(len(f)):
            f[i]+=f2[i]
    return toarray(f)
## Linear fit
def linearfit(x, y):
    l=len(x)
    if(l!=len(y)):
        return
    n=len(x)
    mx=np.mean(x)
    my=np.mean(y)
    xy=np.dot(x,y)-n*mx*my
    xx=np.dot(x,x)-n*mx*mx
    yy=np.dot(y,y)-n*my*my
    k=xy/xx
    b=my-k*mx
    r=xy/np.sqrt(xx*yy)
    return k,b,r, lambda x:k*x+b
# Cache data for data analysing
def cachedata(fname="data.npy"):
    if not os.path.isfile(fname):
        print("Caching data to %s..."%(fname))
        np.save(fname, collectdata())
cachedata()
L=np.array([16, 23, 32, 45, 64, 90, 128, 181, 256])
data=np.load("data.npy")
#data=collectdata()
val, err=data
D=-1.8596#47275
Dh=2.52237
Dl=1.87246
x=L**D
S=np.sum(val[0:3], axis=0)
data[:, :3]/=S
def cpower(p, rho, L):
    """
    实验数据x, y和拟合函数之间的差，p为拟合需要找到的系数
    """
    rho0, a, y= p
    if(abs(y)>3):
        return 100000000
    return  rho0+a*L**y-rho
def cexp(p, y, x):
    k, b, d=p
    return k*np.log(x+d)+b-y
def drawdensity():
    k,b,r,f=linearfit(L**D, val[1])
    print(b)
    plt.plot(x, f(x), 'k-')
    plt.errorbar(x, val[1], err[1], fmt='o')
    plt.xlabel(r"$L^{%.4f}$"%D)
    plt.ylabel(r"$\rho_j$")
    plt.grid()
    plt.savefig("junction.pdf")
    plt.cla()
    
    k,b,r,f=linearfit(L**D, val[2])
    print(b)
    plt.plot(x, f(x), 'k-')
    plt.errorbar(x, val[2], err[2], fmt='bo')
    plt.xlabel(r"$L^{%.4f}$"%D)
    plt.ylabel(r"$\rho_b$")
    plt.grid()
    plt.savefig("nonbridge.pdf")
    plt.cla()
    
    plt.errorbar(x, (val[0]-0.796)/1e-4, err[0]/1e-4, fmt='ro')
    plt.xlabel(r"$L^{%.4f}$"%D)
    plt.ylabel(r"$(\rho_l-0.796)/10^{-4}$")
    plt.ylim([-1, 1])
    plt.grid()
    plt.savefig("leaf.pdf")
    plt.cla()
def drawmax():
    tmp=val[4]*L**(-Dh)
    k,b,r,f=linearfit(x, tmp)
    print(b)
    plt.plot(x, f(x), 'k-', x, tmp, 'bo')
    plt.xlabel(r"$L^{%.4f}$"%D)
    plt.ylabel(r"$L^{%.5f}C_{lf}$"%Dh)
    plt.grid()
    plt.savefig("maxl.pdf")
    plt.cla()

    tmp=val[4]*L**(-Dh)
    k,b,r,f=linearfit(x, tmp)
    print(b)
    plt.plot(x, f(x), 'k-', x, tmp, 'bo')
    plt.xlabel(r"$L^{%.4f}$"%D)
    plt.ylabel(r"$L^{%.5f}C_{bf}$"%Dl)
    plt.grid()
    plt.savefig("maxb.pdf")
    plt.cla()
    
def fitdensity():
    p=np.array([ 0.05678149, -0.98247447,  -1.85891885])
    print("枢纽比例", leastsq(cpower, p, args=(val[1], L)))
    p=np.array([0.14722413,  0.98731717,  -1.86037568])
    print("非桥比例", leastsq(cpower, p, args=(val[2], L)))
def fitclus():
    p=np.array([-19.43377199,   0.97956239,   2.52234679])
    print("最大丛", leastsq(cpower, p, args=(val[3], L)))
    p=np.array([-11.62610919,   0.34717929,   2.52239323])
    print("最大去叶丛", leastsq(cpower, p, args=(val[4], L)))
    p=np.array([-2.06606095,  0.74730953,  1.8724634 ])
    print("最大去桥丛", leastsq(cpower, p, args=(val[5], L)))
def fitleaf():
    p=np.array([  73.40869749, -150.01109385,    5.29100011])
    print("最大叶", leastsq(cexp, p , args=(val[-1], L)))
def drawleaf():
    x=np.log(L+5.291)
    a=val[-1]
    k,b,r,f=linearfit(x, a)
    print(k, b, r)
    plt.plot(x, f(x), 'k-', x, a, 'o')
    plt.xlabel(r"$\ln(L+5.291)$")
    plt.ylabel(r"$C_{l}$")
    plt.grid()
    plt.savefig("maxleaf.pdf")
    plt.cla()

if __name__=="__main__":
    #fitleaf()
    #fitdensity()
    #fitclus()
    drawmax()
    drawmax()
    
    
