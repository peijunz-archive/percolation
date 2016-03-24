#include <stdio.h>
#include <stdlib.h>
#include "16807.h"
#include "ndarray.h"
#include "deque.h"
#include "linklist.h"

//External Macros
#define BOND true
#define SITE false

//Local Macros
#define LINE "---------------------------\n"
#define EMPTY 0
#define INQUENE -1
#define POPOUT -2
//#define CLUSTER
//#define ZONE
class torus{
    bool type;
public:
    ndarray<char> site;
    ndarray<llist<char>> nears;
    torus(int dim, int width, double prob, int tp);
//    ~torus();
    void print();
    void wrapping();
};
inline int rev(int ax){
    return -1-ax;
}

torus::torus(int dim, int width, double prob, int tp):
    site(dim,width), nears(dim,width), type(tp){
    int ax, i, near;
    if(tp==SITE){//待定，由于找邻居Site也需要大量循环
        randomize(site,prob);
    }
    else{
        homogenize(site);//修改顺序以方便对比
        for(ax=0;ax<site.dim;ax++){
            for(i=0;i<site.size();i++){
                if(distribute(prob)){
                    near=site.rollindex(i,ax);
                    site.head[near]+=1;
                    nears.head[near].addsnode(rev(ax));
                    nears.head[i].addsnode(ax);
                    site.head[i]+=1;
                }
            }
        }
    }
}

//torus::~torus(){
//    if(type==BOND)
//        delete [] bond;
//}

void torus::print(){
    snode<char>* ptr;
    printf("%sSite:\n%s",LINE,LINE);
    site.print();
    if(type==SITE)
        return;
    for(int i=0;i<site.size();i++){
        printf("Site %d: ", i);
        ptr=nears.head[i].head;
        while(ptr){
            printf("%3d",ptr->data);
            ptr=ptr->next;
        }
        putchar('\n');
    }
}

void torus::wrapping(){//For bond ONLY
    quene<int> q;
    ndarray<char> zone[site.dim];
    snode<char> *ptr=0;
    int delta, wrap[site.dim];
    int i, point=0,near=0, wrapcount, ax, absax, tmpax;
#ifdef CLUSTER
    int cluster=-1;
#endif
    for(ax=0;ax<site.dim;ax++){
        zone[ax].reset(site.dim, site.shape);
        homogenize(zone[ax]);//init zone to 0
    }
    for(i=0;i<site.size();i++){
        if (site.head[i]>0){//unvisited site
            for(ax=0;ax<site.dim;ax++){//初始化起点的区为{0,0,...,0}
                zone[ax].head[i]=0;//init to 0
                wrap[ax]=0;
            }
            wrapcount=0;//记录有几个方向wrap
            q.append(i);
#ifdef CLUSTER
            cluster-=1;
#endif
#ifdef CLUSTER
            printf("%sNew cluster:%d\n%s",LINE,i,LINE);
#endif
            while(q.length>0){
                point=q.popleft();
#ifdef CLUSTER
                printf("%-4d",point);
                site.head[point]=cluster;
#else
                site.head[point]=POPOUT;
#endif
                ptr=nears.head[point].head;
                while(ptr){
                    ax=ptr->data;
                    ptr=ptr->next;
                    near=site.rollindex(point,ax);// TODO delta
                    absax=(ax>=0)?ax:(-1-ax);
                    if((2*ax+1)*(near-point)<0) delta=(ax>=0)?1:-1;
                    else delta=0;
                    if (site.head[near]>0){
                        for(tmpax=0;tmpax<site.dim;tmpax++)
                            zone[tmpax].head[near]=zone[tmpax].head[point];
                        zone[absax].head[near]+=delta;
                        q.append(near);
                        site.head[near]=INQUENE;
                    }
                    else if(site.head[near]==INQUENE){
                        zone[absax].head[point]+=delta;//will cancel it soon
                        for(int tmpax=0;tmpax<site.dim;tmpax++){
                            if(wrap[tmpax]) continue;
                            if(zone[tmpax].head[near] != zone[tmpax].head[point]){
                                wrap[tmpax]=1;
                                wrapcount+=1;
#ifdef ZONE
                                printf("Ax:%d\tJudgement: %d %d, Delta:%d\n",tmpax,point,near,delta);
                                printf("Site:\n");
                                site.print();
                                printf("Zone:\n");
                                zone[0].print();
                                printf("Zone:\n");
                                zone[1].print();
#endif
                            }
                        }
                        zone[absax].head[point]-=delta;//Canceled!
                    }
                }
            }

#ifdef CLUSTER
            putchar('\n');
#endif
            if(wrapcount>0){
                printf("Wrapping Status: ");
                for(tmpax=0;tmpax<site.dim;tmpax++)
                    printf("%d ",wrap[tmpax]);
                putchar('\n');
            }
        }
    }
#ifdef CLUSTER
    for(i=0;i<site.size();i++){
        if(site.head[i]<0)
            site.head[i]+=1;
            site.head[i]*=-1;
    }
    site.print();
#endif
}
