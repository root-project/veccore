#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include <type_traits>
#ifndef VECGEOM_ASSERT_H
#include <cassert>
#endif

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

template <class Mask> Bool_s MaskEmpty(const Mask &mask);
template <class Mask> Bool_s MaskFull(const Mask &mask);

template <class T, class Mask>
void MaskedAssign(T &dest, const Mask &mask, const T &src);

template <class T, class Mask>
T Blend(const Mask &mask, const T &tval, const T &fval);

// construct a type from a pointer - generic impl for vector types
// may be template specialized in backends
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<!std::is_scalar<T>::value, T>::type
FromPtr(typename TypeTraits<T>::ScalarType const *x)
{
  return T(x);
}

// construct a type from a pointer - generic impl for scalar types
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<std::is_scalar<T>::value, T>::type
FromPtr(typename TypeTraits<T>::ScalarType const *x)
{
  return T(*x);
}

// store type to an address destination (pointer to scalar type)- generic impl for vector types
// may be template specialized in backends
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void Store(const T &x, typename std::enable_if<!std::is_scalar<T>::value,
                                               typename TypeTraits<T>::ScalarType>::type *dest)
{
  x.store(dest);
}

// store to an address destination - generic impl for scalar types
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void Store(const T& x, typename std::enable_if<std::is_scalar<T>::value,
                                               typename TypeTraits<T>::ScalarType>::type *dest)
{
  *dest = x;
}

// indexed *read* lane accesses for vector and mask types

// lane access to scalar types - trivial implementation
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type
LaneAt(const T &x, size_t index)
{
  assert(index == 0);
  (void)index;
  return x;
}

// lane access to vector types - generic implementation
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<!std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type
LaneAt(const T &x, size_t index)
{
  assert(index < VectorSize<T>());
  (void)index;
  return x[index];
}

// per lane assignment to vector types - generic implementation
template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void
AssignLane(T &x, size_t index, typename std::enable_if<std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type const &y)
{
  assert(index == 0);
  (void)index;
  x = y;
}

template <typename T>
VECCORE_FORCE_INLINE VECCORE_CUDA_HOST_DEVICE
void AssignLane(T &x, size_t index,
    typename std::enable_if<!std::is_scalar<T>::value, typename TypeTraits<T>::ScalarType>::type const &y)
{
  assert(index < VectorSize<T>());
  x[index] = y;
}

// per lane assignment to mask types
template <typename T>
VECCORE_FORCE_INLINE VECCORE_CUDA_HOST_DEVICE
void AssignMaskLane(T &x, size_t index, typename std::enable_if<!std::is_scalar<T>::value, bool>::type const &y)
{
  x[index] = y;
}


VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void AssignMaskLane(bool &x, size_t index, bool const &y)
{
  assert(index == 0);
  (void)index;
  x = y;
}



// lane access to masks of vector types - generic implementation
template <typename Mask>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
bool MaskLaneAt(const Mask &x, size_t index)
{
  return x[index];
}

// for scalar mask case, a simple template specialization is good
template <>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
bool MaskLaneAt<bool>(const bool &x, size_t index)
{
  assert(index == 0);
  (void)index;
  return x;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<std::is_scalar<T>::value,T>::type
Gather(typename TypeTraits<T>::ScalarType const *ptr,
       typename TypeTraits<T>::IndexType  const &idx)
{
  return *(ptr + idx);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<!std::is_scalar<T>::value,T>::type
Gather(typename TypeTraits<T>::ScalarType const *ptr,
       typename TypeTraits<T>::IndexType  const &idx)
{
  T result;
  for (size_t i = 0; i < VectorSize<T>(); i++)
    result[i] = ptr[idx[i]];
  return result;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<std::is_scalar<T>::value,void>::type
Scatter(const T& x, typename TypeTraits<T>::ScalarType *ptr,
        typename TypeTraits<T>::IndexType  const &idx)
{
  ptr[idx] = x;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
typename std::enable_if<!std::is_scalar<T>::value,void>::type
Scatter(const T& x, typename TypeTraits<T>::ScalarType *ptr,
        typename TypeTraits<T>::IndexType  const &idx)
{
  for (size_t i = 0; i < VectorSize<T>(); i++)
    ptr[idx[i]] = x[i];
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
