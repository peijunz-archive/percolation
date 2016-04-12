#ifndef nditer_H
#define nditer_H

/**
 * @brief Iterator of sub-index for an ndarray
 */
class nditer{
public:
    /**
     * @brief An array restoring current sub-index
     */
    int *ind;
//    nditer():dim(0),shape(NULL),ind(NULL){}
    nditer(int d, int *sh);
    ~nditer();
    bool next();
    void print();
private:
    int dim;
    int *shape;
};
/**
 * @param d     dimension
 * @param sh    shape
 */
nditer::nditer(int d, int *sh){
    dim=d;
    shape=sh;
    ind=new int[d];
    for(int i=0;i<d;i++){
        ind[i]=0;
    }
}
nditer::~nditer(){
    delete [] ind;
}
/**
 * @brief Go one step further for corresponding raw index
 * @return If the iterator reached the end or not
 */
bool nditer::next(){
    int i=dim-1;
    do{
        if(ind[i]>=shape[i]-1){
            ind[i]=0;
            i--;
        }
        else{
            ind[i]+=1;
            return true;
        }
    }while(i>=0);
    for(i=0;i<dim;i++){
        ind[i]=0;
    }
    return false;
}
/**
 * @brief Print out current sub-index array
 */
void nditer::print(){
    for(int i=0;i<dim;i++)
        printf("%4d ", ind[i]);
    putchar('\n');
}

#endif
