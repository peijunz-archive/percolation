#ifndef ZONEBOND_H
#define ZONEBOND_H
#include <cstdint>

enum bondtype : uint8_t {
    empty = 0,  ///< No bond here
    branch = 1, ///< Branch, identify by recursive cutting leaf
    junction = 2, ///< Junction, default value for any bond
    nonbrg = 3, ///< A bond without which the cluster is still connected
    marked = 64 ///< Mark to replace axis if a point is backtraced.
};
constexpr uint upper_size=8;
constexpr uint lower_size=1;
constexpr int least_size(uint n){
    return (1<=n && n<=upper_size)?(n>lower_size?least_size(n-n/2)*2:lower_size):0;
}

template<int N>
/// Int of at least given size N
struct least {typedef typename least <least_size(N)>::_int _int;};

template<> struct least<0> {typedef void _int;};
template<> struct least<1> {typedef int8_t _int;};
template<> struct least<2> {typedef int16_t _int;};
template<> struct least<4> {typedef int32_t _int;};
template<> struct least<8> {typedef int64_t _int;};

template<int D>
/**
 * @brief The combined chars union for small char arrays
 *
 * + An char array
 * + For `2<=D<=9
 * + Assignment/Comparation can be parallelize.
 */
union zone {
private:
    typedef typename least<D>::_int intD_t;
public:
    int8_t c[D];
    intD_t a;
    zone(): a(0) {}
    zone(intD_t init): a(init) {}
    int8_t & operator[](int i) {
        return c[i];
    }
    intD_t & operator=(const int b) {
        return (a = b);
    }
    intD_t & operator=(const zone &com) {
        a = com.a;
        return a;
    }
    bool operator==(const zone com) {
        return a == com.a;
    }
    bool operator!=(const zone com) {
        return a != com.a;
    }
    intD_t operator-(zone &rhs) {
        zone tmp;
        for(int i = 0; i < D; i++) {
            tmp[i] = c[i] - rhs[i];
        }
        return tmp.a;
    }
};


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
struct nbond {
    uint8_t size;
    /// Container
    int8_t c[2 * D];
    nbond(): size(0) {}
    int8_t & operator[](int i) {
        return c[i];
    }
    void append(int8_t x) {
        c[size++] = x;
    }
    void clear() {
        size = 0;
    }
    /**
     * @brief Delete a bond at a known position
     * @param pos   position
     */
    void del(int pos) {
        for(int i = pos + 1; i < size; i++) {
            c[i - 1] = c[i];
        }
        size--;
    }
    /**
     * @brief Find a reverse bond of ax and then delete it
     * @param ax    original axis
     * @return found or not
     */
    bool finddelrev(int8_t ax) {
        if(ax < 0) ax += D;
        else ax -= D;
        for(int i = 0; i < size; i++) {
            if(c[i] == ax) {
                del(i);
                return true;
            }
        }
        return false;
    }
};
#endif //ZONEBOND_H
