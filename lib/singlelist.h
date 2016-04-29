#ifndef SINGLELIST_H
#define SINGLELIST_H
#include <iostream>
/**
 * @internal
 * @brief Data structure of uni-directional list: quene and stack
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
    node():next(nullptr){}
    ///Constructor with data
    node(T d):next(nullptr), data(d){}
};

template <typename dtype>
/// The naive stack class
class stack{
public:
    ///Constructor
    stack():head(nullptr){}
    void clear(){
        node<dtype> *tmp;
        while(head){
            tmp=head;
            head=head->next;
            delete tmp;
        }
    }
    ///Destructor
    ~stack(){clear();}
    /// Add new data
    void append(dtype d){
        node<dtype> *tmp=new node<dtype>(d);
        tmp->next=head;
        head=tmp;
    }
    /// Pop some data out
    dtype pop(){
        node<dtype> *tmp;
        dtype d;
        if(head){
            tmp=head;
            d=head->data;
            head=head->next;
            delete tmp;
            return d;
        }
        cerr<<"ERROR in stack.pop():\tAlready EMPTY"<<endl;
        exit(0);
    }
    ///Judge if the stack is empty
    bool notempty(){return head;}
private:
    node<dtype> *head;///< head of data
};


template <typename dtype>
/// The naive quene class
class quene{
public:
    ///Constructor
    quene(){tail.next=nullptr;head=&tail;}
    ///Destructor
    void clear(){
        node<dtype> *tmp;
        while(tail.next){
            tmp=tail.next;
            tail.next=tmp->next;
            delete tmp;
        }
    }
    ~quene(){clear();}
    /// Add new data
    void append(dtype d){
        head->next=new node<dtype>(d);
        head=head->next;
    }
    /// Pop some data out
    dtype pop(){
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
        cerr<<"ERROR in quene.pop():\tAlready EMPTY"<<endl;
        exit(0);
    }
    /// Judge if the quene is empty
    bool notempty(){return (head!=&tail);}
private:
    node<dtype> *head;  ///< Head go 1st
    node<dtype> tail;   ///< Tail remain
};

#endif //SINGLELIST
