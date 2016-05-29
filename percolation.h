#include <cstdio>
#include <vector>
#include <cstdint>
#include <cmath>
#include <random>
#include <queue>
#define NDEBUG
#include <cassert>
#include "ndarray.h"
#include "zonebond.h"
random_device tmp;
mt19937 myrand(tmp());
#define COLOR
/**
 * @file percolation.h
 * @author zpj
 * @brief Some code for bond classification in percolation
 * @todo Remember bond type without using other method like backtracing tree?
 * Obsolete the bondtype matrix
 */
uint nbacktrace=0, ndescend=0;
using namespace std;

template<uint D>
/**
 * @brief The torus class for bond classification
 */
class ctorus {
    ndarray<uint16_t, D> cumleaf;     ///< Cumulated leafs in relay
    ndarray<uint32_t, D> cumbfree;   ///< Cumulated non-bridges in relay
    ndarray<uint16_t, D> time;      ///< Time for BFS
    ndarray<uint, D> father;        ///< Father node for backtracing
    ndarray<int8_t, D> fatherax;   ///< The axis from which the father come
    uint tmp;
  public:
    uint maxleaf;                ///< The Biggest Branch
    uint maxclus;                ///< The Biggest Cluster
    uint countclus;              ///< The Total Cluster
    uint maxlfree;               ///< The biggest leaf-free Cluster
    uint countlfree;             ///< The Total leaf-free Cluster
    uint maxbfree;               ///< The biggest bridge-free cluster
    uint countbfree;             ///< The Total bridge-free cluster
    ndarray<nbond<D>, D> bonds;     ///< Rember the bonds by means of list
    ctorus(uint width) {
        bonds = ndarray<nbond<D>, D>(width);
        cumleaf = ndarray<uint16_t, D>(bonds);
        cumbfree = ndarray<uint32_t, D>(bonds);
        time = ndarray<uint16_t, D>(bonds);
        father = ndarray<uint, D>(bonds);
        fatherax = ndarray<int8_t, D>(bonds);
    }
    /**
     * @brief set bonds and initialize the variables
     * @param prob  probility for setting bond
     *
     * Junction is the default value for a bond.
     */
    void setbond(double prob) {
        uint near;
        maxleaf = maxclus = maxlfree = maxbfree = 0;
        countclus = countlfree = countbfree = 0;
        cumleaf = 0;
        cumbfree = 0;
        time = 0;
        for(uint curr = 0; curr < bonds.size(); curr++) {
            bonds[curr].clear();
        }
        for(uint curr = 0; curr < bonds.size(); curr++) {
            for(uint ax = 0; ax < D; ax++) {
                if(myrand() < prob * myrand.max()) {
                    near = bonds.rollindex(curr, ax);
                    countclus++;
                    bonds[curr].append(ax);
                    bonds[near].append(ax - D);
                }
            }
        }
    }
    /**
     * @brief Maximize connected components of the cluster without adding any site,
     * should be changed to only connected ones
     */
    void connect() {
        queue<uint> q;
        uint curr, near, ccol = 0;
        ndarray<uint, D> color(bonds);
        color = 0;  //zero is grey, unvisited
        for(uint i = 0; i < bonds.size(); i++) {
            if((color[i] == 0) && bonds[i].size) {
                color[i] = ++ccol;
                q.push(i);
                while(!q.empty()) {
                    curr = q.front();
                    for(uint ii = 0; ii < bonds[curr].size; ii++) {
                        near = color.rollind(curr, bonds[curr][ii]);
                        if(color[near] == 0) {
                            color[near] = ccol;
                            q.push(near);
                        }
                    }
                    q.pop();
                }
            }
        }
        for(uint i = 0; i < bonds.size(); i++) {
            bonds[i].clear();
        }
        for(uint curr = 0; curr < bonds.size(); curr++) {
            if(color[curr]) {
                for(uint ax = 0; ax < D; ax++) {
                    near = bonds.rollindex(curr, ax);
                    if(color[near] == color[curr]) {
                        bonds[curr].append(ax);
                        bonds[near].append(ax - D);
                    }
                }
            }
        }
    }

