#include <stdio.h>
#include <stdlib.h>
#include "lib/16807.h"
#include "lib/ndarray.h"
#include "lib/graph.h"

int main(){
    ndarray grid;
    int s[3]={2,3,4},i=0,p[3]={1,1,1};
    setmem(grid, 3, s);
    init(grid, '5');
//    for(i=0;i<grid.stride[0];i++){
//        grid.head[i]=i;
//    }
    myrand(INIT_RANDOM);
    place(grid);
    printarr(grid);
    return 0;
}
