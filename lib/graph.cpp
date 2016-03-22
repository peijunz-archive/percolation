#include <stdio.h>
#include <stdlib.h>
#include "16807.h"
#include "ndarray.h"
typedef char corrtype;
struct torus{
    ndarray site;
    ndarray *bond;
};
inline int torusdim(torus t){
    return t.site.dim;
}

void inittorus(torus &t, int dim, int width){
    int shape[dim],ax;
    t.bond=(ndarray *)malloc(sizeof(ndarray)*dim);
    for(int ax=0;ax<dim;ax++){
        shape[ax]=width;
    }
    setmem(t.site,dim,shape);
    for(ax=0;ax<dim;ax++)
        setmem(t.bond[ax],dim,shape);
}
void destroytorus(torus &t){
    int ax;
    destroy(t.site);
    for(ax=0;ax<t.site.dim;ax++)
        destroy(t.bond[ax]);
    free(t.bond);
}

void setbond(torus &t, double prob){
    int ax, i;
    for(ax=0;ax<t.site.dim;ax++){
        place(t.bond[ax],prob);
        for(i=0;i<t.site.stride[0];i++){
            t.site.head[i]+=t.bond[ax].head[i];
            t.site.head[i]+=rollval(t.site,i,ax,false);
        }
    }
}
void setsite(torus &t, double prob){
    int ax,i;
    place(t.site,prob);
    for(ax=0;ax<t.site.dim;ax++){
        for(i=0;i<t.site.stride[0];i++){
            t.bond[ax].head[i]=t.site.head[i]*rollval(t.site,i,ax);
        }
    }
}
void wrapping(torus &t){

}
