#ifndef VECCORE_VECTOR_BACKEND_H
#define VECCORE_VECTOR_BACKEND_H

#ifndef VECCORE_DISABLE_SIMD

// need to make a switching mechanism to other vector backends
// here when they become available (native, other libraries, etc)

#include "core/simd.h"
#include "backend/vc.h"

#else

// fallback to scalar backend if SIMD is disabled

#include "backend/scalar.h"

namespace VecCore { namespace Backend {

template <typename T> using Vector = Scalar<T, true>;

} }

#endif
#endif
