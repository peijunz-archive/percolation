#ifndef GRAPH_H
#define GRAPH_H

#define BOND true
#define SITE false

typedef unsigned char corrtype;

struct torus{
    ndarray site;
    ndarray *bond;
};

void inittorus(torus &t, int dim, int width, int type=BOND);//OK

void destroytorus(torus &t);

void setbond(torus &t, double prob);

void setsite(torus &t, double prob);

void wrapping(torus &t);

void printtorus(torus &t);//OK

#endif // GRAPH_H
