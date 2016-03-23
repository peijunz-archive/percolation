#include <stdio.h>
#include <stdlib.h>
#include "16807.h"
#include "ndarray.h"
#include "deque.h"

//External Macros
#define BOND true
#define SITE false

//Local Macros
#define LINE "---------------------------\n"
#define EMPTY 0
#define INQUENE -1
#define POPOUT -2

typedef unsigned char corrtype;

struct torus{
    ndarray site;
    ndarray *bond;
    bool type;
};

void inittorus(torus &t, int dim, int width, int type=BOND){
    int shape[dim],ax;
    for(int ax=0;ax<dim;ax++){
        shape[ax]=width;
    }
    setmem(t.site,dim,shape);
    if(type==BOND){
        t.bond=(ndarray *)malloc(sizeof(ndarray)*dim);
        for(ax=0;ax<dim;ax++)
            setmem(t.bond[ax],dim,shape);
        t.type=BOND;
    }
    else{
        t.type=SITE;
        t.bond=NULL;
    }
}

void destroytorus(torus &t){
    int ax;
    destroy(t.site);
    if(t.type==BOND){
        for(ax=0;ax<t.site.dim;ax++)
            destroy(t.bond[ax]);
        free(t.bond);
    }
}

void setbond(torus &t, double prob){
    int ax, i, hehe=0,near;
    if(t.type==SITE){
        printf("setbond(): TYPE ERROR for BOND PERCOLATION!\n");
        exit;
    }
    for(ax=0;ax<t.site.dim;ax++){
        place(t.bond[ax],prob);
    }
    init(t.site);
    for(i=0;i<t.site.stride[0];i++){
        for(ax=0;ax<t.site.dim;ax++){
            if(t.bond[ax].head[i]){
                near=rollindex(t.bond[ax],i,ax,hehe);
                t.site.head[near]+=1;
                t.site.head[i]+=1;
            }
        }
    }
}

void setsite(torus &t, double prob){
    int ax,i;
    if(t.type==SITE)
        place(t.site,prob);
    else{
        printf("setsite(): TYPE ERROR for SITE PERCOLATION!\n");
        exit;
    }
}

void printtorus(torus &t){
    printf("%sSite:\n%s",LINE,LINE);
    printarr(t.site);
    if(t.type==SITE)
        return;
    for(int i=0;i<t.site.dim;i++){
        printf("%sBond in axis %d\n%s", LINE, i, LINE);
        printarr(t.bond[i]);
    }
}

void wrapping(torus &t){
    quene q;
    initq(q);
    ndarray zone[t.site.dim];
    int delta, wrap[t.site.dim];
    int i, point=0,near=0, wrapcount, path, ax, absax, tmpax;
    for(ax=0;ax<t.site.dim;ax++){
        setmem(zone[ax],t.site.dim, t.site.shape);
        init(zone[ax]);//init zone to 0
    }
    for(i=0;i<t.site.stride[0];i++){
        if (t.site.head[i]>0){//unvisited site
            for(ax=0;ax<t.site.dim;ax++){//初始化起点的区为{0,0,...,0}
                zone[ax].head[i]=0;//init to 0
                wrap[ax]=0;
            }
            wrapcount=0;//记录有几个方向wrap
            append(q,i);
//            printf("%sNew cluster:\n%s",LINE,LINE);
            while(q.length>0){
                point=popleft(q);
//                printf("%4d",point);
                t.site.head[point]=POPOUT;
                for(ax=-t.site.dim;ax<t.site.dim;ax++){
                    near=rollindex(t.site,point,ax,delta);
                    absax=(ax>=0)?ax:(-1-ax);
                    if(t.type==SITE) path=t.site.head[near];
                    else{//bond
                        if(ax>=0) path=t.bond[absax].head[point];
                        else path=t.bond[absax].head[near];
                    }
                    if(path){
                        if (t.site.head[near]>0){
                            for(tmpax=0;tmpax<t.site.dim;tmpax++)
                                zone[tmpax].head[near]=zone[tmpax].head[point];
                            zone[absax].head[near]+=delta;
                            append(q,near);
                            t.site.head[near]=INQUENE;
                        }
                        else if(t.site.head[near]==INQUENE){
                            zone[absax].head[point]+=delta;//for comparation, will cancel it soon
                            for(int tmpax=0;tmpax<t.site.dim;tmpax++){
                                if(wrap[tmpax]) continue;
                                if(zone[tmpax].head[near] != zone[tmpax].head[point]){
                                    wrap[tmpax]=1;
                                    wrapcount+=1;
                                }
                            }
                            zone[absax].head[point]-=delta;//Canceled!
                        }
                    }
                }
            }
//            putchar('\n');
            if(wrapcount>0){
                printf("Wrapping Status: ");
                for(tmpax=0;tmpax<t.site.dim;tmpax++)
                    printf("%d ",wrap[tmpax]);
                putchar('\n');
            }
        }
    }
    for(ax=0;ax<t.site.dim;ax++)
        destroy(zone[ax]);
    return;
}
