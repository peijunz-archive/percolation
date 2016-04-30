#ifndef ZONE_H
#define ZONE_H
#include <iostream>
#include <cstdint>
#define NDEBUG
#include <cassert>
template<int D>
/**
 * @brief The combined chars union for small char arrays(D<=4)
 *
 * + An char array
 * + Assignment/Comparation can be parallelize.
 */
union zone{
    int8_t c[D];
    int32_t a;
    zone():a(0){assert((D<=4)&&(D>1));}
    zone(int32_t init):a(init){assert((D<=4)&&(D>1));}
    int8_t & operator[](int i){return c[i];}
    int32_t & operator=(const int b){return (a=b);}
    int32_t & operator=(const zone com){a=com.a; return a;}
    bool operator==(const zone com){return a==com.a;}
    bool operator!=(const zone com){return a!=com.a;}
    int32_t operator-(zone &rhs){
        zone tmp;
        for(int i=0;i<D;i++){
            tmp[i]=c[i]-rhs[i];
        }
        return tmp.a;
    }
    void print(){
        for(int i=0;i<D;i++){
            cout<<c[i]<<"\t";
        }
        cout<<endl;
    }
};
template<int D>
/**
 * @brief The combined large chars union for small char arrays(D<=4)
 *
 * + An char array
 * + Assignment/Comparation can be parallelize.
 */
union zonehd{
    int8_t c[D];
    int64_t a;
    zonehd():a(0){assert((D<=8)&&(D>4));}
    zonehd(int64_t init):a(init){assert((D<=8)&&(D>4));}
    int8_t & operator[](int i){return c[i];}
    int64_t & operator=(const int b){return (a=b);}
    int64_t & operator=(const zonehd com){a=com.a; return a;}
    bool operator==(const zonehd com){return a==com.a;}
    bool operator!=(const zonehd com){return a!=com.a;}
    int64_t operator-(zonehd &rhs){
        zonehd tmp;
        for(int i=0;i<D;i++){
            tmp[i]=c[i]-rhs[i];
        }
        return tmp.a;
    }
    void print(){
        for(int i=0;i<D;i++){
            cout<<c[i]<<"\t";
        }
        cout<<endl;
    }
};

#endif //ZONE_H
