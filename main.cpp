#include <iostream>
#include "classify.h"
ltorus<2> t(128);
int main(){
//    string s("vinilla.png");
//    string s2("pruned.png");
    string s3("cut.png");
    uint8_t c[4]={0, 64, 255, 128};
    setseed();
    t.setbond(0.5);
    t.prune();
    t.dejunct();
    t.savetoimg<5>(s3, c);
    return 0;
}
