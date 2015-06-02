#ifndef VECCORE_BOXING_PRIMITIVES_H_
#define VECCORE_BOXING_PRIMITIVES_H_

namespace VecCore {
inline namespace VECCORE_IMPL_NAMESPACE{

template <typename T> class BoxedPrimitiveRef;

// a helper class to assign something under a mask
template <typename T, typename Mask> 
class MaskedType {
 private:
  T& fT;
  Mask fmask;

 public:
 MaskedType( T & t, Mask m ) : fT(t), fmask(m) {};

 T & operator=( T const & other ){
   if(fmask) fT=other;
   return fT;
 }

 template <typename M>
 T & operator=( BoxedPrimitiveRef<M> const other ){
    if(fmask) fT=other.data();
    return fT;
  }

 // we need to add some common operations
 T& operator+=( T const &other ) {
   if(fmask) fT+= other;
   return fT;
 }
 T& operator-=( T const &other ) {
   if(fmask) fT-= other;
   return fT;
 }
 T& operator*=( T const &other ) {
   if(fmask) fT*= other;
   return fT;
 }
 T& operator/=( T const &other ) {
   if(fmask) fT/= other;
   return fT;
 }
};

// a class to "Box ( == make a class)" a primitive type
// enabling to treat simple pods just like we treat vector types (for example Vc) 
// the name Ref indicates that we box a reference to the primitive
// a second class "BoxPrimitive" not holding a ref could also be done
template <typename T> 
class BoxedPrimitiveRef{
  private:
   T & fT;

  public:
   T data() const {return fT;}

   BoxedPrimitiveRef(T & t) : fT(t) {}
  //PrimitiveClassPromotion(T t) : fT(t) {}

   // operator to do masked operations notation adopted from Vc
   MaskedType<T,bool> operator()(bool mask){
    return MaskedType<T,bool>( fT,mask);
 }

   // operator/accessor to fetch a "vector lane"
   T const operator[](int i) const {
       assert(i==0);
       return fT;
   }

   // for "Unboxing" type conversions
   operator T() { return T(fT); }

   // operators +=,-=,*=,/=
#define INPLACE_BINARY_OP(OPERATOR) \
  VECCORE_CUDA_HEADER_BOTH \
  VECCORE_INLINE \
  BoxedPrimitiveRef& operator OPERATOR(const BoxedPrimitiveRef other) { \
    fT OPERATOR other.fT; return *this; \
  } \
  VECCORE_CUDA_HEADER_BOTH \
  VECCORE_INLINE \
  BoxedPrimitiveRef& operator OPERATOR(T scalar) { \
    fT OPERATOR scalar; return *this; \
  }
  INPLACE_BINARY_OP(+=)
  INPLACE_BINARY_OP(-=)
  INPLACE_BINARY_OP(*=)
  INPLACE_BINARY_OP(/=)
#undef INPLACE_BINARY_OP
};

    template <typename T>
    bool operator>( BoxedPrimitiveRef<T> const a, BoxedPrimitiveRef<T> const b ){
        return a.data() > b.data();
    }
    template <typename T>
    bool operator<( BoxedPrimitiveRef<T> const a, BoxedPrimitiveRef<T> const b ){
        return a.data() < b.data();
   }

#define BINARY_OP(OPERATOR, INPLACE) \
template <typename T, typename S> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
BoxedPrimitiveRef<T> operator OPERATOR(const BoxedPrimitiveRef<T> lhs, \
                                 const BoxedPrimitiveRef<S> rhs) { \
  BoxedPrimitiveRef<T> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename T> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
BoxedPrimitiveRef<T> operator OPERATOR(BoxedPrimitiveRef<T> const lhs, \
                                 const T rhs) { \
  BoxedPrimitiveRef<T> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename T> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
BoxedPrimitiveRef<T> operator OPERATOR(const T lhs, \
                                 BoxedPrimitiveRef<T> const rhs) { \
  BoxedPrimitiveRef<T> result(rhs); \
  result INPLACE lhs; \
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
BoxedPrimitiveRef<T> operator-(BoxedPrimitiveRef<T> const vec) {
    return -vec.fT;
}

}}

#endif
