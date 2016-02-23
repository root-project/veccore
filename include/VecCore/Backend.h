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
using Scalar = Basic;
using Vector = Basic;
#elif defined(VECCORE_ENABLE_VC)
using Scalar = VcScalar;
using Vector = VcVector;
#endif
} // namespace backend
} // namespace vecCore
#endif
