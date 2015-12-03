#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include <type_traits>

#include "Common.h"

#include "Backend/Basic.h"
#include "Backend/CUDA.h"
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"

// default backends

namespace vecCore {
namespace backend {
#ifndef VECCORE_ENABLE_VC
	template <typename T> using Scalar = Basic<T>;
	template <typename T> using Vector = Basic<T>;
#else
	template <typename T> using Scalar = VcScalar<T>;
	template <typename T> using Vector = VcVector<T>;
#endif
}

}

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

template <class Mask>
VECCORE_FORCE_INLINE
bool IsEmpty(const Mask mask) { return mask.isEmpty(); }

template <>
VECCORE_FORCE_INLINE
bool IsEmpty<bool>(const bool mask) { return !mask; }

#endif
