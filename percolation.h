#include <iostream>
#include <cassert>
#include <vector>
#include <cstdint>
#include <cmath>
#include <set>
#include <opencv/cv.hpp>
#include "16807.h"
#include "ndarray.h"
#include "singlelist.h"
using namespace std;
using namespace cv;
/**
 * @file percolation.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo Data structure for graph?
 */
template<int N>
/**
 * @brief The near bond struct for N-Dimensional lattice
 *
 * + Absorb data effeciently
 * + No check for overflowing
 */
struct nbond{
    unsigned char size;
    char c[2*N];
    nbond():size(0){}
    char & operator[](int i){return c[i];}
    void append(char x){
        assert(size<2*N);
        c[size++]=x;
    }
    void clear(){size=0;}
    /**
     * @brief delete a bond at a position
     * @param pos   position
     */
    void del(int pos){
        for(int i=pos+1;i<size;i++){
            c[i-1]=c[i];
        }
        size--;
    }
    /**
     * @brief find a bond of value ax and then delete
     * @param ax
     * @return found or not
     */
    bool fdel(char ax){
        for(int i=0;i<size;i++){
            if(c[i]==ax){
                del(i);
                return true;
            }
        }
        return false;
    }
};

const int unvisited=-1;
inline int sign(int x){
    return (x>=0)?1:-1;
}
inline char rev(char ax, int D){
    return (ax<0)?ax+D:ax-D;
}
template<int D>
/**
 * @brief The combined chars union for small char arrays
 *
 * + An char array
 * + Assignment/Comparation can be parallelize.
 */
union combc{
    int8_t c[D];
    int32_t a;
    combc():a(0){assert((D<=4)&&(D>1));}
    combc(int32_t init):a(init){assert((D<=4)&&(D>1));}
    int8_t & operator[](int i){return c[i];}
    int32_t & operator=(const int b){return (a=b);}
    int32_t & operator=(const combc com){a=com.a; return a;}
    bool operator==(const combc com){return a==com.a;}
    bool operator!=(const combc com){return a!=com.a;}
    int32_t operator-(combc &rhs){
        combc tmp;
        int sig=0, t;
        tmp=0;
        for(int i=0;i<D;i++){
            if(sig){
                tmp[i]=sig*(c[i]-rhs[i]);
            }
            else{
                t=c[i]-rhs[i];
                if(t>0){
                    sig=1;
                    tmp[i]=t;
                }
                else if(t<0){
                    sig=-1;
                    tmp[i]=-t;
                }
            }
        }
        return tmp.a;
    }
    void print(){
        cout<<"(";
        for(int i=0;i<D;i++){
            cout<<(int)c[i]<<", ";
        }
        cout<<"\b\b) \n";
    }
};

template<int D>
void printd(int32_t a){
    combc<D> tmp(a);
    tmp.print();
}

template<int D>
/**
 * @brief The low dimensional torus class for D<=4
 *
 * + For low dimensional torus, the bonds can be saved by nbond
 *   whose size is only 2D+1 byte
 * + Use `combc` to save time judging wrapping
 */
class ltorus{
    ndarray<nbond<D>> bonds;
public:
    set<int32_t> wraps;
    ltorus(int width){
        assert(D<=4 && D>1);
        bonds=ndarray<nbond<D>>(D, width);
    }
    void setbond(double prob){
        int near;
        wraps.clear();
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
    int deleaf(int leaf){
        int father;
        char ax;
        while(bonds[leaf].size==1){
            ax=bonds[leaf][0];
            father=bonds.rollind(leaf, ax, D);
            bonds[leaf].clear();
            bonds[father].fdel(rev(ax, D));
            leaf=father;
        }
        return leaf;
    }
    void prune(){
        for(int i=0;i<bonds.size();i++){
            deleaf(i);
        }
    }

    bool wrapping(){
        quene<int> q;                           //quene for BFS
        //status for visit, must init
        ndarray<int16_t> time(bonds);
        time=-1;//-1 for unvisited
        //zone for wrapping judgement. Auto init
        ndarray<combc<D>> zone(bonds);
        //Wrapping status for Cluster/Torus
        bool twrap=false;
        int32_t dz;
        int delta, curr, near, ax, absax;
        for(int i=0;i<bonds.size();i++){
            if ((time[i]==unvisited) && bonds[i].size){
                zone[i]=0;
                time[i]=0;
                q.append(i);
                while(q.notempty()){
                    curr=q.pop();
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        if(ax>=0){delta=1;absax=ax;}
                        else{delta=-1;absax=ax+D;}
                        near=bonds.rollindex(curr,absax,delta);
                        if(delta==sign(near-curr)) delta=0;
                        if(time[near]==unvisited){
                            zone[near]=zone[curr];
                            zone[near][absax]+=delta;
                            time[near]=time[curr]+1;
                            q.append(near);
                        }
                        else if(time[near]>=time[curr]){
                            zone[curr][absax]+=delta;
                            dz=zone[near]-zone[curr];
                            if((dz==0)||wraps.count(dz)){
                                bonds[curr].del(ii);
                                bonds[near].fdel(rev(ax, D));
                                ii--;
                                deleaf(curr);
                                deleaf(near);
                            }
                            else{
                                cout<<"-----------------------\n";
                                cout<<">>> From:\n";
                                bonds.printind(curr);
                                zone[curr].print();
                                cout<<">>> To:\n";
                                bonds.printind(near);
                                zone[near].print();
                                wraps.insert(dz);
                                cout<<">>> Wrapping:\n";
                                printd<D>(dz);
                                twrap=true;
                            }
                            zone[curr][absax]-=delta;
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
        assert(bonds.size()<1024*1024);
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
