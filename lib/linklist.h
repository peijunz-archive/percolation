#ifndef LINKLIST_H
#define LINKLIST_H

template <typename C>
class node{
public:
    node *next;
    C data;
    node():next(0){}
    ~node();
}

template <typename dtype, typename ltype>
class llist{
public:
    dtype data;
    node<ltype> *next;
}

#endif //LINKLIST_H
