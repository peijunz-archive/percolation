#include <stdio.h>
#include <stdlib.h>
#include "lib/16807.h"
#include "lib/ndarray.h"
#include "lib/graph.h"
#include "lib/deque.h"
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
