#include <cstdio>
#include <cmath>
#include "percolation.h"
#include "save2dimg.h"
using namespace std;
typedef uint64_t sum_t;
/**
 * @brief The means struct for save/calculating mean/std
 */
struct means {
    uint64_t s1;
    double s2;
    int n;
    means(): s1(0), s2(0), n(0) {}
    void add(int d) {
        s1 += d;
        s2 += pow(d, 2);
        n++;
    }
    double mean1() {
        return s1 / (double)n;
    }
    double mean2() {
        return s2 / (double)n;
    }
    double std() {
        return sqrt(mean2() - pow(mean1(), 2));
    }
    double err() {
        return std() / sqrt(n);
    }
    void print(const char *name){
        printf("%-24s%-12ld%-12.3e%-16.6e%-12.3e\n",
               name, s1, std(), mean1(), err());
    }
    static void header(){
        char line[]="--------";
        char fmt[]="%-24s%-12s%-12s%-16s%-12s\n";
        printf(fmt, line, line, line, line, line);
        printf(fmt, "Name", "Sum(x)", "Std(x)", "Mean(x)", "Err4Mean");
        printf(fmt, line, line, line, line, line);
    }
};
template <uint D>
/**
 * @brief Generate and analyse data
 * @param width
 * @param times
 * @param prob
 *
 * 不断的重复统计对应的物理量。
 * 每个计算机每天有计算量q0=f*t=1.8GHz*86400=1.55e5 G
 * 100个两天有计算量q=200*q0=4.32e7 G
 * Speed:
 * +3D, p=0.24881182, v=2.5GHz, t=26s, n=10 => 6.5G计算量每次
 * 故每台机器可以重复2e4次，总共重复6e6次
 */
void gendata(int width, int times, double prob) {
    means leaf, junc, circ, maxclus, maxlfree, maxbfree, maxleaf;
    ctorus<D> t(width);
    printf(">>> Dim=%d, width=%d, times=%d, prob=%f\n", D, width, times, prob);
    means::header();
    for(int i = 0; i < times; i++) {
        t.setbond(prob);
        t.prune();
        t.dejunct();
        leaf.add(t.countclus - t.countlfree);
        junc.add(t.countlfree - t.countbfree);
        circ.add(t.countbfree);
        maxclus.add(t.maxclus);
        maxlfree.add(t.maxlfree);
        maxbfree.add(t.maxbfree);
        maxleaf.add(t.maxleaf);
    }
    leaf.print("Leaf-Total");
    junc.print("Junction-Total");
    circ.print("Circuit-Total");
    maxclus.print("Max-Clus");
    maxlfree.print("Max-Leaf-Free-Clus");
    maxbfree.print("Max-Branch-Free-Clus");
    maxleaf.print("Max-Leaf-Clus");
}
template <bool dense=false>
void savesample(int width, double prob){
    ctorus<2> t(width);
    t.setbond(prob);
    savenlist(t, "original.png");
    if(dense){
        t.connect();
        savenlist(t, "densed.png");
    }
    t.prune();
    savenlist(t, "pruned.png");
}

int main() {
    savesample<true>(24, 0.48);
//    vector<double> probl={0.1, 0.15, 0.2, 0.22, 0.235, 0.24881182,
//        0.26, 0.28, 0.33, 0.4, 0.55, 0.7, 0.9, 0.99};
    vector<int> L={32, 45, 64, 92};
    int nmin=10;//10000/Day, 7/min, 420/h;
    for(uint i =0; i<L.size();i++){
        gendata<2>(L[i], L[L.size()-1]*nmin/L[i], 0.24881182);
    }
//    for(uint i=0;i<probl.size();i++){
//        gendata<3>(128, 40, probl[i]);
//    }
    return 0;
}
