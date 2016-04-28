#include <iostream>
#include "percolation.h"
#include <cmath>
ltorus<2> t(2048);
int main(){
//    string s("vinilla.png");
//    string s2("pruned.png");
//    string s3("cut.png");
    setseed();
    for(int i=0;i<5;i++){
        t.setbond(0.5);
        t.prune();
        t.dejunct();
        t.getratio();
    }
//    t.savetoimg<5>(s3);
    return 0;
}
