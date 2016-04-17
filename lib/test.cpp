#include <iostream>
#include "singlelist.h"
#include "16807.h"
#include "ndarray.h"
#include "nditer.h"

/**
 * @file test.cpp
 * @author zpj
 * @brief Test various modules
 * @bug No
 */
/**
 * @brief test myrand()
 */
int testrand(){
    myrand(-1);
    cout<<"Module: 16807"<<endl;
    for(int i=0;i<20;i++){
        cout<<myrand()<<' ';
        if(myrand()>1 && myrand()<0)
            return 1;
    }
    cout<<endl;
    return 0;
}
/**
 * @brief test the ndarray template
 */
int testndarray(){
    ndarray<char> a[5];
    ndarray<int> b={3,4,5};
    int s[3]={2,3,4};
    ndarray<double> c(3,s);
    for(int i=0;i<5;i++){
        a[i]=ndarray<char>(3,5);
    }
    a[2].print();
    b.print();
    c.print();
    return 0;
}
/**
 * @brief test two list class: quene and stack
 */
int testlist(){
    stack<int> s;
    quene<int> q;
    for(int i=0;i<10;i++){
        s.append(i);
        q.append(i);
    }
    cout<<"Module: Stack"<<endl;
    while(s.notempty()){
        cout<<s.pop()<<' ';
    }
    cout<<endl;
    cout<<"Module: Quene"<<endl;
    while(q.notempty()){
        cout<<q.pop()<<' ';
    }
    cout<<endl;
    return 0;
}
/**
 * @brief test nditer class
 */
void testnditer(){
    int a[2]={4,5};
    nditer it(2,a);
    while(it.next()){
        it.print();
    }
}
