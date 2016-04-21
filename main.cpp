#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

int main(){
    ndarray<int> a{3,4,4,5};
    for(int i=0;i<a.size();i++){
        a[i]=i+1;
    }
    cout<<a;
    return 0;
}
