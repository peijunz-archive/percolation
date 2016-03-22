#ifndef GRAPH_H
#define GRAPH_H
typedef char corrtype;
struct torus{
    ndarray site;
    ndarray *bond;
};
void inittorus(torus &t, int dim, int width);
void destroytorus(torus &t);
void setbond(torus &t, double prob);
void setsite(torus &t, double prob);
void wrapping(torus &t);

#endif // GRAPH_H
