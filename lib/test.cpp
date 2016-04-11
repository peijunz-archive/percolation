#include <iostream>
#include "linklist.h"
#include "16807.h"
#include "ndarray.h"

int testrand(){
    myrand(-1);
    for(int i=0;i<100;i++){
        if(myrand()>1 && myrand<0)
            return 1;
    }
    return 0;
}

int testndarray(){
    ndarray<char> a[5];
    ndarray<int> b={3,4,5};
    int s[3]={2,3,4};
    ndarray<double> c(3,s);
    for(int i=0;i<5;i++){
        a[i].set(3,5);
    }
    a[2].print();
    b.print();
    c.print();
}

int testlist(){
    stack<int> s;
    quene<int> q;
    for(int i=0;i<10;i++){
        s.append(i);
        q.append(i);
    }
    cout<<"Stack"<<endl;
    while(s.notempty()){
        cout<<s.pop()<<endl;
    }
    cout<<"Quene"<<endl;
    while(q.notempty()){
        cout<<q.pop()<<endl;
    }
}
