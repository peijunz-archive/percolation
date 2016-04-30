#include <iostream>
#include "classify.h"
#include "wrapping.h"
int main(){
    wtorus<2> t(128);
    cout<<bisecp(t,1000)<<endl;
    return 0;
}
