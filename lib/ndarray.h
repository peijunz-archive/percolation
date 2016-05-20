#ifndef NDARRAY_H
#define NDARRAY_H
#include <iostream>
#include <initializer_list>
using namespace std;
/**
 * @file ndarray.h
 * @author zpj
 * @brief The n-Dimensional array template for n>1
 * @todo
 * + broadcasting?
 * + Math operations
 * + Make dimension a template argument
 *
 * Should have a view class for slicing?
 */
template <typename dtype>
/// The n-Dimensional C-style array template class
class ndarray{
private:
    uint _dim;            ///< dimension of the array
    uint *_shape;         ///< shape of each dimension
    /**
     * @brief stride of every axis.
     *
     * For the purpose of transposition,
     * the naive stride of last index is reserved.
     * So the last stride may be non-zero. Be cautious!
     */
    uint *_stride;
    void check(){
        if(_dim<=0){
            cerr<<"Error: Positive dimension needed!"<<endl;
            exit(0);
        }
        for(uint i=0;i<_dim;i++){
            if(_shape[i]<=0){
                cerr<<"Error: Positive shape needed!"<<endl;
                exit(0);
            }
        }
    }

public:
    dtype *head;        ///< head of the array data
    /// Clear Memory
    void clear(){
        if(_dim){
            delete [] _shape;
            delete [] _stride;
            delete [] head;
            _stride=&_dim;
        }
    }
    ~ndarray(){clear();}

    /// Naive Constructor setting size to 0
    ndarray():_dim(0),_shape(nullptr),_stride(&_dim),head(nullptr){}
    /**
     * @brief Initialize an ndarray for a square matrix
     * @param d     dimension
     * @param w     width
     */
    ndarray(uint d, uint w):
        _dim(d),_shape(new uint[d]),_stride(new uint[d+1]){
        _stride[d]=1;
        for(uint i=0;i<d;i++){
            _shape[i]=w;
            _stride[d-i-1]=_stride[d-i]*w;
        }
        check();
        head=new dtype[*_stride];
    }
    /**
     * @brief A ndarray with a shape given by pointer/array
     * @param d     dimension
     * @param sh    shape array
     *
     * For dynamic construction
     */
    ndarray(uint d, uint *sh):
        _dim(d),_shape(new uint[d]),_stride(new uint[d+1]){
        _stride[d]=1;
        for(uint i=0;i<d;i++){
            _shape[i]=sh[i];
            _stride[d-i-1]=_stride[d-i]*sh[d-i-1];
        }
        check();
        head=new dtype[*_stride];
    }
    /**
     * @brief Initialize an ndarray with a shape given by an initializer list
     * @param l     initializer list for shape
     *
     * For static construction
     */
    ndarray(initializer_list<uint> l):_dim(l.size()){
        _shape=new uint[_dim];
        copy(begin(l), end(l), _shape);
        _stride=new uint[_dim+1];
        _stride[_dim]=1;
        for(uint i=0;i<_dim;i++){
            _stride[_dim-i-1]=_stride[_dim-i]*_shape[_dim-i-1];
        }
        check();
        head=new dtype[*_stride];
    }

    /**
     * @brief Copy constructor copying the shape only.
     * @param x     The source of ndarray
     * + You can copy the shape at the definition or copy the
     *   shape with moving assignment anywhere
     * + If you want to copy the data, use copy assignment!
     *
     * ```cpp
     * ndarray<int> a{3,4};
     * a=1;
     * ndarray<int> b=a, c; // b get the shape of a at definition
     * c=ndarray<int>(a)    // c get the shape of a
     * ```
     */
    template<typename T>
    ndarray(const ndarray<T> & x):
        _dim(x.dim()),_shape(new uint[x.dim()]),\
        _stride(new uint[x.dim()+1]),\
        head(new dtype[x.size()])
    {
        for(uint i=0;i<_dim;i++){
            _shape[i]=x.shape(i);
            _stride[i+1]=x.stride(i);
        }
        _stride[0]=x.size();
    }
    /// Move constructor
    ndarray(ndarray<dtype> && x):
        _dim(x._dim),_shape(x._shape),head(x.head),_stride(x._stride){
        x._shape=nullptr;
        x.head=nullptr;
        x._stride=nullptr;
    }
    /**
     * @brief Copy Assignment copying all data
     * @param x     The source of ndarray
     * @todo If the shape is the same while stride is not,
     *   how to copy? Using nditer?
     *
     * + Copy an empty x array will clear() the array
     * + Copy to an empty array will deep copy the array x
     * + Except for above conditions, copy between arrays
     *   of different size is an error
     * + Indirective method is clear first and then copy
     * + If size is equal, copy according to raw index,
     *   regardless of the shape, dim, stride
     *
     * ```
     * ndarray<int> c{3,3}, d;
     * c=1
     * d=c;    // Copy the whole data of c
     * ```
     */
    ndarray<dtype> & operator= (const ndarray<dtype>& x){
        if(this != &x){
            if(*(x._stride)==0){
                clear();
                return *this;
            }
            if(*_stride==0){
                *this=ndarray<dtype>(x);
            }
            if(*_stride==*(x._stride)){
                for(uint i=0;i<*_stride;i++){
                    head[i]=x.head[i];
                }
            }
            else{
                cerr<<"Unmatched copy! Please clear() first."<<endl;
                exit(0);
            }
        }
        return *this;
    }
    /// Move assignment
    ndarray<dtype> & operator= (ndarray<dtype>&& x){
        _dim=x._dim;
        _shape=x._shape;
        head=x.head;
        _stride=x._stride;
        x._shape=nullptr;
        x.head=nullptr;
        x._stride=nullptr;
        return *this;
    }
    /**
     * @brief Set all the values in the array to a init value
     * @param val   initial value
     */
    ndarray<dtype> & operator= (dtype val){
        for(uint i=0;i<*_stride;i++)
            head[i]=val;
        return *this;
    }

