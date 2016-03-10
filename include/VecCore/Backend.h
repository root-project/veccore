#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

namespace vecCore {
// backend interface

template <class Mask> Bool_t IsEmpty(const Mask mask);
template <class Mask> Bool_t IsFull(const Mask mask);

template <class T, class Mask>
void MaskAssign(T& dest, bool mask, const T &src);

template <class T, class Mask>
T Blend(const Mask mask, const T& tval, const T& fval);

// extra backend functions

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

#include "Backend/Basic.h"
#include "Backend/Scalar.h"
#include "Backend/CUDA.h"

#if !defined(VECCORE_NVCC)
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"
#include "Backend/VcSimdArray.h"
#endif

#endif
