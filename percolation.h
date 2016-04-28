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
/**
 * @file percolation.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo
 * + Delete some nonbridge/junctions to simplify the graph
 * + Add BFS function for count max size for each threshold
 */

using namespace std;
using namespace cv;
inline int sign(int x){return (x>=0)?1:-1;}

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
    bool finddelrev(char ax){
        if(ax<0) ax+=N;
        else ax-=N;
        for(int i=0;i<size;i++){
            if(c[i]==ax){
                del(i);
                return true;
            }
        }
        return false;
    }
};

const int unvisited=-100;
const int visited=100;
/**
 * @brief The bondtype enum
 *
 * junction is the default value for a bond
 */
enum bondtype:unsigned char{empty, branch, junction, nonbrg};
template<int D>
/**
 * @brief The low dimensional torus class for D<=4
 *
 * + For low dimensional torus, the bonds can be saved by nbond
 *   whose size is only 2D+1 byte
 * + Use `combc` to save time judging wrapping
 */
class ltorus{
    ndarray<nbond<D>> bonds;///< Rember the bonds by means of near linked list
    ndarray<unsigned char> type[D];  ///< Rember the type for each bond
    ndarray<int16_t> time;
    ndarray<int> father;
    ndarray<char> fatherax;
public:
    ltorus(int width){
        assert(D<=4 && D>1);
        bonds=ndarray<nbond<D>>(D, width);
        for(int i=0;i<D;i++){
            type[i]=ndarray<char>(D, width);
        }
    }
    void setbond(double prob){
        int near;
        for(int i=0;i<D;i++) type[i]=empty;
        for(int curr=0;curr<bonds.size();curr++){
            bonds[curr].clear();
        }
        for(int curr=0;curr<bonds.size();curr++){
            for(int ax=0; ax<D; ax++){
                if(myrand()<prob){
                    near=bonds.rollindex(curr, ax);
                    type[ax][curr]=junction;
                    bonds[curr].append(ax);
                    bonds[near].append(ax-D);
                }
            }
        }
    }
    inline void setbdtype(int curr, int dest, char ax, bondtype t){
        if(ax>=0) type[ax][curr]=t;
        else type[ax+D][dest]=t;
    }
    /**
     * @brief Prune all the leaves recursively
     */
    void prune(){
        int start, father;
        char ax;
        for(int i=0;i<bonds.size();i++){
            start=i;
            while(bonds[start].size==1){
                ax=bonds[start][0];
                father=bonds.rollind(start, ax);
                setbdtype(start, father, ax, branch);
                bonds[start].clear();
                bonds[father].finddelrev(ax);
                cout<<start<<" to "<<father<<endl;
                start=father;
            }
        }
    }

    void goroot(int &a){
        int son=a;
        do{
            if(fatherax[a]!=visited){
                son=a;
                a=father[a];
                setbdtype(a, son, fatherax[son], nonbrg);
                fatherax[son]=visited;
            }
            else{
                a=father[a];
            }
        }while(bonds[a].size==1);
    }

    /**
     * @brief backtrace to the highest root for a and b
     * @param l1 leaf 1
     * @param l2 leaf 2
     *
     * For performance, after backtracing, it will set new root
     */
    void backtrace(int a, int b){
        vector<int> sa, sb;
        while(a!=b){
            if(time[a]>=time[b]){
                sa.push_back(a);
                goroot(a);
            }
            else{
                sb.push_back(b);
                goroot(b);
            }
        }
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
        quene<int> q;                           //quene for BFS
        time=ndarray<int16_t>(bonds);
        father=ndarray<int>(bonds);
        fatherax=ndarray<char>(bonds);
        time=unvisited;
        int curr, near, ax;
        for(int i=0;i<bonds.size();i++){
            if ((time[i]==unvisited) && bonds[i].size){
                time[i]=0;
                father[i]=unvisited;
                q.append(i);
                while(q.notempty()){
                    curr=q.pop();
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        near=bonds.rollind(curr,ax);
                        bonds[near].finddelrev(ax);
                        if(time[near]==unvisited){
                            time[near]=time[curr]+1;
                            q.append(near);
                            father[near]=curr;
                        }
                        else{
                            setbdtype(curr, near, ax, nonbrg);
                            backtrace(near, curr);
                        }
                    }
                }
            }
        }
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
        ndarray<unsigned char> g(D, W);
        g=0;
        for(int i=0;i<width;i++){
            for(int j=0;j<width;j++){
                g(L*i+sf,L*j+sf)=85;
                for(int ax=0;ax<D;ax++){
                    for(int l=1;l<L;l++){
                        g((L*i+l*(1-ax)+sf)%W,(L*j+l*ax+sf)%W)=type[ax](i,j)*85;
                    }
                }
            }
        }
        M=Mat(width*L, width*L, CV_8UC1, g.head);
        imwrite(s,M);
    }
};
