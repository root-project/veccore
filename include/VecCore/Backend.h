#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include <type_traits>

#include "Common.h"

#include "Backend/CUDA.h"
#include "Backend/Scalar.h"
#include "Backend/Vector.h"

template <class T, class Mask>
VECCORE_FORCE_INLINE
void MaskedAssign(T& var, const Mask& m, const T val)
{
	var(m) = val;
}

template <>
VECCORE_FORCE_INLINE
void MaskedAssign<float, bool>(float& var, const bool& m, const float val)
{
    if (m) var = val;
}

template <>
VECCORE_FORCE_INLINE
void MaskedAssign<double, bool>(double& var, const bool& m, const double val)
{
    if (m) var = val;
}

template <class T, class Mask>
VECCORE_FORCE_INLINE
T Blend(const Mask& m, const T val1, const T val2)
{
    T result(val2);
    MaskedAssign(result, m, val1);
    return result;
}

template <class Mask>
VECCORE_FORCE_INLINE
bool IsFull(const Mask mask) { return mask.isFull(); }

template <>
VECCORE_FORCE_INLINE
bool IsFull<bool>(const bool mask) { return mask; }

#endif
