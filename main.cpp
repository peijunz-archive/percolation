#include <iostream>
using namespace std;
#include "classify.h"
#include "wrapping.h"
int main(){
    wtorus<2> t(128);
    cout<<(1u<<31)-1<<endl;
    setseed();
    t.setbond(0.5);
    t.prune();
    cout<<t.wrapping()<<endl;
    return 0;
}
