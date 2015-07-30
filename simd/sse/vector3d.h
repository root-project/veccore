#ifndef VECCORE_VECTOR3D_SSE_H
#define VECCORE_VECTOR3D_SSE_H

#include <core/util.h>
#include <simd/sse/sse.h>

#if defined(__SSE4_1__)
namespace VecCore {
namespace SIMD {
namespace SSE {

template <typename T> class Vector3D;

template<> class Vector3D<float> {
  public:
    inline Vector3D<float>() : fData(_mm_setzero_ps()) { }

    inline Vector3D<float>(float s) : fData(_mm_set1_ps(s)) { }

    inline Vector3D<float>(float x, float y, float z) : fData(_mm_set_ps(0, z, y, x)) { }

    inline Vector3D<float>(__m128 m) : fData(m) { }

    inline void SetZero() { fData = _mm_setzero_ps(); }

    inline void Set(float s) { fData = _mm_set1_ps(s); }

    inline void Set(__m128 xyzw) { fData = xyzw; }

    inline void Set(float x, float y, float z)
    {
      fData = _mm_set_ps(0, z, y, x);
    }

    inline operator const float *() const
    {
        return (const float *) &fData;
    }

    inline operator       __m128()       { return fData; }
    inline operator const __m128() const { return fData; }

    inline float& operator[](int k)       { return (&x)[k]; }
    inline float  operator[](int k) const { return (&x)[k]; }

    inline Vector3D<float>& operator+=(float s) {
      fData = _mm_add_ps(fData, _mm_set1_ps(s));
      return *this;
    }

    inline Vector3D<float>& operator-=(float s) {
      fData = _mm_sub_ps(fData, _mm_set1_ps(s));
      return *this;
    }

    inline Vector3D<float>& operator*=(float s) {
      fData = _mm_mul_ps(fData, _mm_set1_ps(s));
      return *this;
    }

    inline Vector3D<float>& operator/=(float s) {
      fData = _mm_div_ps(fData, _mm_set1_ps(s));
      return *this;
    }

    inline Vector3D<float>& operator+=(const Vector3D<float>& v) {
      fData = _mm_add_ps(fData, v.fData);
      return *this;
    }

    inline Vector3D<float>& operator-=(const Vector3D<float>& v) {
      fData = _mm_sub_ps(fData, v.fData);
      return *this;
    }

    inline Vector3D<float>& ElemMult(const Vector3D<float>& v) {
      fData = _mm_mul_ps(fData, v.fData);
      return *this;
    }

    inline Vector3D<float>& ElemDiv(const Vector3D<float>& v) {
      fData = _mm_div_ps(fData, v.fData);
      return *this;
    }

    inline float Norm() const {
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(fData, fData, 0x71)));
    }

    inline float RNorm() const {
      return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(fData, fData, 0x71)));
    }

    inline float Norm2() const {
      return _mm_cvtss_f32(_mm_dp_ps(fData, fData, 0x71));
    }

    inline Vector3D<float> Normv() const {
      return Vector3D<float>(_mm_sqrt_ps(Norm2v()));
    }

    inline Vector3D<float> RNormv() const {
      return Vector3D<float>(_mm_rcp_ps(Normv()));
    }

    inline Vector3D<float> Norm2v() const {
      return Vector3D<float>(Dot3(fData, fData));
    }

    inline void Normalize() {
      fData = _mm_div_ps(fData, Normv());
    }

    inline Vector3D<float> Normalized() const {
      return Vector3D<float>(_mm_div_ps(fData, Normv()));
    }

    inline bool NearZero() const {
      return Norm() < std::numeric_limits<float>::epsilon();
    }

    inline Vector3D<float> Abs() const {
      return Vector3D<float>(SIMD::SSE::Abs(fData));
    }

    inline Vector3D<float> Sgn() const {
      return Vector3D<float>(SIMD::SSE::Sgn(fData));
    }

    inline float Min() const {
      return x < z ? (x < y ? x : y) : (y < z ? y : z);
    }

    inline float Max() const {
      return x > z ? (x > y ? x : y) : (y > z ? y : z);
    }

    inline int MinElement() const {
      return x < z ? (x < y ? 0 : 1) : (y < z ? 1 : 2);
    }

    inline int MaxElement() const {
      return x > z ? (x > y ? 0 : 1) : (y > z ? 1 : 2);
    }

    inline int closest_axis() const {
      return Abs().MaxElement();
    }

  protected:
    union {
      __m128 fData;
      struct {
        float x, y, z, w;
      };
    } __attribute__ ((aligned(16)));
};

inline Vector3D<float> operator-(const Vector3D<float>& v)
{
    return Vector3D<float>(Neg(v));
}

inline Vector3D<float> operator+(float s, const Vector3D<float>& v)
{
    return Vector3D<float>(_mm_add_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator+(const Vector3D<float>& v, float s)
{
    return Vector3D<float>(_mm_add_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator-(float s, const Vector3D<float>& v)
{
    return Vector3D<float>(_mm_sub_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator-(const Vector3D<float>& v, float s)
{
    return Vector3D<float>(_mm_sub_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator*(float s, const Vector3D<float>& v)
{
    return Vector3D<float>(_mm_mul_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator*(const Vector3D<float>& v, float s)
{
    return Vector3D<float>(_mm_mul_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator/(const Vector3D<float>& v, float s)
{
    return Vector3D<float>(_mm_div_ps(v, _mm_set1_ps(s)));
}

inline Vector3D<float> operator+(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_add_ps(v1, v2));
}

inline Vector3D<float> operator-(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_sub_ps(v1, v2));
}

inline float  operator*(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0x71));
}

inline float Dot(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0x71));
}

inline Vector3D<float> Dotv(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_dp_ps(v1, v2, 0x7F));
}

inline Vector3D<float> operator^(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(Cross3(v1, v2));
}

inline Vector3D<float> Cross(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(Cross3(v1, v2));
}

/* multiply and divide element by element */

inline Vector3D<float> ElemMult(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_mul_ps(v1, v2));
}

inline Vector3D<float> ElemDiv(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_div_ps(v1, v2));
}

inline Vector3D<float> Min(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_min_ps(v1, v2));
}

inline Vector3D<float> Max(const Vector3D<float>& v1, const Vector3D<float>& v2)
{
    return Vector3D<float>(_mm_max_ps(v1, v2));
}

} /* namespace SSE */
} /* namespace SIMD */
} /* namespace VecCore */

#endif /* __SSE4_1__ */

#endif

