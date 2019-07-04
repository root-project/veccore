#ifndef VECCORE_SIMD_H
#define VECCORE_SIMD_H

#if defined(VECCORE_X64) || defined(VECCORE_X86)
#  ifdef _MSC_VER
#    include <intrin.h>
#  else
#    include <x86intrin.h>
#  endif
#endif

#if defined(__AVX512F__) || defined(__MIC__)
#define VECCORE_SIMD_ALIGN 64
#elif defined(__AVX__)
#define VECCORE_SIMD_ALIGN 32
#else
#define VECCORE_SIMD_ALIGN 16
#endif

#endif
