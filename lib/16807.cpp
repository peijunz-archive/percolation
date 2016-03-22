#include <time.h>

#define M 2147483647    //2^31-1
#define A 16807         //16807产生器

double myrand(int init=0){
    /*init>0, use thae value to initialize seed
              =0, continue
              <0, use time to initialize*/
    const int q=M/A, r=M%A;
    static int seed;
    int ip=A*(seed%q), rp=r*(seed/q);
    if(init>0)
        seed=init%M;
    else if(init<0)
        myrand((unsigned int)time(NULL));
    else{
        if(ip>=rp) seed=ip-rp;
        else seed=ip-rp+M;
    }
    return ((double)seed)/(M-1);
}
