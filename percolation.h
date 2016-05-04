#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include "16807.h"
#include "ndarray.h"
#include "zonebond.h"
#include "singlelist.h"
/**
 * @file classify.h
 * @author zpj
 * @brief Some code for bond classification in percolation
 * @todo Remember bond type without using other method like backtracing tree?
 * Obsolete the bondtype matrix
 */

using namespace std;

template<uint D>
/**
 * @brief The torus class for bond classification
 */
class ctorus{
    ndarray<uint8_t> cumleaf;     ///< Cumulated leafs in relay
    ndarray<uint16_t> cumbfree;   ///< Cumulated non-bridges in relay
    ndarray<int16_t> time;      ///< Time for BFS
    ndarray<int> father;        ///< Father node for backtracing
    ndarray<int8_t> fatherax;   ///< The axis from which the father come
    uint tmp;
public:
    int maxclus;                ///< The Biggest Cluster
    int countclus;              ///< The Total Cluster
    int maxlfree;               ///< The biggest leaf-free Cluster
    int countlfree;             ///< The Total leaf-free Cluster
    int maxbfree;               ///< The biggest bridge-free cluster
    int countbfree;             ///< The Total bridge-free cluster
    ndarray<nbond<D>> bonds;    ///< Rember the bonds by means of list
//    ndarray<uint8_t> type[D];   ///< Rember the type for each bond by matrix
    ctorus(int width){
        bonds=ndarray<nbond<D>>(D, width);
//        for(uint i=0;i<D;i++){
//            type[i]=ndarray<int8_t>(D, width);
//        }
        cumleaf=ndarray<uint8_t>(bonds);
        cumbfree=ndarray<uint16_t>(bonds);
        time=ndarray<int16_t>(bonds);
        father=ndarray<int>(bonds);
        fatherax=ndarray<int8_t>(bonds);
    }
    /**
     * @brief set bonds and initialize the variables
     * @param prob  probility for setting bond
     *
     * Junction is the default value for a bond.
     */
    void setbond(double prob){
        int near;
//        for(uint i=0;i<D;i++) type[i]=empty;
        maxclus=maxlfree=maxbfree=countclus=countlfree=countbfree=0;
        cumleaf=0;
        cumbfree=0;
        time=0;
        for(int curr=0;curr<bonds.size();curr++){
            bonds[curr].clear();
        }
        for(int curr=0;curr<bonds.size();curr++){
            for(uint ax=0; ax<D; ax++){
                if(myrand()<prob){
                    near=bonds.rollindex(curr, ax);
//                    type[ax][curr]=junction;
                    countclus++;
                    bonds[curr].append(ax);
                    bonds[near].append(ax-D);
                }
            }
        }
    }
//    /**
//     * @brief Set bond type
//     * @param curr  current point
//     * @param dest  destination
//     * @param ax    axis to go
//     * @param t     type
//     */
//    inline void settype(int curr, int dest, int8_t ax, bondtype t){
//        if(ax>=0) type[ax][curr]=t;
//        else type[ax+D][dest]=t;
//    }
    /**
     * @brief Prune all the leaves recursively
     *
     * BTW: Get the maxbranch as the init value of maxclus.
     */
    void prune(){
        int start, fat;
        int8_t ax;
        for(int i=0;i<bonds.size();i++){
            start=i;
            while(bonds[start].size==1){//加在里面是四分之一的情况要做
                ax=bonds[start][0];
                fat=bonds.rollind(start, ax);
//                settype(start, fat, ax, branch);
                cumleaf[fat]+=1+cumleaf[start];
                if(cumleaf[fat]>maxclus) maxclus=cumleaf[fat];
                bonds[start].clear();
                bonds[fat].finddelrev(ax);
                start=fat;
            }
        }
    }
    /**
     * @brief Decend to a lower level closer to root
     * @param a     current point
     *
     * Mark the path as non-bridge if it was not yet marked
     * After prune, and even deletion of bonds, the degree root>1, so no
     * need to add judgement for root!
     * Set a circ counter to remember
     *
     * For every diverging point, must stop!
     */
    void descend(int &a){
        int son=a;
        do{
            if(fatherax[a]!=marked){
                son=a;
                a=father[a];
//                settype(a, son, fatherax[son], nonbrg);
                tmp++;
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
     * + For performance, after backtracing, it will set new father
     * for all relays!
     * + Download all non-bridges in relays to the root relay
     * + The stack will reserve a fixed size to reduce the consumption
     * of reallocating memory
     */
    void backtrace(int a, int b){
        static vector<int> s;
        if(s.size()>64) s.resize(64);
        do{
            if(time[a]>=time[b]){
                s.push_back(a);
                descend(a);
            }
            else{
                s.push_back(b);
                descend(b);
            }
        }while(a!=b);
        while(fatherax[a]==marked){
            s.push_back(a);
            a=father[a];
        }
        countbfree+=tmp;
        cumbfree[a]+=tmp;
        for(uint i=0;i<s.size();i++){
            father[s[i]]=a;
            cumbfree[a]+=cumbfree[s[i]];
            cumbfree[s[i]]=0;
        }
        if(maxbfree<cumbfree[a]) maxbfree=cumbfree[a];
        s.clear();
    }

    /**
     * @brief Identify junctions and then delete it
     *
     * A semi-destructive process:
     * + Will turn the undirected graph into directed
     */
    void dejunct(){
        quene<int> q;
        int curr, near, ax, currclus, currbfree;
        for(int i=0;i<bonds.size();i++){
            if ((time[i]==0) && bonds[i].size){
                time[i]++;
                currclus=cumleaf[i];
                fatherax[i]=0;
                currbfree=0;
                q.append(i);
                while(q.notempty()){
                    curr=q.pop();
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        near=bonds.rollind(curr,ax);
                        bonds[near].finddelrev(ax);
                        currbfree++;
                        currclus+=cumleaf[near]+1;
                        if(time[near]==0){
                            time[near]=time[curr]+1;
                            q.append(near);
                            father[near]=curr;
                            fatherax[near]=ax;
                        }
                        else{
//                            settype(curr, near, ax, nonbrg);
                            tmp=1;
                            backtrace(near, curr);
                        }
                    }
                }
                if(maxlfree<currbfree) maxlfree=currbfree;
                if(maxclus<currclus) maxclus=currclus;
                countlfree+=currbfree;
            }
        }
    }
    /**
     * @brief Get the number for each bond type
     */
    void bondcount(){
        cout<<bonds.size()*D<<'\t';
        cout<<countclus<<'\t'<<countlfree<<'\t'<<countbfree<<'\t';
        cout<<maxclus<<'\t'<<maxlfree<<'\t'<<maxbfree<<endl;
    }
};

template<int D>
/**
 * @brief wrapping torus
 * + Use `zone` to save zone information to judge wrapping
 */
class wtorus{
    int sign(int x){return (x>=0)?1:-1;}
public:
    ndarray<nbond<D>> bonds;
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
        int fat, leaf;
        int8_t ax;
        for(int i=0;i<bonds.size();i++){
            leaf=i;
            while(bonds[leaf].size==1){
                ax=bonds[leaf][0];
                fat=bonds.rollind(leaf, ax);
                bonds[leaf].clear();
                bonds[fat].finddelrev(ax);
                leaf=fat;
            }
        }
    }
    int wrapping(){
        quene<int> q;
        ndarray<uint8_t> visit(bonds);
        ndarray<zone<D>> z(bonds);
        typename zone<D>::intD_t dz;
        int curr, near, count=0;
        int8_t delta, ax, absax;
#ifndef WRAPONCE
            prune();
#endif
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
#ifdef WRAPONCE
                                    return 1;
#else
                                    count++;
#endif
                            }
                            z[curr][absax]-=delta;
                        }
                    }
                }
            }
        }
        return count;
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
