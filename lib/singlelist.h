/***
 * data structure of single directional list
 * quene and stack
 ***/

#ifndef SINGLELIST_H
#define SINGLELIST_H

#include <iostream>
#include <stdlib.h>

using namespace std;
template <typename T>
/**
 * @brief The node class of single direction.
 *
 * Infrastructure for naive stack and quene
 */
class node{
public:
    node *next;
    T data;
    node():next(0){}
    node(T d):next(0), data(d){}
};

template <typename dtype>
/**
 * @brief The stack class
 */
class stack{
public:
    stack():head(0){}
    ~stack();
    ///Add new data
    void append(dtype);
    ///popout some data
    dtype pop();
    ///Judge if the stack is empty
    bool notempty(){return head?true:false;}
private:
    node<dtype> *head;
};

template <typename dtype>
void stack<dtype>::append(dtype d){
    node<dtype> *tmp=new node<dtype>(d);
    tmp->next=head;
    head=tmp;
}

template <typename dtype>
dtype stack<dtype>::pop(){
    node<dtype> *tmp;
    dtype d;
    if(head){
        tmp=head;
        d=head->data;
        head=head->next;
        delete tmp;
        return d;
    }
    cout<<"ERROR in stack.pop():\tAlready EMPTY"<<endl;
    exit(0);
}

template <typename dtype>
stack<dtype>::~stack(){
    node<dtype> *tmp;
    while(head){
        tmp=head;
        head=head->next;
        delete tmp;
    }
}

template <typename dtype>
class quene{
public:
    quene(){tail.next=0;head=&tail;}
    ~quene();
    ///Add new data
    void append(dtype);
    ///Pop some data out
    dtype pop();
    ///Judge if the stack is empty
    bool notempty(){return (head!=&tail);}
private:
    node<dtype> *head;
    node<dtype> tail;
};

template <typename dtype>
quene<dtype>::~quene(){
    node<dtype> *tmp;
    while(tail.next){
        tmp=tail.next;
        tail.next=tmp->next;
        delete tmp;
    }
}

template <typename dtype>
dtype quene<dtype>::pop(){
    node<dtype> *tmp;
    dtype d;
    if(tail.next){
        tmp=tail.next;
        tail.next=tmp->next;
        if(tmp==head){
            head=&tail;
        }
        d=tmp->data;
        delete tmp;
        return d;
    }
    cout<<"ERROR in quene.pop():\tAlready EMPTY"<<endl;
}

template <typename dtype>
void quene<dtype>::append(dtype d){
    head->next=new node<dtype>(d);
    head=head->next;
}

#endif //SINGLELIST
