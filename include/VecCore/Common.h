#ifndef VECCORE_COMMON_H
#define VECCORE_COMMON_H

#include "CUDA.h"
#include "SIMD.h"

#ifdef _MSC_VER
#define __restrict__ __restrict
#endif

#ifndef VECCORE_NO_FORCED_INLINING
#ifdef _MSC_VER
#define VECCORE_FORCE_NOINLINE __declspec(noinline)
#define VECCORE_FORCE_INLINE inline __forceinline
#else
#define VECCORE_FORCE_NOINLINE __attribute__((noinline))
#define VECCORE_FORCE_INLINE inline __attribute__((always_inline))
#endif
#else
#define VECCORE_FORCE_NOINLINE
#define VECCORE_FORCE_INLINE
#endif

#endif
