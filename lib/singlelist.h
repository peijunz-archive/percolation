#ifndef SINGLELIST_H
#define SINGLELIST_H
#include <iostream>
#include <stdlib.h>
/**
 * @file singlelist.h
 * @author zpj
 * @brief Data structure of uni-directional list: quene and stack
 * @bug No
 ***/

using namespace std;
template <typename T>
/**
 * @brief The node class of single direction.
 *
 * Infrastructure for naive stack and quene
 */
class node{
public:
    ///Pointer to next block
    node *next;
    /// Data to save
    T data;
    ///Constructor
    node():next(0){}
    ///Constructor with data
    node(T d):next(0), data(d){}
};

template <typename dtype>
/// The naive stack class
class stack{
public:
    ///Constructor
    stack():head(0){}
    ///Destructor
    ~stack();
    /// Add new data
    void append(dtype);
    /// Pop some data out
    dtype pop();
    ///Judge if the stack is empty
    bool notempty(){return head?true:false;}
private:
    node<dtype> *head;///< head of data
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
    cerr<<endl<<"ERROR in stack.pop():\tAlready EMPTY"<<endl;
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
/// The naive quene class
class quene{
public:
    ///Constructor
    quene(){tail.next=0;head=&tail;}
    ///Destructor
    ~quene();
    /// Add new data
    void append(dtype);
    /// Pop some data out
    dtype pop();
    /// Judge if the quene is empty
    bool notempty(){return (head!=&tail);}
private:
    node<dtype> *head;  ///< Head go 1st
    node<dtype> tail;   ///< Tail remain
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
    cerr<<endl<<"ERROR in quene.pop():\tAlready EMPTY"<<endl;
    exit(0);
}

template <typename dtype>
void quene<dtype>::append(dtype d){
    head->next=new node<dtype>(d);
    head=head->next;
}

#endif //SINGLELIST
