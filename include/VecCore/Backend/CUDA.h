#ifndef VECCORE_CUDA_BACKEND_H
#define VECCORE_CUDA_BACKEND_H

#if defined(VECCORE_ENABLE_CUDA)

#include "Backend/Basic.h"

namespace vecCore {
namespace backend {
template <typename T> using CUDA = Basic<T, false>;
} // namespace backend
} // namespace vecCore

#endif
#endif
