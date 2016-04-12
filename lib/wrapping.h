#include <stdio.h>
#include <stdlib.h>
#include "16807.h"
#include "ndarray.h"
#include "singlelist.h"

/**
 * @file wrapping.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo 确定数据结构
 *
 * 由于是三维情形，所以可以直接用邻接矩阵存储这个图！
 *
 */

const int inquene=-1;
const int popout=-2;

inline int rev(int ax){
    return -1-ax;
}
void wrapping(){//For bond ONLY
    quene<int> q;
    ndarray<char> zone[site.dim];
    int delta, wrap[site.dim];
    int i, point=0,near=0, wrapcount, ax, absax, tmpax;
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
            while(q.notempty()){
                point=q.pop();
                site[point]=popout;
                while(nears[point].notempty()){
                    ax=nears[point].pop();
                    near=site.rollindex(point,ax);// TODO delta
                    absax=(ax>=0)?ax:(-1-ax);
                    if((2*ax+1)*(near-point)<0) delta=(ax>=0)?1:-1;
                    else delta=0;
                    if (site[near]>0){
                        for(tmpax=0;tmpax<site.dim;tmpax++)
                            zone[tmpax][near]=zone[tmpax][point];
                        zone[absax][near]+=delta;
                        q.append(near);
                        site[near]=inquene;
                    }
                    else if(site[near]==inquene){
                        zone[absax][point]+=delta;//will cancel it soon
                        for(int tmpax=0;tmpax<site.dim;tmpax++){
                            if(wrap[tmpax]) continue;
                            if(zone[tmpax][near] != zone[tmpax][point]){
                                wrap[tmpax]=1;
                                wrapcount+=1;
                            }
                        }
                        zone[absax][point]-=delta;//Canceled!
                    }
                }
            }
            if(wrapcount>0){
                printf("Wrapping Status: ");
                for(tmpax=0;tmpax<site.dim;tmpax++)
                    printf("%d ",wrap[tmpax]);
                putchar('\n');
            }
        }
    }
}
