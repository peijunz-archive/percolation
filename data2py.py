#!/usr/bin/env python3
from numpy import *
import glob, os
interval=array([24, 12, 12, 16, 12])
pos=cumsum(interval)
t="Leaf-Total              801794      1.053e+02   7.223369e+03    9.992e+00"
class stat2:
    '''Second order statistics
    params: N is number, S is sum(x), S2 is sum(x**2)'''
    def __init__(self, N=0, S=0, S2=0):
        self.N=N
        self.S=S
        self.S2=S2
        if(self.N*self.S2<self.S**2):
            print('ERROR! sigma**2<0', N, S, S2)
            self.S2=self.S**2/self.N
    def __add__(self, other):
        return stat2(self.N+other.N, self.S+other.S, self.S2+other.S2)
    def append(self, elem):
        '''Add a element into stat'''
        self.N+=1
        self.S+=elem
        self.S2+=elem**2
    def mean(self):
        '''Get the mean'''
        return self.S/self.N
    def mean2(self):
        '''Get the mean of the 2nd order'''
        return self.S2/self.N
    def std(self):
        return sqrt(self.mean2()-self.mean()**2)
    def err(self):
        return self.std()/sqrt(self.N)
    def result(self):
        return [self.mean(), self.err()]
    def __str__(self):
        return str([self.N, self.mean(), self.err()])
class datapack:
    q=7
    name=['Leaf-Total', 'Junction-Total', 'Circuit-Total', 'Max-Clus',
          'Max-Leaf-Free-Clus', 'Max-Branch-Free-Clus', 'Max-Leaf-Clus']
    def __init__(self, meta, l):
        self.meta=meta
        self.l=l
    def __str__(self):
        s="--------\nMatadata: "+str(self.meta)+'\n--------\n'
        for i in range(self.q):
            s+=self.name[i]+'\t'+str(self.l[i])+'\n'
        return s
    def __add__(self, rhs):
        if self.meta==rhs.meta:
            return datapack(self.meta, [self.l[i]+rhs.l[i] for i in range(datapack.q)])
        else:
            print("Unmatched metadata")
            return
    def unpack(self):
        return [s.result() for s in self.l]
def str2meta(s):
    return [eval(k.split('=')[1]) for k in s.split(', ')]
def str2stat(ss, N):
    data=[eval(ss[pos[i]:pos[i+1]]) for i in range(4)]
    S=data[0]
    S2=N*data[1]**2+S**2/N
    return stat2(N, S, S2)
def datablock(s):
    l=s.split('\n')
    metadata=str2meta(l[0])
    N=metadata[2]
    del metadata[2]
    l=l[4:-1]
    return datapack(metadata, [str2stat(k, N) for k in l])
def getfile(name):
    f=open(name, 'r')
    dstr=''.join(f.readlines()[1:-1])
    L=dstr.split('>>> ')[1:]
    return [datablock(k) for k in L]
def toarray(f):
    return array([d.unpack() for d in f]).transpose([2,1,0])
def collectdata():
    lf=glob.glob("data/*.dat")
    f=getfile(lf[0])
    for ff in lf[1:]:
        f2=getfile(ff)
        for i in range(len(f)):
            f[i]+=f2[i]
    return toarray(f)
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
    print(collectdata())
