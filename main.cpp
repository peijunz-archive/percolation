#include <iostream>
#include "percolation.h"
using namespace std;
int main(){
    int width=10;
    int times;
    double prob;
    setseed();
    cout<<"D=2, Input\twidth\ttimes\tprob\n";
    cin>>width>>times>>prob;
    cout<<"Parameters:\t"<<width<<'\t'<<times<<'\t'<<prob<<endl;
    ctorus<2> t(width);
    cout<<"键\t枝\t结\t环\t最大丛\t去枝丛\t去桥丛\n";
    for(int i=0;i<times;i++){
        t.setbond(prob);
        t.prune();
        t.dejunct();
        t.bondcount();
    }
    return 0;
}
