#include <iostream>
#include <ctime>
#define NDEBUG
#include <cassert>
using namespace std;
/**
 * @file 16807.cpp
 * @brief 16807 psudo-random number generator
 *
 *
 * See [Lehmer random number generator](https://en.wikipedia.org/wiki/
 * Lehmer_random_number_generator) for more details
 */
/// M=2^31-1
const int M=2147483647;
/// A=7^5
const int A=16807;
const int q=M/A, r=M%A;
int seed=0;

/// Go one step
double myrand(){
    assert(seed);
    int ip=A*(seed%q), rp=r*(seed/q);
    if(ip>=rp) seed=ip-rp;
    else seed=ip-rp+M;
    return ((double)seed)/(M-1);
}

/**
 * @brief 16807 psudo-random number generator initializer
 * @param init
 *  + init<0, reset the seed using current time
 *  + init>=0, reset the seed using init
 *
 * @param go    go for some iterations to prevent the issue
 *              that time/M is always very small
 * @return psudo-random number
 */
void setseed(int init=-1, int go=10){
    if(init>=0)
        seed=init%M;
    else if(init<0){
        seed=((unsigned int)time(NULL))%M;
        cout<<"The seed is "<<seed<<endl;
    }
    for(int i=0;i<go;i++){
        myrand();
    }
}
/**
 * @brief Get the current seed for reproduction
 * @return current seed
 */
int getseed(){
    return seed;
}
