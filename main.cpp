#include <iostream>
using namespace std;
#define WRAPONCE
#include "percolation.h"
//#include "save2dimg.h"
int main(){
    ctorus<2> t(128);
    setseed();
    cout<<"总键\t有效键\t去枝\t去桥\t最大丛\t去枝丛\t去桥丛\n";
    for(int i=0;i<1000;i++){
        t.setbond(0.5);
        t.prune();
        t.dejunct();
//        t.bondcount();
    }
    return 0;
}
