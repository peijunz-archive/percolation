#include <stdio.h>
#include "lib/16807.h"
#include "lib/ndarray.cpp"
#include "lib/deque.h"
#include "lib/graph.h"
#define TEST 1995120185
int main(){
    myrand(INIT_RANDOM);
    torus t(8,8,0.07,BOND);
//    t.print();
    t.wrapping();
    return 0;
}
