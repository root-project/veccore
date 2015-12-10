#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include "Common.h"
#include "Limits.h"

#include "Backend/Basic.h"
#include "Backend/CUDA.h"
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"
#include "Backend/VcSimdArray.h"

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
} // namespace backend
} // namespace vecCore
#endif