    uint size() const {return *_stride;}
    uint dim() const {return _dim;}
    uint shape(uint i) const {return _shape[i];}
    const uint * shape() const {return _shape;}
    uint stride(uint i) const{return _stride[i+1];}
    const uint * stride() const{return _stride+1;}

    /**
     * @brief Naive indexing using raw index
     * @param rawind    raw index
     *
     * + Good efficiency!
     * + If dim==1, use [] rather than ()
     */
    dtype & operator[] (uint rawind){return *(head+rawind);}

    template<uint N>
    /**
     * @brief dot product for small N
     * @param coo   the coordinates of an element
     * @return
     */
    uint dot(uint *coo){
        uint offset=0;
        for(uint i=0;i<N;i++){
            offset+=coo[i]*_stride[i+1];
        }
        return offset;
    }
    template<uint D>
    /**
     * @brief Indexing by an index array
     * @param coo   the coordinates of an element
     *
     * + Especially useful for array with unknown dimension.
     * + The speed is about 0.6+ that of operator ()(Args... args)
     * + If _dim==1, use [] directly rather than ()
     *
     * @return The indexed element
     */
    dtype & operator()(uint *coo){
        uint offset=0;
        for(uint i=0;i<D;i++){
            offset+=coo[i]*_stride[i+1];
        }
        return head[offset];
    }

    uint adder(uint *p,uint last){
      return *p*last;
    }
    template<typename... Args>
    uint adder(uint *p, uint first, Args... args) {
      return *p*first+adder(p+1, args...);
    }

    /**
     * @brief Indexing by the argument list of operator()
     * @param args the index of an element
     *
     * + Very useful for array with known dimension.
     * + Emulate the original C style array
     * + If _dim==1, use [] directly rather than ()
     * + If args are not enough, suppose we fill it with 0
     *
     * @return The indexed element
     */
    template<typename... Args>
    dtype & operator()(Args... args){
        return *(head+adder(_stride+1, args...));
    }

    /**
     * @brief Roll the index in a given axis with periodical boundary condition
     * @param rawind raw index
     * @param axis
     * @param dir   positive/negative
     * + For positive axis: `0, 1,..., dim-1`
     * + For corresponding negative axis: `-dim, 1-dim,..., -1`
     * @return Raw index after rolling
     */
    uint rollindex(uint rawind, int axis, int dir=1){
        uint axisind=(rawind%_stride[axis])/_stride[axis+1];
        if(dir==1){
            rawind+=_stride[axis+1];
            if(axisind+1==_shape[axis])
                rawind-=_stride[axis];
        }
        else{
            rawind-=_stride[axis+1];
            if(axisind==0)
                rawind+=_stride[axis];
        }
        return rawind;
    }
    /**
     * @brief roll ind for unknow positiveness by using rollindex
     */
    uint rollind(int rawind, int ax){
        if(ax<0){
            return rollindex(rawind, ax+_dim, -1);
        }
        else{
            return rollindex(rawind, ax, 1);
        }
    }

    /// Transpose between two axis
    void transpose(uint i=1, uint j=0){
        if(i==j) return;
        uint tmp;
        tmp=_stride[i+1];
        _stride[i+1]=_stride[j+1];
        _stride[j+1]=tmp;
        tmp=_shape[i];
        _shape[i]=_shape[j];
        _shape[j]=tmp;
    }
    uint size_attached(){
        uint s=sizeof(ndarray<dtype>);
        if(_dim){
            s+=sizeof(uint)*(2*_dim+1);
        }
        return s;
    }
    void printind(uint ind){
        uint t[_dim];
        for(uint i=_dim-1;i>=0;i--){
            t[i]=ind%_stride[i];
            ind/=_stride[i];
        }
        cout<<"(";
        for(uint i=0;i<_dim;i++){
            cout<<t[i]<<", ";
        }
        cout<<"\b\b) \n";
    }
};
#endif //NDARRAY_H
