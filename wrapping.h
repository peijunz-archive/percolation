#include <iostream>
#define NDEBUG
#include <cassert>
#include <cstdint>
#include <set>
#include <opencv/cv.hpp>
#include "16807.h"
#include "ndarray.h"
#include "singlelist.h"
#include "nbond.h"
#include "zone.h"
using namespace std;
using namespace cv;
/**
 * @file percolation.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo Data structure for graph?
 */
inline int sign(int x){
    return (x>=0)?1:-1;
}

template<int D>
/**
 * @brief The low dimensional torus class for D<=4
 *
 * + For low dimensional torus, the bonds can be saved by nbond
 *   whose size is only 2D+1 byte
 * + Use `combc` to save time judging wrapping
 */
class wtorus{
    ndarray<nbond<D>> bonds;
public:
    wtorus(int width){
        assert(D<=4 && D>1);
        bonds=ndarray<nbond<D>>(D, width);
    }
    void setbond(double prob){
        int near;
        for(int curr=0;curr<bonds.size();curr++){
            bonds[curr].clear();
        }
        for(int curr=0;curr<bonds.size();curr++){
            for(int ax=0; ax<D; ax++){
                if(myrand()<prob){
                    near=bonds.rollindex(curr, ax);
                    bonds[curr].append(ax);
                    bonds[near].append(ax-D);
                }
            }
        }
    }
    void prune(){
        int father, leaf;
        char ax;
        for(int i=0;i<bonds.size();i++){
            leaf=i;
            while(bonds[leaf].size==1){
                ax=bonds[leaf][0];
                father=bonds.rollind(leaf, ax);
                bonds[leaf].clear();
                bonds[father].finddelrev(rev(ax, D));
                leaf=father;
            }
        }
    }
    bool wrapping(){
        quene<int> q;
        ndarray<uint8_t> visit(bonds);
        ndarray<zone<D>> z(bonds);
        zone<D> dz;
        int curr, near;
        int8_t delta, ax, absax;
        visit=0;   // 0 for unvisited
        for(int i=0;i<bonds.size();i++){
            if ((time[i]==unvisited) && bonds[i].size){
                z[i]=0;
                visit[i]=1;
                q.append(i);
                while(q.notempty()){
                    curr=q.pop();
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        if(ax>=0){delta=1;absax=ax;}
                        else{delta=-1;absax=ax+D;}
                        near=bonds.rollindex(curr,absax,delta);
                        if(delta==sign(near-curr)) delta=0;
                        bonds[near].finddelrev(ax);
                        if(!visit[near]){
                            z[near]=z[curr];
                            z[near][absax]+=delta;
                            visit[near]=1;
                            q.append(near);
                        }
                        else{
                            z[curr][absax]+=delta;
                            dz=z[near]-z[curr];
                            if(dz.a!=0){
                                ii--;
                            }
                            z[curr][absax]-=delta;
                        }
                    }
                }
            }
        }
        return twrap;
    }
    /**
     * @brief save 2D matrix to image
     * @param filename
     * @param bdlen length of bond
     *
     * Save 2D percolation to image using openCV
     */
    template<int L>
    void savetoimg(string s){
        int width=bonds.shape(0), ax, sf=L/2, W=width*L;
        Mat M;
        assert(L>1);
        assert(D==2);
        ndarray<unsigned char> g(D, W);
        g=0;
        for(int i=0;i<width;i++){
            for(int j=0;j<width;j++){
                g(L*i+sf,L*j+sf)+=bonds(i,j).size*63;
                for(int k=0;k<bonds(i,j).size;k++){
                    ax=bonds(i,j)[k];
                    if(ax<0) continue;
                    for(int l=1;l<L;l++){
                        g((L*i+l*(1-ax)+sf)%W,(L*j+l*ax+sf)%W)=63;
                    }
                }
            }
        }
        M=Mat(width*L, width*L, CV_8UC1, g.head);
        imwrite(s,M);
    }
};
