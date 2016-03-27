#ifndef GRAPH_H
#define GRAPH_H

#include "ndarray.h"
#include "linklist.h"
#define BOND true
#define SITE false

class torus{
public:
    ndarray <char> site;
    ndarray<llist<char>> nears;
    torus(int dim, int width):site(dim,width), nears(dim,width){}
    torus(int dim, int width, double prob);
    void reset(double prob, bool clr=true);
    void print();
    void wrapping();
};

#endif // GRAPH_H
