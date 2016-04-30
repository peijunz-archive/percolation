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
 * @file wrapping.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo Data structure for graph?
 */
inline int sign(int x){
    return (x>=0)?1:-1;
}

template<int D>
/**
 * @brief wrapping torus
 * + Use `zone` to save zone information to judge wrapping
 */
class wtorus{
    ndarray<nbond<D>> bonds;
public:
    wtorus(int width){
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
                bonds[father].finddelrev(ax);
                leaf=father;
            }
        }
    }
    bool wrapping(){
        quene<int> q;
        ndarray<uint8_t> visit(bonds);
        ndarray<zone<D>> z(bonds);
        typename zone<D>::intD_t dz;
        int curr, near;
        int8_t delta, ax, absax;
        visit=0;   // 0 for unvisited
        for(int i=0;i<bonds.size();i++){
            if (!visit[i] && bonds[i].size){
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
                            if(dz!=0){
                                return true;
                            }
                            z[curr][absax]-=delta;
                        }
                    }
                }
            }
        }
        return false;
    }
    template<unsigned int L>
    /**
     * @brief save 2D matrix to image
     * @param filename
     *
     * Save 2D percolation to image using openCV
     */
    void savetoimg(string filename){
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
        imwrite(filename,M);
    }
};
template<int D>
/**
 * @brief wrapping probability for a setting
 * @param p     probility for bond setting
 * @param n     time of iteration
 * @return wrapping probability
 */
double wrapprob(wtorus<D> &t, double p, int n=100){
    double count=0;
    for(int i=0;i<n;i++){
        t.setbond(p);
        count+=t.wrapping();
    }
    return count/n;
}
template<int D>
/**
 * @brief bisectional method for finding a setting for given wrapping prob
 * @param n     time of iteration
 * @param y0    the given wrapping prob
 * @return bond setting
 */
double bisecp(wtorus<D> &t, int n, double y0=0.5){
    double xl=0, xr=1, xc=0, err=1/sqrt(n);
    double yc=0;
    while(fabs(yc-y0)>err){
        xc=(xl+xr)/2;
        yc=wrapprob(t, xc, n);
        if(yc>y0) xr=xc;
        else xl=xc;
        cout<<"x="<<xc<<"\tdy="<<yc-y0<<endl;
    }
    return xc;
}
