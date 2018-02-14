#ifndef VECRNG_RNGDEFS_H
#define VECRNG_RNGDEFS_H 1

#include <VecCore/VecCore>

namespace vecRng {

using namespace vecCore;

using ScalarBackend = backend::Scalar;
#if defined(VECCORE_CUDA)
using VectorBackend = backend::Scalar;
#else
#if defined(VECCORE_ENABLE_VC)
using VectorBackend = backend::VcVector;
#elif defined(VECCORE_ENABLE_UMESIMD)
using VectorBackend = backend::UMESimd;
#else
using VectorBackend = backend::Scalar;
#endif
#endif

// scalar types

using Size_t = vecCore::Size_s;

using Real_t = vecCore::Real_s;
using Float_t = vecCore::Float_s;
using Double_t = vecCore::Double_s;

using Int_t = vecCore::Int_s;
using Int16_t = vecCore::Int16_s;
using Int32_t = vecCore::Int32_s;
using Int64_t = vecCore::Int64_s;

using UInt_t = vecCore::UInt_s;
using UInt16_t = vecCore::UInt16_s;
using UInt32_t = vecCore::UInt32_s;
using UInt64_t = vecCore::UInt64_s;

// vector types

using Real_v = typename VectorBackend::Real_v;
using Float_v = typename VectorBackend::Float_v;
using Double_v = typename VectorBackend::Double_v;

using Int_v = typename VectorBackend::Int_v;
using Int16_v = typename VectorBackend::Int16_v;
using Int32_v = typename VectorBackend::Int32_v;
using Int64_v = typename VectorBackend::Int64_v;

using UInt_v = typename VectorBackend::UInt_v;
using UInt16_v = typename VectorBackend::UInt16_v;
using UInt32_v = typename VectorBackend::UInt32_v;
using UInt64_v = typename VectorBackend::UInt64_v;

} // end namespace vecRng

#if defined(VECCORE_CUDA)
#define VECRNG_IMPL_NAMESPACE cuda
#else
#define VECRNG_IMPL_NAMESPACE cxx
#endif

#ifdef VECCORE_CUDA_DEVICE_COMPILATION
#define VECRNG_GLOBAL static __constant__ const
#else
#define VECRNG_GLOBAL constexpr
#endif

#ifdef VECCORE_CUDA
#include <cuda.h>
#include <curand_kernel.h>
typedef curandState Random_t;
#else
typedef int Random_t;
#endif

#endif // VECRNG_RNGDEFS_H
