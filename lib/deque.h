#ifndef DEQUE_H
#define DEQUE_H

typedef int elemtype;//存储rawindex，使用int可以达到4e9，十亿

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

void initq(quene &q);

void append(quene &q, elemtype &elem);//tested

elemtype pop(quene &q);//tested

elemtype popleft(quene &q);//tested

#endif
