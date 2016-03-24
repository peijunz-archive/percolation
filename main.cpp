#include <stdio.h>
#include "lib/16807.h"
#include "lib/ndarray.h"
#include "lib/deque.h"
#include "lib/graph.h"
#define TEST 1995120185
int main(){
    int seed, dim, width;
    double prob;
    printf("Input seed, dimension, width, prob: ");
    scanf("%d%d%d%lf",&seed, &dim, &width,&prob);
    printf("%d %d %d %f\n",seed, dim, width, prob);
    myrand(seed);
    torus t(dim,width,prob,BOND);
//    t.print();
    t.wrapping();
    return 0;
}
