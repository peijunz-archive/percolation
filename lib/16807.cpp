#include <iostream>
#include <ctime>
/** \file 16807.cpp
 * 16807 psudo-random number generator
 */
double myrand(int init=0){
    const int M=2147483647, A=16807;
    const int q=M/A, r=M%A;
    static int seed;
    int ip=A*(seed%q), rp=r*(seed/q);
    if(init>0)
        seed=init%M;
    else if(init<0){
        seed=((unsigned int)time(NULL))%M;
        std::cout<<"The seed is "<<seed<<std::endl;
    }
    else{
        if(ip>=rp) seed=ip-rp;
        else seed=ip-rp+M;
    }
    return ((double)seed)/(M-1);
}
