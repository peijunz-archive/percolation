#include <iostream>
#include "percolation.h"
#include <cmath>
ltorus<2> t(16);
int main(){
    string s("test.png");
    string s2("pruned.png");
    string s3("cut.png");
//    setseed(1461604257);
    setseed();
    t.setbond(0.5);
    t.prune();
    t.dejunct();
    t.savetoimg<5>(s3);
//    bisecp(10000);
//    findmulti(0.486328, 1000);
//    cout<<findmulti(0.457031, 100000)<<endl;
    return 0;
}
