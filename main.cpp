#include <stdio.h>
#include <stdlib.h>
#include "lib/16807.h"
#include "lib/ndarray.h"
#include "lib/graph.h"
#include "lib/deque.h"

int main(){
    torus t;
//    int i=0,ax=0;
//    unsigned char c=250;//Use unsigned char to save coordinate
    quene q;
    initq(q);
    myrand(INIT_RANDOM);
//    inittorus(t, 2, 10000, BOND);
//    setbond(t,0.5);
    inittorus(t, 2, 10000, SITE);
    setsite(t,0.59);
//    printtorus(t);
//    for(axs=0;ax<t.site.dim;ax++){
//        setmem(zone[i],t.site.dim, t.site.shape);
//        printf("main\n");
//        init(zone[i]);//init zone to 0
//        printarr(zone[i]);
//    }
    wrapping(t);
    destroytorus(t);
//    printf("%d\n",c);
    return 0;
}
