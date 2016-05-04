#include <iostream>
using namespace std;
#define WRAPONCE
#include "percolation.h"
//#include "save2dimg.h"
int main(){
    ctorus<2> t(128);
    setseed();
    for(int i=0;i<1000;i++){
        t.setbond(0.5);
        t.prune();
        t.dejunct();
        t.bondcount();
    }
    return 0;
}
