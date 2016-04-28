#include <iostream>
#define NDEBUG
#include <cassert>
#include <vector>
#include <cstdint>
#include <cmath>
#include <set>
#include <opencv/cv.hpp>
#include "16807.h"
#include "ndarray.h"
#include "singlelist.h"
/**
 * @file percolation.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo
 * + Merge the wrapping branch
 * + Count max size for each path threshold
 */

using namespace std;
using namespace cv;

template<int D>
/**
 * @brief The near bond struct for N-Dimensional lattice
 *
 * + Absorb data effeciently
 * + No check for overflowing
 */
struct nbond{
    uint8_t size;
    /// Container
    int8_t c[2*D];
    nbond():size(0){}
    int8_t & operator[](int i){return c[i];}
    void append(int8_t x){
        assert(size<2*D);
        c[size++]=x;
    }
    void clear(){size=0;}
    /**
     * @brief delete a bond at a known position
     * @param pos   position
     */
    void del(int pos){
        for(int i=pos+1;i<size;i++){
            c[i-1]=c[i];
        }
        size--;
    }
    /**
     * @brief Find a reverse bond of ax and then delete it
     * @param ax
     * @return found or not
     */
    bool finddelrev(int8_t ax){
        if(ax<0) ax+=D;
        else ax-=D;
        for(int i=0;i<size;i++){
            if(c[i]==ax){
                del(i);
                return true;
            }
        }
        return false;
    }
};

/**
 * @brief The bondtype enum
 *
 * Junction is the default value for a bond
 */
enum bondtype:uint8_t{empty=0, branch=1, junction=2, nonbrg=3, marked=64};
uint8_t tocolor[4]={0, 64, 255, 128};

template<int D>
/**
 * @brief The low dimensional torus class for D<=4
 *
 * + For low dimensional torus, the bonds can be saved by nbond
 *   whose size is only 2D+1 byte
 */
class ltorus{
    ndarray<nbond<D>> bonds;///< Rember the bonds by means of near linked list
    ndarray<uint8_t> type[D];  ///< Rember the type for each bond
    /**
     * @brief time for BFS
     *
     * + -1 for unvisited site.
     * + important for backtracing
     */
    ndarray<int16_t> time;
    ndarray<int> father;    ///< Father node for backtracing
    ndarray<int8_t> fatherax;   ///< The axis from which the father come
    int numbranch;
    int numjunct;
    int numnonbrg;
    int numbond;
public:
    ltorus(int width){
        assert(D<=4 && D>1);
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
        numbond=numbranch=numnonbrg=numjunct=0;
        for(int i=0;i<D;i++) type[i]=empty;
        for(int curr=0;curr<bonds.size();curr++){
            bonds[curr].clear();
        }
        for(int curr=0;curr<bonds.size();curr++){
            for(int ax=0; ax<D; ax++){
                if(myrand()<prob){
                    near=bonds.rollindex(curr, ax);
                    type[ax][curr]=junction;
                    numbond++;
                    bonds[curr].append(ax);
                    bonds[near].append(ax-D);
                }
            }
        }
    }
    /**
     * @brief set bond type
     * @param curr  current point
     * @param dest  destination
     * @param ax    axis to go
     * @param t     type
     */
    inline void setbdtype(int curr, int dest, int8_t ax, bondtype t){
        if(ax>=0) type[ax][curr]=t;
        else type[ax+D][dest]=t;
    }
    /**
     * @brief Prune all the leaves recursively
     */
    void prune(){
        int start, father;
        int8_t ax;
        for(int i=0;i<bonds.size();i++){
            start=i;
            while(bonds[start].size==1){
                ax=bonds[start][0];
                father=bonds.rollind(start, ax);
                setbdtype(start, father, ax, branch);
                numbranch++;
                bonds[start].clear();
                bonds[father].finddelrev(ax);
                start=father;
            }
        }
    }
    /**
     * @brief go to a higher root and mark the path if unmarked
     * @param a     current point
     */
    void goroot(int &a){
        int son=a;
        do{
            if(fatherax[a]!=marked){
                son=a;
                a=father[a];
                setbdtype(a, son, fatherax[son], nonbrg);
                numnonbrg++;
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
     * For performance, after backtracing, it will set new root!
     */
    void backtrace(int a, int b){
        vector<int> sa, sb;
        do{
            if(time[a]>=time[b]){
                sa.push_back(a);
                goroot(a);
            }
            else{
                sb.push_back(b);
                goroot(b);
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
     * + Will turn the undirected graph into directed graph
     */
    void dejunct(){
        quene<int> q;
        time=ndarray<int16_t>(bonds);
        father=ndarray<int>(bonds);
        fatherax=ndarray<int8_t>(bonds);
        time=-1;
        int curr, near, ax;
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
                            setbdtype(curr, near, ax, nonbrg);
                            numnonbrg++;
                            backtrace(near, curr);
                        }
                    }
                }
            }
        }
        numjunct=numbond-numbranch-numnonbrg;
    }
    void getratio(){
        double S=bonds.size()*D;
        cout<<"总\t"<<bonds.size()*D<<"\t"<<1<<endl;
        cout<<"键\t"<<numbond<<"\t"<<numbond/S<<endl;
        cout<<"枝\t"<<numbranch<<"\t"<<numbranch/S<<endl;
        cout<<"结\t"<<numjunct<<"\t"<<numjunct/S<<endl;
        cout<<"环\t"<<numnonbrg<<"\t"<<numnonbrg/S<<endl;
    }

    /**
     * @brief Count the max cluster if only take bonds larger than th into
     *  consideration
     * @param th    threshold, should be empty, branch, junction
     * @return  the size of the biggest cluster
     */
    int count(bondtype th);
    /**
     * @brief save 2D matrix to image
     * @param filename
     * @param bdlen length of bond
     *
     * Save 2D percolation to image using openCV
     */
    template<int L>
    void savetoimg(string s){
        int width=bonds.shape(0), sf=L/2, W=width*L;
        Mat M;
        assert(L>1);
        assert(D==2);
        assert(bonds.size()<1024*1024);
        ndarray<uint8_t> g(D, W);
        g=0;
        for(int i=0;i<width;i++){
            for(int j=0;j<width;j++){
                g(L*i+sf,L*j+sf)=63;
                for(int ax=0;ax<D;ax++){
                    for(int l=1;l<L;l++){
                        g((L*i+l*(1-ax)+sf)%W,(L*j+l*ax+sf)%W)\
                                =tocolor[type[ax](i,j)];
                    }
                }
            }
        }
        M=Mat(width*L, width*L, CV_8UC1, g.head);
        imwrite(s,M);
    }
};
