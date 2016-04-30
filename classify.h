#include <iostream>
#define NDEBUG
#include <cassert>
#include <vector>
#include <cstdint>
#include <opencv/cv.hpp>
#include "16807.h"
#include "ndarray.h"
#include "nbond.h"
#include "singlelist.h"
/**
 * @file classify.h
 * @author zpj
 * @brief Some code for bond classification in percolation
 * @todo Count max size for each path threshold
 */

using namespace std;
using namespace cv;

enum bondtype:uint8_t{
    empty=0,    ///< No bond here
    branch=1,   ///< Branch, identify by recursive cutting leaf
    junction=2, ///< Junction, default value for any bond
    nonbrg=3,   ///< A bond without which the cluster is still connected
    marked=64   ///< Mark to replace axis if a point is backtraced.
};

/// Default color scheme for bond
uint8_t tocolor[4]={0, 64, 255, 128};

template<unsigned int D>
/**
 * @brief The low dimensional torus class for D<=4
 *
 * + For low dimensional torus, the bonds can be saved by nbond
 *   whose size is only 2D+1 byte
 */
class ltorus{
    ndarray<nbond<D>> bonds;    ///< Rember the bonds by means of list
    ndarray<uint8_t> type[D];   ///< Rember the type for each bond by matrix
    ndarray<int16_t> time;      ///< time for BFS
    ndarray<int> father;        ///< Father node for backtracing
    ndarray<int8_t> fatherax;   ///< The axis from which the father come
    int count[4];
public:
    ltorus(int width){
        bonds=ndarray<nbond<D>>(D, width);
        for(int i=0;i<D;i++){
            type[i]=ndarray<int8_t>(D, width);
        }
    }
    /**
     * @brief set bonds and initialize the variables
     * @param prob  probility for setting bond
     *
     * Junction is the default value for a bond
     */
    void setbond(double prob){
        int near;
        count[0]=0;
        for(int i=0;i<D;i++) type[i]=empty;
        for(int curr=0;curr<bonds.size();curr++){
            bonds[curr].clear();
        }
        for(int curr=0;curr<bonds.size();curr++){
            for(int ax=0; ax<D; ax++){
                if(myrand()<prob){
                    near=bonds.rollindex(curr, ax);
                    type[ax][curr]=junction;
                    count[0]++;
                    bonds[curr].append(ax);
                    bonds[near].append(ax-D);
                }
            }
        }
    }
    /**
     * @brief Set bond type
     * @param curr  current point
     * @param dest  destination
     * @param ax    axis to go
     * @param t     type
     */
    inline void settype(int curr, int dest, int8_t ax, bondtype t){
        if(ax>=0) type[ax][curr]=t;
        else type[ax+D][dest]=t;
    }
    /**
     * @brief Prune all the leaves recursively
     */
    void prune(){
        int start, father;
        int8_t ax;
        count[branch]=0;
        for(int i=0;i<bonds.size();i++){
            start=i;
            while(bonds[start].size==1){
                ax=bonds[start][0];
                father=bonds.rollind(start, ax);
                settype(start, father, ax, branch);
                count[branch]++;
                bonds[start].clear();
                bonds[father].finddelrev(ax);
                start=father;
            }
        }
    }
    /**
     * @brief Decend to a lower level closer to root
     * @param a     current point
     *
     * Mark the path as non-bridge if it was not yet marked
     */
    void decend(int &a){
        int son=a;
        do{
            if(fatherax[a]!=marked){
                son=a;
                a=father[a];
                settype(a, son, fatherax[son], nonbrg);
                count[nonbrg]++;
                fatherax[son]=marked;
            }
            else{
                a=father[a];
            }
        }while(bonds[a].size==1);
    }

    /**
     * @brief backtrace to the highest root for both a and b
     * @param a leaf 1
     * @param b leaf 2
     *
     * For performance, after backtracing, it will set
     * new father for all relays!
     */
    void backtrace(int a, int b){
        vector<int> sa, sb;
        do{
            if(time[a]>=time[b]){
                sa.push_back(a);
                decend(a);
            }
            else{
                sb.push_back(b);
                decend(b);
            }
        }while(a!=b);
        for(unsigned int i=0;i<sa.size();i++){
            father[sa[i]]=a;
        }
        for(unsigned int i=0;i<sb.size();i++){
            father[sb[i]]=a;
        }
    }

    /**
     * @brief Identify junctions and then delete it
     *
     * A semi-destructive process:
     * + Will turn the undirected graph into directed
     */
    void dejunct(){
        quene<int> q;
        time=ndarray<int16_t>(bonds);
        father=ndarray<int>(bonds);
        fatherax=ndarray<int8_t>(bonds);
        time=-1;
        int curr, near, ax;
        count[nonbrg]=0;
        for(int i=0;i<bonds.size();i++){
            if ((time[i]<0) && bonds[i].size){
                time[i]=0;
                q.append(i);
                while(q.notempty()){
                    curr=q.pop();
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        near=bonds.rollind(curr,ax);
                        bonds[near].finddelrev(ax);
                        if(time[near]<0){
                            time[near]=time[curr]+1;
                            q.append(near);
                            father[near]=curr;
                            fatherax[near]=ax;
                        }
                        else{
                            settype(curr, near, ax, nonbrg);
                            count[nonbrg]++;
                            backtrace(near, curr);
                        }
                    }
                }
            }
        }
        count[junction]=count[0]-count[branch]-count[nonbrg];
    }
    /**
     * @brief Get the number for each bond type
     */
    void bondcount(){
        cout<<bonds.size()*D;
        for(int i=0; i<4; i++){
            cout<<"\t"<<count[i];
        }
        cout<<endl;
    }

    /**
     * @brief Count the max cluster if only take bonds larger than threshold
     * into consideration
     * @param th    threshold, should be empty, branch, junction
     * @return  The size of the biggest cluster
     */
    int maxclus(bondtype th);
    /**
     * @brief Save 2D percolation with bond classification to image using openCV
     * @param filename
     * @param col   Color scheme for bondtype
     */
    template<unsigned int L>
    void savetoimg(string filename, uint8_t *col=tocolor){
        int width=bonds.shape(0), sf=L/2, W=width*L;
        Mat M;
        assert(L>1);
        assert(D==2);
        ndarray<uint8_t> g(D, W);
        g=0;
        for(int i=0;i<width;i++){
            for(int j=0;j<width;j++){
                g(L*i+sf,L*j+sf)=63;
                for(int ax=0;ax<D;ax++){
                    for(int l=1;l<L;l++){
                        g((L*i+l*(1-ax)+sf)%W,(L*j+l*ax+sf)%W)\
                                =col[type[ax](i,j)];
                    }
                }
            }
        }
        M=Mat(width*L, width*L, CV_8UC1, g.head);
        imwrite(filename ,M);
    }
};
