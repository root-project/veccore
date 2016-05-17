#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include <type_traits>
#include <cassert>

namespace vecCore {

template <typename T> struct TypeTraits;

// backend interface

template <typename T> using Mask_v  = typename TypeTraits<T>::MaskType;
template <typename T> using Index_v = typename TypeTraits<T>::IndexType;

template <typename T> struct ScalarType {
  using Type = typename TypeTraits<T>::ScalarType;
};

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
constexpr Size_s VectorSize()
{
  return sizeof(T)/sizeof(typename ScalarType<T>::Type);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
constexpr Size_s VectorSize(const T&)
{
  return sizeof(T)/sizeof(typename ScalarType<T>::Type);
}

template <class Mask> Bool_s MaskEmpty(const Mask mask);
template <class Mask> Bool_s MaskFull(const Mask mask);

template <class T, class Mask>
void MaskedAssign(T& dest, Mask mask, const T &src);

template <class T, class Mask>
T Blend(const Mask mask, const T& tval, const T& fval);

// construct a type from a pointer - generic impl for vector types
// may be template specialized in backends
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<!std::is_scalar<T>::value, T>::type FromPtr(typename TypeTraits<T>::ScalarType const *x) {
  return T(x);
}


// construct a type from a pointer - generic impl for scalar types
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<std::is_scalar<T>::value, T>::type FromPtr(typename TypeTraits<T>::ScalarType const *x) {
  return T(*x);
}

// store type to an address destination (pointer to scalar type)- generic impl for vector types
// may be template specialized in backends
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void Store(T x, typename std::enable_if<!std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type *dest) {
  x.store(dest);
}

// store to an address destination - generic impl for scalar types
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void Store(T x, typename std::enable_if<std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type *dest) {
  *dest = x;
}

// indexed *read* lane accesses for vector and mask types

// lane access to scalar types - trivial implementation
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type
LaneAt(const T& x, size_t index)
{
  assert(index == 0);
  return x;
}

// lane access to vector types - generic implementation
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<!std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type
LaneAt(const T& x, size_t index)
{
  assert(index < VectorSize<T>());
  return x[index];
}

// lane access to masks of vector types - generic implementation
template <typename Mask>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
bool MaskLaneAt(Mask x, size_t index) {
  return x[index];
}

// for scalar mask case, a simple template specialization is good
template <>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
bool MaskLaneAt<bool>(bool x, size_t index) {
  assert(index == 0);
  return x;
}

// extra functions

VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
constexpr Bool_s EarlyReturnAllowed()
{
#ifdef VECCORE_NVCC_DEVICE
  return false;
#else
  return true;
#endif
}

}

#include "Backend/Scalar.h"
#include "Backend/ScalarWrapper.h"

#if !defined(VECCORE_NVCC)
#include "Backend/SIMDSizes.h"
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"
#include "Backend/VcSimdArray.h"
#include "Backend/UMESimd.h"
#include "Backend/UMESimdArray.h"
#endif

#endif
