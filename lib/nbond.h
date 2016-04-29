#ifndef NBOND_H
#define NBOND_H
#include <cstdint>
#define NDEBUG
#include <cassert>
template<int D>
/**
 * @brief The near bond struct for N-Dimensional lattice
 *
 * ## Advantage:
 * + Absorb data effeciently
 * + No check for overflowing
 * + Fixed Small Space Usage: `(2*D+1)` byte for each site
 * + Good for about `D<10`
 *
 * ## Comparison
 * As bonds are always fewer than `2*D`, if we use either linked list or vector,
 * it will take up too much space.
 * + vector will take up 24 byte which is enough for 12D space.
 * + linked list will take `9*(k+1)`, whereas `1<=k<=2`
 */
struct nbond{
    uint8_t size;
    /// Container
    int8_t c[2*D];
    nbond():size(0){}
    int8_t & operator[](int i){return c[i];}
    void append(int8_t x){
        assert(size<2*D);
        c[size++]=x;
    }
    void clear(){size=0;}
    /**
     * @brief Delete a bond at a known position
     * @param pos   position
     */
    void del(int pos){
        for(int i=pos+1;i<size;i++){
            c[i-1]=c[i];
        }
        size--;
    }
    /**
     * @brief Find a reverse bond of ax and then delete it
     * @param ax    original axis
     * @return found or not
     */
    bool finddelrev(int8_t ax){
        if(ax<0) ax+=D;
        else ax-=D;
        for(int i=0;i<size;i++){
            if(c[i]==ax){
                del(i);
                return true;
            }
        }
        return false;
    }
};

#endif //NBOND_H
