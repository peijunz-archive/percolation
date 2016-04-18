#include <iostream>
#include <ctime>
using namespace std;
/**
 * @file 16807.cpp
 * @author zpj
 * @brief 16807 psudo-random number generator
 * @bug No
 */
const int M=2147483647, A=16807;
const int q=M/A, r=M%A;
int seed=0;
/**
 * @brief 16807 psudo-random number generator initializer
 * @param init
 *  + init<0, reset the seed using current time
 *  + init>=0, reset the seed using init
 *
 * M=2^31-1, A=7^5
 * @return psudo-random number
 */
void setseed(int init=-1){
    if(init>=0)
        seed=init%M;
    else if(init<0){
        seed=((unsigned int)time(NULL))%M;
        cout<<"The seed is "<<seed<<endl;
    }
}
/// Go one step
double myrand(){
    int ip=A*(seed%q), rp=r*(seed/q);
    if(ip>=rp) seed=ip-rp;
    else seed=ip-rp+M;
    return ((double)seed)/(M-1);
}
