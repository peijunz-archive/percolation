#ifndef DEQUE_H
#define DEQUE_H

typedef ndarray elemtype;
struct quene{
    int length;
    link tail;
    link tail;
};
void initq(quene &q);
void append(quene &q, elemtype &elem);
elemtype pop(quene &q);
elemtype popl(quene &q);
#endif
