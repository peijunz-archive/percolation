#include <cstdio>
#include "percolation.h"
using namespace std;
typedef uint64_t sum_t;
int gendata() {
    int width;
    int times;
    sum_t leaf = 0, junc = 0, circ = 0,
          maxclus = 0, maxlfree = 0, maxbfree = 0, maxleaf = 0;
    double prob;
    printf("D=3, Input\twidth\ttimes\tprob\n");
    scanf("%d%d%lf", &width, &times, &prob);
    printf("Parameters:\t%d\t%d\t%f\n", width, times, prob);
    ctorus<2> t(width);
    double N=times, A=times*prob*2*t.bonds.size();
    printf("Item\tSum\tMean\tRatio\n");
    for(int i = 0; i < times; i++) {
        t.setbond(prob);
        t.prune();
        t.dejunct();
        leaf += t.countclus - t.countlfree;
        junc += t.countlfree - t.countbfree;
        circ += t.countbfree;
        maxclus += t.maxclus;
        maxlfree += t.maxlfree;
        maxbfree += t.maxbfree;
        maxleaf += t.maxleaf;
    }
    printf("Leaf\t%ld\t%f\t%f\n", leaf, leaf/N, leaf/A);
    printf("Junc\t%ld\t%f\t%f\n", junc, junc/N, junc/A);
    printf("Circ\t%ld\t%f\t%f\n", circ, circ/N, circ/A);
    printf("mClus\t%ld\t%f\n", maxclus, maxclus/N);
    printf("mLFClus\t%ld\t%f\n", maxlfree, maxlfree/N);
    printf("mBFClus\t%ld\t%f\n", maxbfree, maxbfree/N);
    printf("mLeaf\t%ld\t%f\n", maxleaf, maxleaf/N);
    return 0;
}

int main() {
    gendata();
    return 0;
}
