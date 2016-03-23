#ifndef GRAPH_H
#define GRAPH_H

#define BOND true
#define SITE false

class torus{
    bool type;
public:
    ndarray<char> site;
    ndarray<char> *bond;
    torus(int dim, int width, double prob, int tp);
    ~torus();
    void print();
    void wrapping();
};

#endif // GRAPH_H
