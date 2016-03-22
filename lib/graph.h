#ifndef GRAPH_H
#define GRAPH_H
typedef char corrtype;
struct torus{
    ndarray site;
    ndarray *bond;
};
void inittorus(torus &t, int dim, int width);//OK
void destroytorus(torus &t);
void setbond(torus &t, double prob=0.5);//OK
void setsite(torus &t, double prob=0.5);//OK
void wrapping(torus &t);
void printtorus(torus &t);//OK
#endif // GRAPH_H
