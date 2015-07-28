#ifndef VECCORE_SIMD_H
#define VECCORE_SIMD_H

#include <core/macros.h>

#if !defined(VECCORE_DISABLE_SIMD)
#   if defined(__SSE__) || defined(__AVX__) || defined(__MIC__)
#       define VECCORE_ENABLE_SIMD
#   endif
#endif

#if defined(VECCORE_ENABLE_SIMD)
#   include <x86intrin.h>
#   if defined(__MIC__)
#       define VECCORE_SIMD_MIC
#       define VECCORE_SIMD_ALIGN 64
#   elif defined(__AVX__)
#       define VECCORE_SIMD_AVX
#       define VECCORE_SIMD_ALIGN 32
#   elif defined(__SSE__)
#       define VECCORE_SIMD_SSE
#       define VECCORE_SIMD_ALIGN 16
#   endif
#endif

#endif
