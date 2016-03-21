#include <stdio.h>
#include <stdlib.h>
#include "lib/16807.h"
#include "lib/ndarray.h"

int main(){
    ndarray grid;
    int s[3]={2,3,4},i=0,p[3]={1,1,1};
    setmem(grid, 3, s);
    init(grid, '5');
    for(i=0;i<grid.size;i++){
        grid.head[i]=i;
    }
    printarr(grid);
    printf("Test index:%d\t%d\n",i,getval(grid,p));
    return 0;
}
