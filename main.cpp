#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "test.h"
#include "singlelist.h"
#include "test.h"
//1458816173 Site Percolation有问题

int main(){
    ndarray<int> a(5,3);
    ndarray<int> c(3,3);
    c=ndarray<int>(3,5);
    cout<<a.size()<<c.size()<<endl;
    testlist();
    testrand();
    return 0;
}
