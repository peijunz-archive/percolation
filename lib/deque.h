#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>

template <typename S> class dnode{
public:
    S elem;
    dnode* next;
    dnode* before;
    dnode():next(NULL), before(NULL){}
};

template <typename elemtype> class deque{
public:
    int length;
    dnode<elemtype>* tail;
    dnode<elemtype>* head;
    deque():length(0),tail(NULL),head(NULL){}
    ~deque();
    elemtype pop();
    elemtype popleft();
    void append(elemtype elem);
};
template <typename elemtype>
deque<elemtype>::~deque(){
    while(length>0){
        pop();
    }
}

template <typename elemtype>
elemtype deque<elemtype>::pop(){
    elemtype elem;
    dnode<elemtype>* tmp=NULL;
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
elemtype deque<elemtype>::popleft(){//Copy of pop(), just replace head<-->tail, before<-->next
    elemtype elem;
    dnode<elemtype>* tmp=NULL;
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
void deque<elemtype>::append(elemtype elem){
    dnode<elemtype>* p=new dnode<elemtype>;
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
