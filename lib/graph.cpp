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
public:
    ndarray<char> site;
    ndarray<llist<char>> nears;
    torus(int dim, int width):site(dim,width), nears(dim,width){}
    torus(int dim, int width, double prob);
    void reset(double prob, bool clr);
//    ~torus();
    void print();
    void wrapping();
};
inline int rev(int ax){
    return -1-ax;
}
void torus::reset(double prob, bool clr=true){
    int ax, i, near;
    homogenize(site);//修改顺序以方便对比
    if(clr){
        for(i=0;i<site.size();i++){
            nears[i].clear();
        }
    }
    for(ax=0;ax<site.dim;ax++){
        for(i=0;i<site.size();i++){
            if(rand()<prob){
                near=site.rollindex(i,ax);
                site[near]+=1;
                nears[near].addsnode(rev(ax));
                nears[i].addsnode(ax);
                site[i]+=1;
            }
        }
    }
}

torus::torus(int dim, int width, double prob):
    site(dim,width), nears(dim,width){
    reset(prob,false);
}

void torus::print(){
    snode<char>* ptr;
    printf("%sSite:\n%s",LINE,LINE);
    site.print();
    for(int i=0;i<site.size();i++){
        printf("Site %d: ", i);
        ptr=nears[i].head;
        while(ptr){
            printf("%3d",ptr->data);
            ptr=ptr->next;
        }
        putchar('\n');
    }
}

template <typename T>
void randomize(ndarray<T> &a, double prob){
    for(int i=0;i<a.stride[0];i++){
        if(myrand()<prob)
            a[i]=1;
        else
            a[i]=0;
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
        zone[ax].set(site.dim, site.shape);
        homogenize(zone[ax]);//init zone to 0
    }
    for(i=0;i<site.size();i++){
        if (site[i]>0){//unvisited site
            for(ax=0;ax<site.dim;ax++){//初始化起点的区为{0,0,...,0}
                zone[ax][i]=0;//init to 0
                wrap[ax]=0;
            }
            wrapcount=0;//记录有几个方向wrap
            q.append(i);
#ifdef CLUSTER
            cluster-=1;
            printf("%sNew cluster:%d\n%s",LINE,i,LINE);
#endif
            while(q.length>0){
                point=q.popleft();
#ifdef CLUSTER
                printf("%-4d",point);
                site[point]=cluster;
#else
                site[point]=POPOUT;
#endif
                ptr=nears[point].head;
                while(ptr){
                    ax=ptr->data;
                    ptr=ptr->next;
                    near=site.rollindex(point,ax);// TODO delta
                    absax=(ax>=0)?ax:(-1-ax);
                    if((2*ax+1)*(near-point)<0) delta=(ax>=0)?1:-1;
                    else delta=0;
                    if (site[near]>0){
                        for(tmpax=0;tmpax<site.dim;tmpax++)
                            zone[tmpax][near]=zone[tmpax][point];
                        zone[absax][near]+=delta;
                        q.append(near);
                        site[near]=INQUENE;
                    }
                    else if(site[near]==INQUENE){
                        zone[absax][point]+=delta;//will cancel it soon
                        for(int tmpax=0;tmpax<site.dim;tmpax++){
                            if(wrap[tmpax]) continue;
                            if(zone[tmpax][near] != zone[tmpax][point]){
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
                        zone[absax][point]-=delta;//Canceled!
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
        if(site[i]<0)
            site[i]+=1;
            site[i]*=-1;
    }
    site.print();
#endif
}
