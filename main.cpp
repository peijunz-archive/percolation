#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

int main(){
    ndarray<int> b{5,5};
    int size=b.size();
    for(int i=0;i<size;i++){
        b[i]=i+1;
    }
    b.transpose();
    b.print();
    return 0;
}
