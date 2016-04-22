#include <iostream>
#include <cassert>
#include <vector>
#include <cstdint>
#include "16807.h"
#include "ndarray.h"
#include "singlelist.h"

using namespace std;
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
};

enum visit{unvisited, inquene, popout};
inline int sign(int x){
    return (x>=0)?1:-1;
}
/**
 * @brief The combined chars union for small char arrays
 *
 * + An char array
 * + Assignment/Comparation can be parallelize.
 */
union combc{
    int8_t c[4];
    int32_t a;
    combc():a(0){}
    int8_t & operator[](int i){return c[i];}
    int32_t & operator=(const int b){return (a=b);}
    int32_t & operator=(const combc com){a=com.a; return a;}
    bool operator==(const combc com){return a==com.a;}
    bool operator!=(const combc com){return a!=com.a;}
};

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
    vector<int> wclus;
public:
    ltorus(int width){
        assert(D<=4 && D>1);
        bonds=ndarray<nbond<D>>(D, width);
    }
    void setbond(double prob){
        int near;
        wclus.clear();
        if(wclus.capacity()>10){
            wclus.shrink_to_fit();
        }
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
    bool wrapping(){
        quene<int> q;                           //quene for BFS
        //status for visit, must init
        ndarray<char> status(bonds);
        status=unvisited;
        //zone for wrapping judgement. Auto init
        ndarray<combc> zone(bonds);
        //Wrapping status for Cluster/Torus
        bool cwrap=false, twrap=false;
        int delta, curr, near, ax, absax;
        for(int i=0;i<bonds.size();i++){
            if ((status[i]==unvisited) && bonds[i].size){
                zone[i]=0;
                q.append(i);
                cwrap=false;
                while(q.notempty()){
                    curr=q.pop();
                    status[curr]=popout;
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        near=bonds.rollindex(curr,ax);
                        if(ax>=0){delta=1;absax=ax;}
                        else{delta=-1;absax=ax+D;}
                        if(delta==sign(near-curr)) delta=0;
                        if(status[near]==unvisited){
                            zone[near]=zone[curr];//
                            zone[near][absax]+=delta;//
                            q.append(near);
                            status[near]=inquene;
                        }
                        else if(status[near]==inquene && !cwrap){
                            zone[curr][absax]+=delta;
                            if(zone[near]!=zone[curr]){
                                wclus.push_back(i);
                                cwrap=true;
                                twrap=true;
                            }
                            zone[absax][curr]-=delta;//Canceled!
                        }
                    }
                }
            }
        }
        return twrap;
    }
};
