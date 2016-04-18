#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

//1458816173 Site Percolation有问题

int main(){
    ndarray<int> c{3,3};
    ndarray<int> d=c;
    for(int i=0;i<c._shape(0);i++){
        for(int j=0;j<c._shape(1);j++){
            c(i,j)=i*i+j*j;
        }
    }
    d[0]=8;
    c.print();
    d.print();
//    d=ndarray<int>(c);
    cout<<c.size()<<endl;
    return 0;
}
