#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>

template <typename S> class node{
public:
    S elem;
    node* next;
    node* before;
    node():next(NULL), before(NULL){}
};

template <typename elemtype> class quene{
public:
    int length;
    node<elemtype>* tail;
    node<elemtype>* head;
    quene():length(0),tail(NULL),head(NULL){}
    ~quene();
    elemtype pop();
    elemtype popleft();
    void append(elemtype elem);
};
template <typename elemtype>
quene<elemtype>::~quene(){
    while(length>0){
        pop();
    }
}

template <typename elemtype>
elemtype quene<elemtype>::pop(){
    elemtype elem;
    node<elemtype>* tmp=NULL;
    if(length==0)
        exit;
    if (length==1)
        tail=NULL;
    elem=head->elem;
    tmp=head;
    head=tmp->next;
    delete tmp;
    if (length!=1)
        head->before=NULL;
    if(length)
        length--;
    return elem;
}

template <typename elemtype>
elemtype quene<elemtype>::popleft(){//Copy of pop(), just replace head<-->tail, before<-->next
    elemtype elem;
    node<elemtype>* tmp=NULL;
    if(length==0)
        exit;
    if (length==1)
        head=NULL;
    elem=tail->elem;
    tmp=tail;
    tail=tmp->before;
    delete tmp;
    if (length!=1)
        tail->next=NULL;
    if(length)
        length--;
    return elem;
}

template <typename elemtype>
void quene<elemtype>::append(elemtype elem){
    node<elemtype>* p=new node<elemtype>;
    p->elem=elem;
    if (head==0){
        head=tail=p;
    }
    else{
        p->next=head;
        head=head->before=p;
    }
    length++;
}

#endif
