#include<iostream>
class iter{
public:
    int dim;
    int *shape;
    int *ind;
    iter():dim(0),shape(NULL),ind(NULL){}
    iter(int d, int *sh);
    ~iter();
    bool next();
    void print();
};
iter::iter(int d, int *sh){
    dim=d;
    shape=sh;//So should not change sh!!!
    ind=new int[d];
    for(int i=0;i<d;i++){
        ind[i]=0;
    }
}
iter::~iter(){
    delete [] ind;
}

bool iter::next(){
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
void iter::print(){
    for(int i=0;i<dim;i++)
        printf("%4d ", ind[i]);
    putchar('\n');
}
