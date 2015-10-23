#ifndef VECCORE_CUDA_BACKEND_H
#define VECCORE_CUDA_BACKEND_H

#if defined(VECCORE_ENABLE_CUDA)

#include "Backend/Scalar.h"

namespace veccore {
namespace backend {
template <typename T> using CUDA = Scalar<T, false>;
} // namespace backend
} // namespace veccore

#endif
#endif
