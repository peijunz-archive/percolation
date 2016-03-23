#ifndef DEQUE_H
#define DEQUE_H

typedef int elemtype;//save raw index

class node;
typedef node* link;
class node{
public:
    elemtype elem;
    link next;
    link before;
    node():next(NULL), before(NULL){}
};

class quene{
public:
    int length;
    link tail;
    link head;
    quene():length(0),tail(NULL),head(NULL){}
    elemtype pop();
    elemtype popleft();
    void append(elemtype elem);
};


#endif
