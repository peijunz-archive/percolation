#include "ndarray.h"
#include <stdio.h>
#include <stdlib.h>

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
    ~quene();
    elemtype pop();
    elemtype popleft();
    void append(elemtype elem);
};
quene::~quene(){
    while(length>0){
        pop();
    }
}

elemtype quene::pop(){
    elemtype elem;
    link tmp=NULL;
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
    length--;
    return elem;
}

elemtype quene::popleft(){//Copy of pop(), just replace head<-->tail, before<-->next
    elemtype elem;
    link tmp=NULL;
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
    length--;
    return elem;
}

void quene::append(elemtype elem){
    link p=new node;
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
