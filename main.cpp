#include <stdio.h>
#include <iostream>
#include <initializer_list>
#include "ndarray.h"
#include "deque.h"
#include "graph.h"
#include "iter.h"
#include "16807.h"
//1458816173 Site Percolation有问题
#define DEBUG

int main(){
    int seed, dim, width;
    double prob;
#ifndef DEBUG
    printf("Input seed, dimension, width, prob: ");
    scanf("%d%d%d%lf",&seed, &dim, &width,&prob);
    printf("%d %d %d %f\n",seed, dim, width, prob);
#else
    seed=19951201;
    dim=8;
    width=8;
    prob=0.068;
#endif
    myrand(seed);
    torus t(dim,width,prob);
    t.wrapping();
    return 0;
}
