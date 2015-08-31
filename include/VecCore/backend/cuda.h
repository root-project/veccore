#ifndef VECCORE_CUDA_BACKEND_H
#define VECCORE_CUDA_BACKEND_H

#ifdef VECCORE_CUDA

#include <VecCore/backend/scalar.h>

namespace VecCore {
namespace Backend {

/* CUDABackend is simply a scalar backend that is not asynchronous */

template <typename T> using CUDA = Scalar<T, false>;

}
}

#endif
#endif
