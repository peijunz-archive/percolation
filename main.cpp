#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

//1458816173 Site Percolation有问题

int main(){
    ndarray<int> c(2,6);
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            c(i,j)=i+j;
        }
    }
    c.print();
    c.transpose();
    c.print();
    cout<<c.size()<<endl;
    setseed();
    cout<<myrand();
    return 0;
}