    /**
     * @brief Prune all the leaves recursively
     *
     * BTW: Get the maxbranch as the init value of maxclus.
     */
    void prune() {
        uint start, fat;
        int8_t ax;
        for(uint i = 0; i < bonds.size(); i++) {
            start = i;
            while(bonds[start].size == 1) {
                ax = bonds[start][0];
                fat = bonds.rollind(start, ax);
                cumleaf[fat] += 1 + cumleaf[start];
                if(cumleaf[fat] > maxleaf) maxleaf = cumleaf[fat];
                bonds[start].clear();
                bonds[fat].finddelrev(ax);
                start = fat;
            }
        }
        maxclus = maxleaf;
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
    void descend(uint &a) {
        uint son = a;
        do {
            if(fatherax[a] != marked) {
                son = a;
                a = father[a];
                tmp++;
                fatherax[son] = marked;
            } else {
                ndescend++;
                a = father[a];
            }
        } while(bonds[a].size == 1);
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
    void backtrace(uint a, uint b) {
        static vector<uint> s;
        do {
            if(time[a] >= time[b]) {
                s.push_back(a);
                descend(a);
            } else {
                s.push_back(b);
                descend(b);
            }
        } while(a != b);
        while(fatherax[a] == marked) {
            s.push_back(a);
            a = father[a];
        }
        countbfree += tmp;
        cumbfree[a] += tmp;
        for(uint i = 0; i < s.size(); i++) {
            father[s[i]] = a;
            cumbfree[a] += cumbfree[s[i]];
            assert(cumbfree[a] < 60000);
            cumbfree[s[i]] = 0;
        }
        nbacktrace++;
        if(maxbfree < cumbfree[a]) maxbfree = cumbfree[a];
        s.clear();
    }
    /**
     * @brief Identify junctions
     *
     * A semi-destructive process:
     * + Will turn the undirected graph into directed? Necessary?
     * + Use time to decide which to backtrace
     */
    void dejunct() {
        queue<uint> q;
        uint curr, near;
        int8_t ax;
        uint currclus, currlfree;
        for(uint i = 0; i < bonds.size(); i++) {
            if((time[i] == 0) && bonds[i].size) {
                time[i]++;
                fatherax[i] = 0;
                currclus = 0;
                currlfree = 0;
                q.push(i);
                while(!q.empty()) {
                    curr = q.front();
                    currclus += cumleaf[curr];
                    for(uint ii = 0; ii < bonds[curr].size; ii++) {
                        currlfree++;
                        currclus++;
                        ax = bonds[curr][ii];
                        near = bonds.rollind(curr, ax);
                        bonds[near].finddelrev(ax);
                        if(time[near] == 0) {
                            time[near] = time[curr] + 1;
                            assert(time[near] < 60000);
                            q.push(near);
                            father[near] = curr;
                            fatherax[near] = ax;
                        } else {
                            tmp = 1;
                            backtrace(near, curr);
                        }
                    }
                    q.pop();
                }
                if(maxlfree < currlfree) maxlfree = currlfree;
                if(maxclus < currclus) maxclus = currclus;
                countlfree += currlfree;
            }
        }
    }
    /// Get the number for each bond type
    void bondcount() {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n", countclus, countclus - countlfree,
               countlfree - countbfree, countbfree, maxclus, maxlfree, maxbfree, maxleaf);
    }
    template<unsigned int L>
    friend void savemat(ctorus<2> &t, string fname, uint8_t *col);
    template<unsigned int L, bool undirected>
    friend void savenlist(ctorus<2> &t, string fname);
};

template<int D>
/**
 * @brief wrapping torus
 * + Use `zone` to save zone information to judge wrapping
 */
class wtorus {
    int sign(int x) {
        return (x >= 0) ? 1 : -1;
    }
  public:
    ndarray<nbond<D>, D> bonds;
    wtorus(int width) {
        bonds = ndarray<nbond<D>, D>(D, width);
    }
    void setbond(double prob) {
        int near;
        for(int curr = 0; curr < bonds.size(); curr++) {
            bonds[curr].clear();
        }
        for(int curr = 0; curr < bonds.size(); curr++) {
            for(int ax = 0; ax < D; ax++) {
                if(myrand() < prob * myrand.max()) {
                    near = bonds.rollindex(curr, ax);
                    bonds[curr].append(ax);
                    bonds[near].append(ax - D);
                }
            }
        }
    }
    void prune() {
        int fat, leaf;
        int8_t ax;
        for(int i = 0; i < bonds.size(); i++) {
            leaf = i;
            while(bonds[leaf].size == 1) {
                ax = bonds[leaf][0];
                fat = bonds.rollind(leaf, ax);
                bonds[leaf].clear();
                bonds[fat].finddelrev(ax);
                leaf = fat;
            }
        }
    }
    int wrapping() {
        queue<int> q;
        ndarray<uint8_t, D> visit(bonds);
        ndarray<zone<D>, D> z(bonds);
        typename zone<D>::intD_t dz;
        int curr, near, count = 0;
        int8_t delta, ax, absax;
#ifndef WRAPONCE
        prune();
#endif
        visit = 0; // 0 for unvisited
        for(int i = 0; i < bonds.size(); i++) {
            if(!visit[i] && bonds[i].size) {
                z[i] = 0;
                visit[i] = 1;
                q.push(i);
                while(!q.empty()) {
                    curr = q.front();
                    for(int ii = 0; ii < bonds[curr].size; ii++) {
                        ax = bonds[curr][ii];
                        if(ax >= 0) {
                            delta = 1;
                            absax = ax;
                        } else {
                            delta = -1;
                            absax = ax + D;
                        }
                        near = bonds.rollindex(curr, absax, delta);
                        if(delta == sign(near - curr)) delta = 0;
                        bonds[near].finddelrev(ax);
                        if(!visit[near]) {
                            z[near] = z[curr];
                            z[near][absax] += delta;
                            visit[near] = 1;
                            q.push(near);
                        } else {
                            z[curr][absax] += delta;
                            dz = z[near] - z[curr];
                            if(dz != 0) {
#ifdef WRAPONCE
                                return 1;
#else
                                count++;
#endif
                            }
                            z[curr][absax] -= delta;
                        }
                    }
                    q.pop();
                }
            }
        }
        return count;
    }
};
template<int D>
/**
 * @brief wrapping probability for a setting
 * @param t     Torus
 * @param p     probility for bond setting
 * @param n     time of iteration
 * @return wrapping probability
 */
double wrapprob(wtorus<D> &t, double p, int n = 100) {
    double count = 0;
    for(int i = 0; i < n; i++) {
        t.setbond(p);
        count += t.wrapping();
    }
    return count / n;
}
template<int D>
/**
 * @brief bisectional method for finding a setting for given wrapping prob
 * @param t     Torus
 * @param n     time of iteration
 * @param y0    the given wrapping prob
 * @return bond setting
 */
double bisecp(wtorus<D> &t, int n, double y0 = 0.5) {
    double xl = 0, xr = 1, xc = 0, err = 1 / sqrt(n);
    double yc = 0;
    while(fabs(yc - y0) > err) {
        xc = (xl + xr) / 2;
        yc = wrapprob(t, xc, n);
        if(yc > y0) xr = xc;
        else xl = xc;
        printf("x=%f\tdy=%f\n", xc, yc - y0);
    }
    return xc;
}
