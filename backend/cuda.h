#ifndef VECCORE_CUDA_BACKEND_H
#define VECCORE_CUDA_BACKEND_H

#ifdef VECCORE_CUDA

#include <backend/scalar.h>

namespace VecCore {
namespace Backend {

/* CUDABackend is simply a ScalarBackend that is not asynchronous */

template <typename T> class CUDA : public Scalar<T, false> {}

}
}

#endif
#endif
