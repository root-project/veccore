#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include "Common.h"
#include "Limits.h"

#include "Backend/Basic.h"
#include "Backend/CUDA.h"

#if !defined(VECCORE_NVCC)
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"
#include "Backend/VcSimdArray.h"
#endif

// default backends

namespace vecCore {
namespace backend {

#if defined(VECCORE_NVCC) || defined(VECCORE_DISABLE_SIMD)
template <typename T> using Scalar = Basic<T>;
template <typename T> using Vector = Basic<T>;
#elif defined(VECCORE_ENABLE_VC)
template <typename T> using Scalar = VcScalar<T>;
template <typename T> using Vector = VcVector<T>;
#endif
} // namespace backend
} // namespace vecCore
#endif
