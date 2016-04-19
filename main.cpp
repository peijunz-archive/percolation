#include <iostream>
#include "ndarray.h"
#include "16807.h"
#include "singlelist.h"

//1458816173 Site Percolation有问题
inline int f(int &i,int &j){
    return 1000*i+j;
}

int main(){
    ndarray<int> b{1000,1000};
    int a[1000][1000];
    int ind[2];
    int &i=ind[0], &j=ind[1];
    for(int k=0;k<500;k++){
        for(int t=0;t<1000000;t++){
            i=t/1000;
            j=t%1000;
            b[t]=i*i+j*j-i*j;
        }
    }
//    for(int k=0;k<500;k++){
//        for(i=0;i<1000;i++){
//            for(j=0;j<1000;j++){
////                b(i,j)=i*i+j*j-i*j;
//                b[f(i,j)]=i*i+j*j-i*j;
//            }
//        }
//    }
//    for(int k=0;k<500;k++){
//        for(i=0;i<1000;i++){
//            for(j=0;j<1000;j++){
//                a[i][j]=i*i+j*j-i*j;
//            }
//        }
//    }
    cout<<a[100][100]<<endl<<b(100,100)<<endl;
    return 0;
}
