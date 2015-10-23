#ifndef VECCORE_VECTOR_BACKEND_H
#define VECCORE_VECTOR_BACKEND_H

#include "core/simd.h"

#ifdef VECCORE_ENABLE_SIMD

// default to Vc backend

#include "backend/vc.h"

#else

// fallback to scalar backend if SIMD is disabled

#include "backend/scalar.h"

namespace VecCore { namespace Backend {

template <typename T> using Vector = Scalar<T, true>;

} }

#endif
#endif
