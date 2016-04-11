#include <iostream>
#include <initializer_list>
#include "ndarray.h"
#include "deque.h"
#include "graph.h"
#include "iter.h"
#include "16807.h"

int testrand(){
    myrand(-1);
    for(int i=0;i<100;i++){
        if(myrand()>1 && myrand<0)
            return 1;
    }
    return 0;
}

int testndarray(){
    ndarray<char> a[5];
    ndarray<int> b={3,4,5};
    int s[3]={2,3,4};
    ndarray<double> c(3,s);
    for(i=0;i<5;i++){
        a[i].set(3,5);
    }
    a.print();
    b.print();
    c.print();
}
