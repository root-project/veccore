/// \file scalar/Backend.h

#ifndef VECCORE_BACKEND_SCALARBACKEND_H_
#define VECCORE_BACKEND_SCALARBACKEND_H_

#include "VecCoreGlobal.h"

#include <algorithm>
#include <cstring>
#include <cassert>

namespace VecCore {

namespace Backend {

namespace Scalar {

inline namespace VECCORE_IMPL_NAMESPACE {

template< typename RealInput_t = double  >
struct kScalar {
  // define the base primitive type
  typedef RealInput_t                 Real_t;
  // we could define the other base primitive types
  typedef double                      Double_t;
  typedef double                      Float_t;

  typedef Real_t                      Real_v;
  typedef bool                        Bool_v;

  typedef double                      Double_v;
  typedef float                       Float_v;
  typedef bool                        DoubleBool_v;
  typedef bool                        FloatBool_v;

  typedef int       Int_v;
  typedef Int_v     Inside_v;
  typedef bool      IntBool_v;

  typedef int  Index_t; // the type of indices

  // this flag is probably the most important
  // distinction between kScalar and kCuda ??
  const static bool early_returns = true;

  // alternative typedefs ( might supercede above typedefs )
  typedef int                   Int_t;

  constexpr static Real_v kOne = 1.0;
  constexpr static Real_v kZero = 0.0;
  constexpr static Bool_v kTrue = true;
  constexpr static Bool_v kFalse = false;

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
};

//template <>
//VECCORE_CUDA_HEADER_BOTH
//inline VECCORE_CONSTEXPR_RETURN bool kScalar::IsEqual<kScalar>() {
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

// same for Mask
VECCORE_INLINE
static
bool const
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

//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//bool IsFull(bool const &cond){
//    return cond;
//}
//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//bool Any(bool const &cond) {
//  return cond;
//}
//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//bool IsEmpty(bool const &cond){
//    return !cond;
//}
//
//
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type Abs(const Type val) {
//  return fabs(val);
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type Sqrt(const Type val) {
//  return std::sqrt(val);
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type Pow(const Type val1, const Type val2) {
//  return std::pow(val1, val2);
//}
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type Pow(Type const &x, Type arg) {
//   return pow(x,arg);
//}
//
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type Pow(Type const &x, int arg) {
//   return pow(x,arg);
//}
//
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type Cbrt(const Type val1) {
//  return cbrt(val1);
//}
//
//
//template <typename Type>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Type ATan2(const Type y, const Type x) {
//  if (x != 0) return  std::atan2(y, x);
//  if (y >  0) return  kPi / 2;
//  if (y <  0) return -kPi / 2;
//  return  0;
//}
//
//template <typename T>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//T Min(T const &val1, T const &val2) {
//#ifndef VECCORE_NVCC_DEVICE
//  return std::min(val1, val2);
//#else
//  return val1 < val2 ? val1 : val2;
//#endif
//}
//
//template <typename T>
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//T Max(T const &val1, T const &val2) {
//#ifndef VECCORE_NVCC_DEVICE
//  return std::max(val1, val2);
//#else
//  return val1 > val2 ? val1 : val2;
//#endif
//}
//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Precision sin(const Precision radians) {
//  return std::sin(radians);
//}
//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Precision cos(const Precision radians) {
//  return std::cos(radians);
//}
//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Precision tan(const Precision radians) {
//  return std::tan(radians);
//}
//
//VECCORE_CUDA_HEADER_BOTH
//VECCORE_INLINE
//Precision Floor( Precision val ){
//    return std::floor(val);
//}
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
