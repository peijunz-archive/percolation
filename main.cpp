#include <stdio.h>
#include <stdlib.h>
#include "lib/16807.h"
#include "lib/ndarray.h"
#include "lib/graph.h"
#include "lib/deque.h"
#define TEST 19951201
int main(){
    myrand(INIT_RANDOM);
    torus t(2,5,0.59,SITE);
    t.print();;
    t.wrapping();
    return 0;
}
