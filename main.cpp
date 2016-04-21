#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

int main(){
    int a[5]={1,2,3,4,5};
    const int *p=a;
    a[2]=7;
    cout<<p[2]<<endl;
    return 0;
}
