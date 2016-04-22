#include <iostream>
#include "percolation.h"

int main(){
    ndarray<int> a;
    ltorus<2> t(5);
    setseed();
    t.setbond(0.9);
    cout<<sizeof(a)<<endl;
    return 0;
}
