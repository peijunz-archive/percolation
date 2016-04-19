#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

int main(){
    cout<<benchndarray(1000)<<endl;
    return 0;
}
