#ifndef nditer_H
#define nditer_H
#include <iostream>
using namespace std;
/**
 * @file nditer.h
 * @author zpj
 * @brief Iterator of sub-index for an ndarray
 * @bug No
 */
/// Iterator of sub-index for an ndarray
class nditer{
public:
    /// An array restoring current sub-index
    int *ind;
    /// Raw index
    int raw;
    nditer(int d, int *sh);
    /// Destructor
    ~nditer();
    bool next();
    /// Print out current sub-index array
    void print();
private:
    int dim;    ///< Dimension
    int *shape; ///< Max Bound
};
/**
 * @brief Constructor
 * @param d     dimension
 * @param sh    shape
 */
nditer::nditer(int d, int *sh){
    dim=d;
    shape=sh;
    ind=new int[d+1];
    for(int i=0;i<d;i++){
        ind[i]=0;
    }
    ind[dim]=-1;
    raw=-1;
}
nditer::~nditer(){
    delete [] ind;
}
/**
 * @brief Go one step further for corresponding raw index
 * @return If the iterator reached the end or not
 */
bool nditer::next(){
    int j;
    raw++;
    ind[dim]+=1;
    for(j=dim-1;ind[j+1]==shape[j];j--){
        ind[j+1]=0;
        ind[j]+=1;
    }
    return !ind[0];
//    int i=dim-1;
//    do{
//        if(ind[i]>=shape[i]-1){
//            ind[i]=0;
//            i--;
//        }
//        else{
//            ind[i]+=1;
//            return true;
//        }
//    }while(i>=0);
//    return false;
}
void nditer::print(){
    cout<<raw<<'\t';
    for(int i=0;i<dim+1;i++)
        cout<<ind[i]<<" ";
    cout<<endl;
}

#endif
