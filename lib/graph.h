#ifndef GRAPH_H
#define GRAPH_H

#define BOND true
#define SITE false

class torus{
    bool type;
public:
    ndarray site;
    ndarray *bond;
    torus(int dim, int width, double prob, int tp);
    ~torus();
    void print();
    void wrapping();
};

#endif // GRAPH_H
