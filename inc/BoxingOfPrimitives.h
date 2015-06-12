#ifndef VECCORE_BOXING_PRIMITIVES_H_
#define VECCORE_BOXING_PRIMITIVES_H_

#include <VecCoreGlobal.h>

namespace veccore {
inline namespace VECCORE_IMPL_NAMESPACE {

template <typename T> class BoxedPrimitive;

// a helper class to assign something under a mask
template <typename T, typename Mask>
class MaskedType {
 private:
    T& fT;
    Mask fmask;

 public:
    VECCORE_INLINE
    MaskedType( T & t, Mask m ) : fT(t), fmask(m) {};

    VECCORE_INLINE
    T & operator=( T const & other ){
      if(fmask) fT=other;
      return fT;
    }

    template <typename M>
    VECCORE_INLINE
    T & operator=( BoxedPrimitive<M> const other ){
      if(fmask) fT=other.data();
      return fT;
    }

    // we need to add some common operations
    VECCORE_INLINE
    T& operator+=( T const &other ) {
      if(fmask) fT+= other;
      return fT;
    }

    VECCORE_INLINE
    T& operator-=( T const &other ) {
      if(fmask) fT-= other;
      return fT;
    }

    VECCORE_INLINE
    T& operator*=( T const &other ) {
      if(fmask) fT*= other;
      return fT;
    }

    VECCORE_INLINE
    T& operator/=( T const &other ) {
      if(fmask) fT/= other;
      return fT;
    }
};


// a class to "Box ( == make a class)" a primitive type
// enabling to treat simple pods just like we treat vector types (for example Vc)
template <typename T>
class BoxedPrimitive{
  private:
    T fT;

  public:
    VECCORE_INLINE
    T data() const {return fT;}

    VECCORE_INLINE
    BoxedPrimitive() : fT(0) { }

    VECCORE_INLINE
    BoxedPrimitive(T const & t) : fT(t) { }

    // construct from an address
    // semantics is: grab value at that address, mimics behaviour of Vc
    VECCORE_INLINE
    BoxedPrimitive(T const * addr ) : fT(*addr) {}

    // copy constructor must copy the value not the reference
    VECCORE_INLINE
    BoxedPrimitive( BoxedPrimitive const & other ) : fT(other.fT) { }

    VECCORE_INLINE
    BoxedPrimitive & operator=( T const x){
       fT = x;
       return *this;
   }

    // operator to do masked operations notation adopted from Vc
    VECCORE_INLINE
    MaskedType<T,bool> operator()(bool mask){
      return MaskedType<T,bool>( fT, mask );
    }

    // operator/accessor to fetch a "vector lane"
    VECCORE_INLINE
    T const operator[](int i) const {
      assert(i==0);
      return fT;
    }

    // for getting an lvalue reference to a "vector lane"
    VECCORE_INLINE
    T &operator[](int i) {
      assert(i==0);
      return fT;
    }

    // for "Unboxing" type conversions
    VECCORE_INLINE
    operator T() { return T(fT); }

    // operators +=,-=,*=,/=
#define INPLACE_BINARY_OP(OPERATOR) \
    VECCORE_CUDA_HEADER_BOTH \
    VECCORE_INLINE \
    BoxedPrimitive& operator OPERATOR(const BoxedPrimitive other) { \
      fT OPERATOR other.fT; return *this; \
    } \
    VECCORE_CUDA_HEADER_BOTH \
    VECCORE_INLINE \
    BoxedPrimitive& operator OPERATOR(T scalar) { \
      fT OPERATOR scalar; return *this; \
    }
    INPLACE_BINARY_OP(+=)
    INPLACE_BINARY_OP(-=)
    INPLACE_BINARY_OP(*=)
    INPLACE_BINARY_OP(/=)
#undef INPLACE_BINARY_OP

    // a store function to write back value to array ( address )
    // mimics Vc
    VECCORE_INLINE
    void store(T * addr) const { *addr = fT; }
};

template <typename T>
VECCORE_INLINE
bool operator>(BoxedPrimitive<T> a, BoxedPrimitive<T> b){
  return a.data() > b.data();
}

template <typename T>
VECCORE_INLINE
bool operator<(BoxedPrimitive<T> a, BoxedPrimitive<T> b){
  return a.data() < b.data();
}

// for comparisons with literals which cannot be promoted to a boxed type
template <typename T, typename Tp >
VECCORE_INLINE
bool operator>(BoxedPrimitive<T>  a, Tp b){
  return a.data() > b;
}

template <typename T, typename Tp >
VECCORE_INLINE
bool operator<( BoxedPrimitive<T>  a, Tp b ){
  return a.data() < b;
}

template <typename T, typename Tp >
VECCORE_INLINE
bool operator>( Tp a , BoxedPrimitive<T>  b ){
  return a > b.data();
}

template <typename T, typename Tp >
VECCORE_INLINE
bool operator<( Tp a, BoxedPrimitive<T>  b ){
  return a < b.data();
}

#define BINARY_OP(OPERATOR, INPLACE) \
template <typename T, typename S> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
BoxedPrimitive<T> operator OPERATOR(const BoxedPrimitive<T> lhs, \
                                 const BoxedPrimitive<S> rhs) { \
  BoxedPrimitive<T> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename T> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
BoxedPrimitive<T> operator OPERATOR(BoxedPrimitive<T> const lhs, \
                                 const T rhs) { \
  BoxedPrimitive<T> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename T> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
BoxedPrimitive<T> operator OPERATOR(const T lhs, \
                                 BoxedPrimitive<T> const rhs) { \
  BoxedPrimitive<T> result(lhs); \
  result INPLACE rhs; \
  return result; \
}
BINARY_OP(+, +=)
BINARY_OP(-, -=)
BINARY_OP(*, *=)
BINARY_OP(/, /=)
#undef BINARY_OP

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
BoxedPrimitive<T> operator-(BoxedPrimitive<T> const x) {
    return -x.data();
}

// restore pod behaviour for streams
template <typename Stream_t, typename T>
Stream_t & operator<<(Stream_t & str, BoxedPrimitive<T> const x){
    str << x.data();
    return str;
}

} // end inline namespace
} // end veccore namespace

#endif
