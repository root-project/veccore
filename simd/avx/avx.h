#ifndef VECCORE_SIMD_AVX_H
#define VECCORE_SIMD_AVX_H

#include <core/simd.h>

#ifdef __AVX__

namespace VecCore {
namespace SIMD {
namespace AVX {

VECCORE_FORCE_INLINE
__m256d Neg(const __m256d& m)
{
    return _mm256_xor_pd(m, _mm256_set1_pd(-0.0));
}

VECCORE_FORCE_INLINE
__m256d Abs(const __m256d& m)
{
    return _mm256_andnot_pd(m, _mm256_set1_pd(-0.0));
}

VECCORE_FORCE_INLINE
__m256d Sgn(const __m256d& m)
{
    __m256d lt = _mm256_cmp_pd(m, _mm256_setzero_pd(), _CMP_LT_OS);
    __m256d gt = _mm256_cmp_pd(m, _mm256_setzero_pd(), _CMP_GT_OS);
    __m256d and0 = _mm256_and_pd(lt, _mm256_set1_pd(-1.0));
    __m256d and1 = _mm256_and_pd(gt, _mm256_set1_pd(+1.0));
    return _mm256_or_pd(and0, and1);
}

VECCORE_FORCE_INLINE
__m256d FMAdd(const __m256d& a, const __m256d& x, const __m256d& y)
{
#ifdef VECCORE_SIMD_FMA
    return _mm256_fmadd_pd(a, x, y);
#else
    return _mm256_add_pd(_mm256_mul_pd(a, x), y);
#endif
}

VECCORE_FORCE_INLINE
__m256d FMSub(const __m256d& a, const __m256d& x, const __m256d& y)
{
#ifdef VECCORE_SIMD_FMA
    return _mm256_fmsub_pd(a, x, y);
#else
    return _mm256_sub_pd(_mm256_mul_pd(a, x), y);
#endif
}

VECCORE_FORCE_INLINE
__m256d Dot4(const __m256d& a, const __m256d& b)
{
    __m256d tmp0 = _mm256_mul_pd(a, b);
    __m256d tmp1 = _mm256_permute2f128_pd(tmp0, tmp0, 0x1);
    __m256d tmp2 = _mm256_hadd_pd(tmp0, tmp1);
    return _mm256_hadd_pd(tmp2, tmp2);
}

VECCORE_FORCE_INLINE
__m256d Dot3(const __m256d& a, const __m256d& b)
{
    return Dot4(_mm256_blend_pd(a, _mm256_setzero_pd(), 0x8), b);
}

VECCORE_FORCE_INLINE
__m256d Shuffle3102(const __m256d& xyzw)
{
    __m256d yxzw = _mm256_permute_pd(xyzw, 0x9);
    __m256d zwyx = _mm256_permute2f128_pd(yxzw, yxzw, 0x1);
    return _mm256_blend_pd(yxzw, zwyx, 0xA); /* returns zxyw */
}

VECCORE_FORCE_INLINE
__m256d Cross3(const __m256d& a, const __m256d& b)
{
    __m256d a012 = a, b012 = b, a201, b201, tmp;
#ifndef __AVX2__
    a201 = Shuffle3102(a);
    b201 = Shuffle3102(b);
     tmp = Shuffle3102(FMSub(b012, a201, _mm256_mul_pd(a012, b201)));
#else
    a201 = _mm256_permute4x64_pd(a, _MM_SHUFFLE(3,1,0,2));
    b201 = _mm256_permute4x64_pd(b, _MM_SHUFFLE(3,1,0,2));
     tmp = FMSub(b012, a201, _mm256_mul_pd(a012, b201));
     tmp = _mm256_permute4x64_pd(tmp, _MM_SHUFFLE(3,1,0,2));
#endif
    return _mm256_blend_pd(_mm256_setzero_pd(), tmp, 0x7);
}

} /* namespace AVX */
} /* namespace SIMD */
} /* namespace VecCore */

#endif
#endif
