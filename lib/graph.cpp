#include <stdio.h>
#include <stdlib.h>
#include "16807.h"
#include "ndarray.h"
#include "deque.h"
#define LINE "---------------------------\n"
#define EMPTY 0
#define INQUENE -1
#define POPOUT -2
#define BOND true
#define SITE false
typedef char corrtype;
struct torus{
    ndarray site;
    // if elem>0, site. if ==0, nothing, if ==-1, inquene, if ==-2 pushed out
    ndarray *bond;
    bool type;
};

void inittorus(torus &t, int dim, int width){
    int shape[dim],ax;
    t.bond=(ndarray *)malloc(sizeof(ndarray)*dim);
    for(int ax=0;ax<dim;ax++){
        shape[ax]=width;
    }
    setmem(t.site,dim,shape);
    for(ax=0;ax<dim;ax++)
        setmem(t.bond[ax],dim,shape);
    t.type=BOND;
}
void destroytorus(torus &t){
    int ax;
    destroy(t.site);
    for(ax=0;ax<t.site.dim;ax++)
        destroy(t.bond[ax]);
    free(t.bond);
}

void setbond(torus &t, double prob){
    int ax, i, tmp;
    t.type=BOND;
    for(ax=0;ax<t.site.dim;ax++){
        place(t.bond[ax],prob);
    }
    init(t.site);
    for(i=0;i<t.site.stride[0];i++){
        for(ax=-t.site.dim;ax<t.site.dim;ax++){
            if(ax>0) tmp=t.bond[ax].head[i];
            if(ax<0) tmp=rollval(t.bond[ax],i,ax);
            if(tmp){
                t.site.head[i]=1;
                break;
            }
        }
    }
//    for(ax=0;ax<t.site.dim;ax++){
//        place(t.bond[ax],prob);
//        for(i=0;i<t.site.stride[0];i++){
//            t.site.head[i]+=t.bond[ax].head[i];
//            t.site.head[i]+=rollval(t.bond[ax],i,-1-ax);//优化为用循环，不需要记录次数
//        }
//    }
}
void setsite(torus &t, double prob){
    int ax,i;
    t.type=SITE;
    place(t.site,prob);
    for(ax=0;ax<t.site.dim;ax++){
        t.bond[ax]=t.site;
    }
}
void printtorus(torus &t){
    printf("%sSite:\n%s",LINE,LINE);
    printarr(t.site);
    for(int i=0;i<t.site.dim;i++){
        printf("%sBond in axis %d\n%s", LINE, i, LINE);
        printarr(t.bond[i]);
    }
}

void wrapping(torus &t){
    //for saving memory,
    quene q;
    initq(q);
    ndarray zone[t.site.dim];
    int dw=0,cluster=0;
    corrtype wrap[t.site.dim]={0};
    int i=0,ax=0,p=0,r=0,ww=0,bd;
    for(ax=0;ax<t.site.dim;ax++){
        init(zone[i]);//init to 0
    }
    for(i=0;i<t.site.stride[0];i++){
        if (t.site.head[i]>0){//have an unvisited site
            for(ax=0;ax<t.site.dim;ax++){
                zone[ax].head[i]=0;//init to 0
                wrap[ax]=0;
            }
            ww=0;//记录总的有几个方向wrap
            append(q,i);
            while(q.length>0){
                p=popleft(q);
                t.site.head[p]=POPOUT;
                for(ax=-t.site.dim;ax<t.site.dim;ax++){
                    r=rollindex(t.site,p,ax,dw);
                    if(t.type==SITE) bd=t.site.head[r];
                    else{//bond
                        if(ax>0) bd=t.bond[ax].head[p];
                        else bd=t.bond[ax].head[r];
                    }
                    if(bd){
                        if (t.site.head[r]>0){
                            for(int tmp=0;tmp<t.site.dim;tmp++)
                                zone[tmp].head[r]=zone[tmp].head[p];
                            zone[ax].head[r]+=dw;
                            t.site.head[r]=INQUENE;
                            append(q,r);
                        }
                        else if(t.site.head[r]==INQUENE){
                            zone[ax].head[p]+=dw;//for comparation, will cancel it soon
                            for(int tmp=0;tmp<t.site.dim;tmp++){
                                if(wrap[tmp]) continue;
                                if(zone[tmp].head[r] != zone[tmp].head[p]){
                                    wrap[tmp]=1;
                                    ww+=1;
                                }
                            }
                            zone[ax].head[p]-=dw;//Canceled!
                        }
                    }
                }
            }
            if(ww>0){
                for(int tmp=0;tmp<t.site.dim;tmp++)
                    printf("%d",wrap[tmp]);
            }
        }
    }
}
