#ifndef VECCORE_BACKEND_VECTOR_H
#define VECCORE_BACKEND_VECTOR_H

#include "SIMD.h"

#ifdef VECCORE_ENABLE_SIMD

// default to Vc backend

#include "Backend/Vc.h"

#else

// fallback to scalar backend if SIMD is disabled

#include "Backend/Scalar.h"

namespace vecCore {
namespace backend {
template <typename T> using Vector = Scalar<T, true>;
} // namespace backend
} // namespace vecCore

#endif
#endif
