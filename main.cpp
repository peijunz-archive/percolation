#include <cstdio>
#include <cmath>
#include "percolation.h"
using namespace std;
typedef uint64_t sum_t;
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

int gendata() {
    int width;
    int times;
    means leaf, junc, circ, maxclus, maxlfree, maxbfree, maxleaf;
    double prob;
    printf("D=2, Input\twidth\ttimes\tprob\n");
    scanf("%d%d%lf", &width, &times, &prob);
    printf("Parameters:\t%d\t%d\t%f\n", width, times, prob);
    ctorus<2> t(width);
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
    return 0;
}

int main() {
    gendata();
    return 0;
}
