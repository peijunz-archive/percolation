#!/usr/bin/env python3
##
# @file collectdata.py
#
from numpy import *
from scipy.optimize import leastsq
import glob, os
## Interval for formatted data
interval=array([24, 12, 12, 16, 12])
## Start/ending position for each block
pos=cumsum(interval)
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
        return sqrt(self.mean2()-self.mean()**2)
## Get the standard error of the mean
    def err(self):
        return self.std()/sqrt(self.N)
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
    return array([d.unpack() for d in f]).transpose([2,1,0])
## @brief Collect all data in data/ folder and conver it to array
def collectdata():
    lf=glob.glob("data/*.dat")
    f=getfile(lf[0])
    for ff in lf[1:]:
        f2=getfile(ff)
        for i in range(len(f)):
            f[i]+=f2[i]
    return toarray(f)
## Linear fit with plotting
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
# Fit the bahavior of density
def density(L, p):
    rho0, a, y= p
    return rho0+a*L**(-y)

def res_density(p, rho, L):
    """
    实验数据x, y和拟合函数之间的差，p为拟合需要找到的系数
    """
    return  rho - func(L, p)
# Cache data for data analysing
def cachedata(fname="data.npy"):
    if not os.path.isfile(fname):
        print("Caching data to %s..."%(fname))
        save(fname, collectdata())
    print("Cache ready!")  
cachedata()
data=load("data.npy")
#data=collectdata()
val, err=data
S=sum(val[0:3], axis=0)
data[:, :3]/=S
