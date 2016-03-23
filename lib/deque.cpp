#include "ndarray.h"
#include <stdio.h>
#include <stdlib.h>

typedef int elemtype;//save raw index

struct node;
typedef node* link;
struct node{
    elemtype elem;
    link next;
    link before;
};

struct quene{
    int length;
    link tail;
    link head;
};

elemtype pop(quene &q){
    elemtype elem;
    link tmp=NULL;
    if(q.length==0)
        exit;
    if (q.length==1)
        q.tail=NULL;
    elem=q.head->elem;
    tmp=q.head;
    q.head=tmp->next;
    free(tmp);
    if (q.length!=1)
        q.head->before=NULL;
    q.length--;
    return elem;
}

elemtype popleft(quene &q){//Copy of pop(), just replace head<-->tail, before<-->next
    elemtype elem;
    link tmp=NULL;
    if(q.length==0)
        exit;
    if (q.length==1)
        q.head=NULL;
    elem=q.tail->elem;
    tmp=q.tail;
    q.tail=tmp->before;
    free(tmp);
    if (q.length!=1)
        q.tail->next=NULL;
    q.length--;
    return elem;
}

void initq(quene &q){
    q.length=0;
    q.tail=NULL;
    q.head=NULL;
}

void append(quene &q, elemtype &elem){
    link p=NULL;
    p=(link)malloc(sizeof(node));
    p->elem=elem;
    if (q.head==0){
        p->before=NULL;
        p->next=NULL;
        q.head=q.tail=p;
    }
    else{
        p->next=q.head;
        q.head=q.head->before=p;
    }
    q.length++;
}
