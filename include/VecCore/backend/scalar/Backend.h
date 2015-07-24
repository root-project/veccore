/// \file scalar/Backend.h

#ifndef VECCORE_BACKEND_SCALARBACKEND_H_
#define VECCORE_BACKEND_SCALARBACKEND_H_

#include "VecCoreGlobal.h"
#include "BoxingOfPrimitives.h"

#include <algorithm>
#include <cstring>
#include <cassert>

namespace veccore{

namespace backend {

namespace scalar {

inline namespace VECCORE_IMPL_NAMESPACE {

template< typename RealInput_t = double  >
struct kScalar {
  // define the base primitive type
  typedef RealInput_t                 Real_t;
  // we could define the other base primitive types
  typedef double                      Double_t;
  typedef float                       Float_t;

  typedef Real_t                      Real_v;
  typedef BoxedPrimitive<Real_t>   BoxedReal_v; // just a temp name to distinguish from Real_v
  typedef bool                        Bool_v;

  typedef double                      Double_v;
  typedef float                       Float_v;
  typedef bool                        DoubleBool_v;
  typedef bool                        FloatBool_v;

  typedef int       Int_v;
  typedef bool      IntBool_v;

  typedef int  Index_t; // the type of indices

  // this flag is probably the most important
  // distinction between kScalar and kCuda ??
  const static bool early_returns = true;

  // alternative typedefs ( might supercede above typedefs )
  typedef int                   Int_t;

//  template <class Backend>
//  VECCORE_CUDA_HEADER_BOTH
//  static VECCORE_CONSTEXPR_RETURN bool IsEqual() { return false; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  static Precision Convert(Precision const &input) { return input; }
  constexpr static int kRealVectorSize = 1;
  constexpr static int kDoubleVectorSize = 1;
  constexpr static int kFloatVectorSize = 1;

  // function determining how types Real_v, Double_v, Float_v are to
  // be constructed given an array as constructor input
  template <typename Type>
  VECCORE_INLINE
  static const Type GrabVectorStartAt( Type const & arrayelement ){
        return arrayelement;
  }
};

//template <>
//VECCORE_CUDA_HEADER_BOTH
//inline VECCORE_CONSTEXPR_RETURN bool kscalar::IsEqual<kScalar>() {
//  return true;
//}

// Functions to extract individual components of vectors
// meant to provide a homogeneous way to provide operator[]
// to both complex and primitive types
// (since operator[] does not exist for primitive double, float, ... )
// function gives back component index of x and this component
// x can be modified

// Alternative Names: Operator[]
template <typename T>
VECCORE_INLINE
static
void
// we should put a restriction on the type of T:
// should be a primitive type --> concepts or static asserts?
ComponentAssign( int index, T const what, T & to ) {
    assert(index == 0);
    to = what;
}


template <typename T>
VECCORE_INLINE
static
T const
GetComponent( T const & x, int index ) {
    assert(index == 0);
    return x;
}


template <typename T>
VECCORE_INLINE
static
void
SetComponent( T & x, int index, T to ) {
    assert(index == 0);
    x = to;
}

// same for Mask
VECCORE_INLINE
static
bool
GetMaskComponent( bool const x, int index ) {
    assert(index == 0);
    return x;
}

template <typename Type>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
void CondAssign(const bool cond,
                Type const &thenval, Type const &elseval, Type *const output) {
  *output = (cond) ? thenval : elseval;
}

template <typename Type>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
void MaskedAssign(const bool cond,
                  Type const &thenval, Type *const output) {
  *output = (cond) ? thenval : *output;
}

// stores a vector type into a memory position ( normally an array ) toaddr
// toaddr has to be properly aligned
template <typename Type>
VECCORE_INLINE
void StoreTo( Type const & what,
              Type * toaddr ){
    *toaddr = what;
}

// loads number of bytes corresponding to the into type from a memory
// location and puts it into "into"
template <typename Type>
VECCORE_INLINE
void LoadFrom( Type & into, Type const *const fromaddr ){
    into = *fromaddr;
}



VECCORE_INLINE
bool IsFull(bool const &cond){
    return cond;
}


//VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
bool Any(bool const &cond) {
  return cond;
}

VECCORE_INLINE
bool IsEmpty(bool const &cond){
    return !cond;
}

template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Type Abs(const Type val) {
  return fabs(val);
}


//template <typename Type>
// SW: It turns out that it is better to explicitely give
// the two different versions for Sqrt; the templated solution
// has problems accepting a wrapped pod
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
float Sqrt(const float val) {
  return std::sqrt(val);
}

VECCORE_INLINE
double Sqrt(const double val) {
  return std::sqrt(val);
}


//
template <typename Type>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Type Pow(const Type val1, const Type val2) {
  return std::pow(val1, val2);
}
//

//
//

// why do we need this??
template <typename Type>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Type Pow(Type const &x, int arg) {
   return pow(x,arg);
}

template <typename Type>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Type Cbrt(const Type val1) {
   return cbrt(val1);
}

template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Type ATan2(const Type y, const Type x) {
  if (x != 0) return  std::atan2(y, x);
  if (y >  0) return  kPi / 2;
  if (y <  0) return -kPi / 2;
  return Type(0.);
}


template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Min(T const &val1, T const &val2) {
#ifndef VECCORE_NVCC_DEVICE
  return std::min(val1, val2);
#else
  return val1 < val2 ? val1 : val2;
#endif
}

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Max(T const &val1, T const &val2) {
#ifndef VECCORE_NVCC_DEVICE
  return std::max(val1, val2);
#else
  return val1 > val2 ? val1 : val2;
#endif
}

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Sin(T radians) {
  return std::sin(radians);
}

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Cos(T radians) {
  return std::cos(radians);
}

// sincos() is not part of math.h on Mac OS

#ifdef __APPLE__
#	define sincos(x)  __sincos(x)
#	define sincosf(x) __sincosf(x)
#endif

VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
void SinCos(float radians, float *s, float *c) {
    sincosf(radians, s, c);
}

VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
void SinCos(double radians, double *s, double *c) {
    sincos(radians, s, c);
}

#ifdef __APPLE__
#	undef sincos
#	undef sincosf
#endif

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Tan(T radians) {
  return std::tan(radians);
}

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Floor( T val ){
    return std::floor(val);
}

//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//void swap(Type &a, Type &b) {
//  std::swap(a, b);
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//void copy(Type const *begin, Type const *const end, Type *const target) {
//#ifndef VECCORE_NVCC_DEVICE
//  std::copy(begin, end, target);
//#else
//  std::memcpy(target, begin, sizeof(Type)*(end-begin));
//#endif
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//void reverse_copy(Type const *const begin, Type const *end,
//                  Type *const target) {
//#ifndef VECCORE_NVCC_DEVICE
//  std::reverse_copy(begin, end, target);
//#else
//  while (--end >= begin) *target++ = *end;
//#endif
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//void reverse(Type *begin, Type *end) {
//#ifndef VECCORE_NVCC_DEVICE
//  std::reverse(begin, end);
//#else
//  while (begin++ < end--) swap(begin, end);
//#endif
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type* AlignedAllocate(size_t size) {
//#ifndef VECCORE_NVCC
//  return static_cast<Type*>(_mm_malloc(sizeof(Type)*size, kAlignmentBoundary));
//#else
//  return new Type[size];
//#endif
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//void AlignedFree(Type *allocated) {
//#ifndef VECCORE_NVCC
//  _mm_free(allocated);
//#else
//  delete[] allocated;
//#endif
//}
//
//template <typename IteratorType>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//IteratorType min_element(IteratorType first, IteratorType last) {
//  return std::min_element(first, last);
//}
//
//template <typename IteratorType>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//bool all_of(IteratorType first, IteratorType last) {
//#ifdef VECCORE_STD_CXX11
//  return std::all_of(first, last, [](bool b){return b;});
//#else
//  while (first < last) {
//    if (!(*first)) return false;
//    ++first;
//  }
//  return true;
//#endif
//}
//
//template <typename InputIterator1, typename InputIterator2>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//bool equal(InputIterator1 first, InputIterator1 last, InputIterator2 target) {
//#ifndef VECCORE_NVCC_DEVICE
//  return std::equal(first, last, target);
//#else
//  while (first != last) {
//    if (*first++ != *target++) return false;
//  }
//  return true;
//#endif
//}

} // End inline namespace

} // End scalar namespace

} // End Backend namespace

} // End global namespace

#endif // VECCORE_BACKEND_SCALARBACKEND_H_
