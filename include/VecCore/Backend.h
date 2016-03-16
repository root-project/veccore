#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

namespace vecCore {

template <typename T> struct ScalarType;

// backend interface

template <typename T>
constexpr Size_t VectorSize()
{
  return sizeof(T)/sizeof(typename ScalarType<T>::Type);
}

template <typename T>
constexpr Size_t VectorSize(const T&)
{
  return sizeof(T)/sizeof(typename ScalarType<T>::Type);
}

template <class Mask> Bool_t IsEmpty(const Mask mask);
template <class Mask> Bool_t IsFull(const Mask mask);

template <class T, class Mask>
void MaskedAssign(T& dest, bool mask, const T &src);

template <class T, class Mask>
T Blend(const Mask mask, const T& tval, const T& fval);

// extra functions

VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
constexpr Bool_t EarlyReturnAllowed()
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
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"
#include "Backend/VcSimdArray.h"
#endif

#endif
