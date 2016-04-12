#include <iostream>
#include <ctime>
/**
 * @file 16807.cpp
 * @author zpj
 * @brief 16807 psudo-random number generator
 * @bug No
 */
/**
 * @brief 16807 psudo-random number generator
 * @param init
 *  + init=0, continue iteration using the previous seed
 *  + init<0, reset the seed using current time
 *  + init>0, reset the seed using init
 * @return psudo-random number
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
