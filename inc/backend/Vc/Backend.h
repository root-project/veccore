/// \file vc/backend.h

#ifndef VECCORE_BACKEND_VCBACKEND_H_
#define VECCORE_BACKEND_VCBACKEND_H_

#ifdef VCBACKEND

#include "VecCoreGlobal.h"
#include <Vc/Vc>

namespace veccore{

namespace backend {

namespace vector {

inline namespace VECCORE_IMPL_NAMESPACE {

template< typename RealInput_t = double >
struct kVc {
  // The convention for type names is
  // _t is a type but it should be a primitive or otherwise scalar type
  // _v means that this type could potentially be a vector type ( but not necessarily )

  // define the base primitive type
  typedef RealInput_t                 Real_t;
  // we could define the other base primitive types
  typedef double                      Double_t;
  typedef double                      Float_t;

  // define vector types for wanted precision
  typedef typename Vc::Vector<Real_t>          Real_v;
  typedef typename Vc::Vector<Real_t>          BoxedReal_v;

  typedef typename Vc::Vector<Real_t>::Mask    Bool_v; // ( should we call this RealBool )

  // define vector types for standard double + float types
  typedef typename Vc::Vector<double>          Double_v;
  typedef typename Vc::Vector<float>           Float_v;
  typedef typename Vc::Vector<double>::Mask    DoubleBool_v;
  typedef typename Vc::Vector<float>::Mask     FloatBool_v;

  typedef typename Vc::int_v                   Int_v;
  typedef typename Vc::int_v::Mask             IntBool_v;

  typedef Vc::Vector<Real_t>                   Index_t;

  // other properties of this backend
  // this is to be benchmarked ... could also make it a
  // configurable template parameter of this trait
  constexpr static bool early_returns = false;

  // the VectorSizes
  constexpr static int kRealVectorSize = Real_v::Size;
  constexpr static int kDoubleVectorSize = Double_v::Size;
  constexpr static int kFloatVectorSize = Float_v::Size;
  // ... in principle also kIntVectorSize ...


  // function determining how types Real_v, Double_v, Float_v are to
  // be constructed given an array as constructor input
  // in case of Vc, we need to give the startADDRESS
  // arrayelement is usually something like an element x[i] in an array x
  template <typename Type>
  VECCORE_INLINE
  static const Type * GrabVectorStartAt( Type const & arrayelement ){
          return &arrayelement;
  }
};

// Functions to extract individual components of vectors
// meant to provide a homogeneous way to provide operator[]
// to both complex and primitive types
// (since operator[] does not exist for primitive double, float, ... )
// function gives back component index of x and this component
// x can be modified

// Alternative Names: Operator[]
template <typename Type>
VECCORE_INLINE
static
void
ComponentAssign( int index, typename Vc::Vector<Type>::EntryType what,
                 typename Vc::Vector<Type> & to ) {
    to[index]=what;
}


template <typename Type>
VECCORE_INLINE
static
typename Vc::Vector<Type>::EntryType const
GetComponent( typename Vc::Vector<Type> const & x, int index ) {
    return x[index];
}

template <typename Type>
VECCORE_INLINE
static
void
SetComponent( typename Vc::Vector<Type> & x, int index,
              Type const to ) {
    x[index]=to;
}

// acces components of a "mask" -- template on type does not work here;
// the compiler was not able to deduce the type correctly
VECCORE_INLINE
static
bool const
GetMaskComponent( typename Vc::Vector<float>::Mask const & x, int index ) {
    return x[index];
}

VECCORE_INLINE
static
bool const
GetMaskComponent( typename Vc::Vector<double>::Mask const & x, int index ) {
    return x[index];
}


// might need to abstract on other things
// LoadFromArray; StoreToArray


template <typename Type>
VECCORE_INLINE
static
void CondAssign(typename Vc::Vector<Type>::Mask const &cond,
                Vc::Vector<Type> const &thenval,
                Vc::Vector<Type> const &elseval,
                Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
  (*output)(!cond) = elseval;
}

template <typename Type>
VECCORE_INLINE
void CondAssign(typename Vc::Vector<Type>::Mask const &cond,
                Type const &thenval,
                Type const &elseval,
                Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
  (*output)(!cond) = elseval;
}

template <typename Type>
VECCORE_INLINE
void MaskedAssign(typename Vc::Vector<Type>::Mask const &cond,
                  Vc::Vector<Type> const &thenval,
                  Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
}

template <typename Type>
VECCORE_INLINE
void MaskedAssign(typename Vc::Vector<Type>::Mask const &cond,
                  Type const &thenval,
                  Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
}

// stores a vector type into a memory position ( normally an array ) toaddr
// toaddr has to be properly aligned
// this function is an abstraction for the Vc API "store"
template <typename Type>
VECCORE_INLINE
void StoreTo( typename Vc::Vector<Type> const & what,
            Type * toaddr ){
    what.store(toaddr);
}

// loads number of bytes corresponding to the into type from a memory
// location and puts it into "into"
template <typename Type>
VECCORE_INLINE
void LoadFrom( typename Vc::Vector<Type> & into, Type const * const fromaddr ){
    into.load(fromaddr);
}


// special version of MaskedAssignment when output
// is of type int_v
//VECCORE_INLINE
//void MaskedAssign(VcBool const &cond,
//                  const Inside_t thenval,
//                  VcInside *const output) {
//  (*output)(VcInside::Mask(cond)) = thenval;
//}

// returns if all lanes/slots in (vector) condition are true
template <typename MaskType>
VECCORE_INLINE
bool IsFull(MaskType const &cond) {
    return cond.isFull();
}

// returns if any lane/slot in (vector) condition is true
template <typename MaskType>
VECCORE_INLINE
bool Any(MaskType const &cond) {
  return !cond.isEmpty();
}

// returns if all lanes/slots in (vector) condition are false
template <typename MaskType>
VECCORE_INLINE
bool IsEmpty(MaskType const &cond) {
  return cond.isEmpty();
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Abs(typename Vc::Vector<Type> const &val) {
  return Vc::abs(val);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Sqrt(typename Vc::Vector<Type> const &val) {
  return Vc::sqrt(val);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> ATan2(typename Vc::Vector<Type> const &y,
                  typename Vc::Vector<Type> const &x) {
  return Vc::atan2(y, x);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Sin(typename Vc::Vector<Type> const &x) {
  return Vc::sin(x);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Cos(typename Vc::Vector<Type> const &x) {
  return Vc::cos(x);
}

template <typename Type>
VECCORE_INLINE
void SinCos(typename Vc::Vector<Type> const &x,
            typename Vc::Vector<Type> * s,
            typename Vc::Vector<Type> * c ) {
  return Vc::sincos(x, s, c);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Tan(typename Vc::Vector<Type> const &radians) {
  // apparently Vc does not have a tan function
  // return Vc::tan(radians);
  // emulating it for the moment
  typename Vc::Vector<Type> s,c;
  Vc::sincos(radians,&s,&c);
  return s/c;
}


template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Pow(typename Vc::Vector<Type> const &x,
                              typename Vc::Vector<Type> const &arg) {
    // Vc does not have a proper Pow function
    return Vc::exp(Vc::log(x) * arg);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Min(typename Vc::Vector<Type> const &val1,
                              typename Vc::Vector<Type> const &val2) {
  return Vc::min(val1, val2);
}

template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Max(typename Vc::Vector<Type> const &val1,
                              typename Vc::Vector<Type> const &val2) {
  return Vc::max(val1, val2);
}


template <typename Type>
VECCORE_INLINE
typename Vc::Vector<Type> Floor( typename Vc::Vector<Type> const &val ){
  return Vc::floor( val );
}

} // End inline namespace

} // end Vector namespace

} // end Backend namespace

} // End global namespace

#endif
#endif // VECCORE_BACKEND_VCBACKEND_H_
