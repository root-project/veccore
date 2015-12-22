#ifndef VECCORE_BACKEND_H
#define VECCORE_BACKEND_H

#include "Common.h"
#include "Limits.h"

#include "Backend/Basic.h"
#include "Backend/CUDA.h"

#ifndef __CUDACC__
#include "Backend/VcScalar.h"
#include "Backend/VcVector.h"
#include "Backend/VcSimdArray.h"
#endif

// default backends

namespace vecCore {
namespace backend {
template <typename T> using Scalar = Basic<T>;
template <typename T> using Vector = Basic<T>;
} // namespace backend
} // namespace vecCore
#endif
