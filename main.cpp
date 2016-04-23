#include <iostream>
#include "percolation.h"
#include <cmath>
ltorus<2> t(6);
/**
 * @brief wrapping probability for a setting
 * @param p     probility for bond setting
 * @param n     time of iteration
 * @return wrapping probability
 */
double wrapprob(double p, int n=100){
    double count=0;
    for(int i=0;i<n;i++){
        t.setbond(p);
        count+=t.wrapping();
    }
    return count/n;
}
/**
 * @brief bisectional method for finding a setting for given wrapping prob
 * @param n     time of iteration
 * @param y0    the given wrapping prob
 * @return bond setting
 */
double bisecp(int n, double y0=0.5){
    double xl=0, xr=1, xc=0, err=1/sqrt(n);
    double yc=0;
    while(fabs(yc-y0)>err){
        xc=(xl+xr)/2;
        yc=wrapprob(xc, n);
        if(yc>y0){
            xr=xc;
        }
        else{
            xl=xc;
        }
        cout<<"x="<<xc<<", Delta="<<yc-y0<<endl;
    }
    return xc;
}
/**
 * @brief find the prob for multiple wrapping clusters
 * @param p     bond setting
 * @param n     sampling time
 * @return prob for multiple wrapping
 */
double findmulti(double p, int n=10){
    double s=0;
    for(int i=0;i<n;i++){
        t.setbond(p);
        t.wrapping();
        if(t.wclus.size()>1) s++;
    }
    return s/n;
}
int main(){
    setseed();
//    bisecp(10000);
//    findmulti(0.486328, 1000);
    cout<<findmulti(0.457031, 100000)<<endl;
    return 0;
}
