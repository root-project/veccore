#ifndef VECCORE_COMMON_H
#define VECCORE_COMMON_H

#include <cstddef>
#include <cstdint>

#ifndef VECCORE_NO_FORCED_INLINING
#define VECCORE_FORCE_NOINLINE __attribute__((noinline))
#define VECCORE_FORCE_INLINE inline __attribute__((always_inline))
#else
#define VECCORE_FORCE_NOINLINE
#define VECCORE_FORCE_INLINE
#endif

#include "CUDA.h"
#include "SIMD.h"

namespace vecCore {

// Masking Interface

// IsFull

template <class Mask>
VECCORE_FORCE_INLINE
bool IsFull(const Mask mask)
{
  return mask.isFull();
}

template <>
VECCORE_FORCE_INLINE
bool IsFull<bool>(const bool mask)
{
  return mask;
}

// IsEmpty

template <class Mask>
VECCORE_FORCE_INLINE
bool IsEmpty(const Mask& mask)
{
  return mask.isEmpty();
}

template <>
VECCORE_FORCE_INLINE
bool IsEmpty<bool>(const bool& mask)
{
  return !mask;
}

// MaskedAssign

template <typename T>
VECCORE_FORCE_INLINE
void MaskedAssign(bool mask, const T &val, T *const var)
{
  if (mask)
    var = val;
}

template <class T, class Mask>
VECCORE_FORCE_INLINE
void MaskedAssign(const Mask &mask, const T &val, T *const var)
{
  (*var)(mask) = val;
}

// CondAssign (for backward-compatibility, deprecated by Blend below)

template <typename T>
VECCORE_FORCE_INLINE
void CondAssign(const bool &mask, const T &tval, const T &fval, T *const var)
{
  (*var) = mask ? tval : fval;
}

template <class T, class Mask>
VECCORE_FORCE_INLINE
void CondAssign(const Mask &mask, const T &tval, const T &fval, T *const var)
{
  (*var) = fval;
  (*var)(mask) = tval;
}

// Blend

template <typename T>
VECCORE_FORCE_INLINE
T Blend(const bool& mask, const T& tval, const T& fval)
{
  return mask ? tval : fval;
}

template <class T, class Mask>
VECCORE_FORCE_INLINE
T Blend(const Mask& mask, const T& tval, const T& fval)
{
  T tmp = fval;
  tmp(mask) = tval;
  return tmp;
}

} // namespace vecCore
#endif
