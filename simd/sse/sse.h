#ifndef VECCORE_SIMD_SSE_H
#define VECCORE_SIMD_SSE_H

#include <core/simd.h>

#ifdef __SSE4_1__

namespace VecCore {
namespace SIMD {
namespace SSE {

VECCORE_FORCE_INLINE
__m128 Neg(const __m128& m)
{
    return _mm_xor_ps(m, _mm_set1_ps(-0.0f));
}

VECCORE_FORCE_INLINE
__m128 Abs(const __m128& m)
{
    return _mm_andnot_ps(m, _mm_set1_ps(-0.0f));
}

VECCORE_FORCE_INLINE
__m128 Dot3(const __m128& a, const __m128& b)
{
    return _mm_dp_ps(a, b, 0x7F);
}

VECCORE_FORCE_INLINE
__m128 Dot4(const __m128& a, const __m128& b)
{
    return _mm_dp_ps(a, b, 0xFF);
}

VECCORE_FORCE_INLINE
__m128 Sgn(const __m128& m)
{
    __m128 cmp0 = _mm_cmplt_ps(m, _mm_setzero_ps());
    __m128 cmp1 = _mm_cmpgt_ps(m, _mm_setzero_ps());
    __m128 and0 = _mm_and_ps(cmp0, _mm_set1_ps(-1.0f));
    __m128 and1 = _mm_and_ps(cmp1, _mm_set1_ps(+1.0f));
    return _mm_or_ps(and0, and1);
}

/* vector cross product, for vectors as (x, y, z, 0) */

VECCORE_FORCE_INLINE
__m128 Cross3(const __m128& a, const __m128& b)
{
    __m128 tmp1 = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3,0,2,1));
    __m128 tmp2 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3,1,0,2));
    __m128 tmp3 = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3,1,0,2));
    __m128 tmp4 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3,0,2,1));
    return _mm_sub_ps(_mm_mul_ps(tmp1,tmp2), _mm_mul_ps(tmp3,tmp4));

    /* operations: 4 shuffles, 2 multiplications, 1 subtraction */
}

/* quaternion product of two quaternions (x, y, z, w) x (a, b, c, d) */

VECCORE_FORCE_INLINE
__m128 Cross4(const __m128& xyzw, const __m128& abcd)
{
    /* the product of the two quaternions is: */
    /* (X,Y,Z,W) = (xd+yc-zb+wa, -xc+yd+za+wb, xb-ya+zd+wc, -xa-yb-zc+wd) */

    __m128 wzyx = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(0,1,2,3));
    __m128 baba = _mm_shuffle_ps(abcd, abcd, _MM_SHUFFLE(0,1,0,1));
    __m128 dcdc = _mm_shuffle_ps(abcd, abcd, _MM_SHUFFLE(2,3,2,3));

    /* variable names below are for componens of result (X,Y,Z,W), nX for -X */

    /* znxwy  = (xb - ya, zb - wa, wd - zc, yd - xc) */
    __m128 ZnXWY = _mm_hsub_ps(_mm_mul_ps(xyzw, baba), _mm_mul_ps(wzyx, dcdc));

    /* xzynw  = (xd + yc, zd + wc, wb + za, yb + xa) */
    __m128 XZYnW = _mm_hadd_ps(_mm_mul_ps(xyzw, dcdc), _mm_mul_ps(wzyx, baba));

    /* _mm_shuffle_ps(XZYnW, ZnXWY, _MM_SHUFFLE(3,2,1,0)) = (xd + yc, zd + wc, wd - zc, yd - xc) */
    /* _mm_shuffle_ps(ZnXWY, XZYnW, _MM_SHUFFLE(2,3,0,1)) = (zb - wa, xb - ya, yb + xa, wb + za) */

    /* _mm_addsub_ps adds elements 1 and 3 and subtracts elements 0 and 2, so we get: */
    /* _mm_addsub_ps(*, *) = (xd+yc-zb+wa, xb-ya+zd+wc, wd-zc+yb+xa, yd-xc+wb+za)     */

    __m128 XZWY = _mm_addsub_ps(_mm_shuffle_ps(XZYnW, ZnXWY, _MM_SHUFFLE(3,2,1,0)),
                                _mm_shuffle_ps(ZnXWY, XZYnW, _MM_SHUFFLE(2,3,0,1)));

    /* now we shuffle components in place and return the result */
    return _mm_shuffle_ps(XZWY, XZWY, _MM_SHUFFLE(2,1,3,0));

    /* operations: 6 shuffles, 4 multiplications, 3 compound additions/subtractions */
}

/* transpose a 4x4 matrix */

VECCORE_FORCE_INLINE
void Transpose4(__m128* mat)
{
    __m128 tmp[4];
    tmp[0] = _mm_unpacklo_ps(mat[0], mat[1]);
    tmp[1] = _mm_unpacklo_ps(mat[2], mat[3]);
    tmp[2] = _mm_unpackhi_ps(mat[0], mat[1]);
    tmp[3] = _mm_unpackhi_ps(mat[2], mat[3]);
    mat[0] = _mm_movelh_ps(tmp[0], tmp[1]);
    mat[1] = _mm_movehl_ps(tmp[1], tmp[0]);
    mat[2] = _mm_movelh_ps(tmp[2], tmp[3]);
    mat[3] = _mm_movehl_ps(tmp[3], tmp[2]);
}

/* 4x4 matrix times a 4 vector */

VECCORE_FORCE_INLINE
__m128 Mat4Mul(const __m128* mat, const __m128& v)
{
    __m128 x = _mm_dp_ps(mat[0], v, 0xF1);
    __m128 y = _mm_dp_ps(mat[1], v, 0xF1);
    __m128 z = _mm_dp_ps(mat[2], v, 0xF1);
    __m128 w = _mm_dp_ps(mat[3], v, 0xF1);
    return _mm_movelh_ps(_mm_unpacklo_ps(x,y), _mm_unpacklo_ps(z,w));
}

/* 3x3 matrix times a 3 vector + offset */

VECCORE_FORCE_INLINE
__m128 Mat3MAdd(const __m128* mat, const __m128& v, const __m128& offset)
{
    __m128 x = _mm_dp_ps(mat[0], v, 0x71);
    __m128 y = _mm_dp_ps(mat[1], v, 0x71);
    __m128 z = _mm_dp_ps(mat[2], v, 0x71);
    return _mm_add_ps(_mm_movelh_ps(_mm_unpacklo_ps(x, y), z), offset);
}

} /* namespace SSE */
} /* namespace SIMD */
} /* namespace VecCore */

#endif

#endif
