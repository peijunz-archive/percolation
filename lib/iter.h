#ifndef ITER_H
#define ITER_H

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

#endif
