#ifndef VECCORE_COMMON_H
#define VECCORE_COMMON_H

#if defined(__x86_64__) || defined(_M_X64)
#define VECCORE_X64
#elif defined(__i386__) || defined(_M_IX86)
#define VECCORE_X86
#elif defined(__aarch64__) || defined(_M_ARM64)
#define VECCORE_ARM64
#elif defined(__arm__) || defined(_M_ARM)
#define VECCORE_ARM
#elif defined(__PPC64__)
#define VECCORE_PPC64
#endif

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
