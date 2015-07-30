#ifndef VECCORE_VECTOR3D_AVX_H
#define VECCORE_VECTOR3D_AVX_H

#include <core/util.h>
#include <simd/avx/avx.h>

#ifdef __AVX__
namespace VecCore {
namespace SIMD {
namespace AVX {

template <typename T> class Vector3D;

template<> class Vector3D<double> {
public:
    inline Vector3D<double>()
        : fData(_mm256_setzero_pd()) { }

    inline Vector3D<double>(double s)
        : fData(_mm256_set1_pd(s)) { }

    inline Vector3D<double>(double x, double y, double z)
        : fData(_mm256_set_pd(0, z, y, x)) { }

    inline Vector3D<double>(__m256d m)
        : fData(m) { }

    inline void SetZero() { fData = _mm256_setzero_pd(); }

    inline void Set(double s) { fData = _mm256_set1_pd(s); }

    inline void Set(__m256d xyzw) { fData = xyzw; }

    inline void Set(double x, double y, double z)
    {
        fData = _mm256_set_pd(0, z, y, x);
    }

    inline operator const double *() const
    {
        return (const double *) &fData;
    }

    inline operator       __m256d()       { return fData; }
    inline operator const __m256d() const { return fData; }

    inline double& operator[](int k)       { return (&x)[k]; }
    inline double  operator[](int k) const { return (&x)[k]; }

    inline Vector3D<double>& operator+=(double s)
    {
        fData = _mm256_add_pd(fData, _mm256_set1_pd(s));
        return *this;
    }

    inline Vector3D<double>& operator-=(double s)
    {
        fData = _mm256_sub_pd(fData, _mm256_set1_pd(s));
        return *this;
    }

    inline Vector3D<double>& operator*=(double s)
    {
        fData = _mm256_mul_pd(fData, _mm256_set1_pd(s));
        return *this;
    }

    inline Vector3D<double>& operator/=(double s)
    {
        fData = _mm256_div_pd(fData, _mm256_set1_pd(s));
        return *this;
    }

    inline Vector3D<double>& operator+=(const Vector3D<double>& v)
    {
        fData = _mm256_add_pd(fData, v.fData);
        return *this;
    }

    inline Vector3D<double>& operator-=(const Vector3D<double>& v)
    {
        fData = _mm256_sub_pd(fData, v.fData);
        return *this;
    }

    inline Vector3D<double>& ElemMult(const Vector3D<double>& v)
    {
        fData = _mm256_mul_pd(fData, v.fData);
        return *this;
    }

    inline Vector3D<double>& ElemDiv(const Vector3D<double>& v)
    {
        fData = _mm256_div_pd(fData, v.fData);
        return *this;
    }

    inline double Norm() const
    {
        return sqrt(Norm2());
    }

    inline double RNorm() const
    {
        return 1.0 / sqrt(Norm2());
    }

    inline double Norm2() const
    {
        __m256d tmp = Dot3(fData, fData);
        double *ptr = (double*)(&tmp);
        return *ptr;
    }

    inline Vector3D<double> Normv() const
    {
        return Vector3D<double>(_mm256_sqrt_pd(Dot3(fData, fData)));
    }

    inline Vector3D<double> RNormv() const
    {
        return Vector3D<double>(_mm256_div_pd(_mm256_set1_pd(1.0), Normv()));
    }

    inline Vector3D<double> Norm2v() const
    {
        return Vector3D<double>(Dot3(fData, fData));
    }

    inline void Normalize()
    {
        fData = Vector3D<double>(_mm256_div_pd(fData, Dot3(fData, fData)));
    }

    inline Vector3D<double> Normalized() const
    {
        return Vector3D<double>(_mm256_div_pd(fData, Normv()));
    }

    inline bool NearZero() const
    {
        return Norm() < std::numeric_limits<double>::epsilon();
    }

    inline Vector3D<double> Abs() const
    {
        return Vector3D<double>(SIMD::AVX::Abs(fData));
    }

    inline Vector3D<double> Sgn() const
    {
        return Vector3D<double>(SIMD::AVX::Sgn(fData));
    }

    inline double Min() const
    {
        return x < z ? (x < y ? x : y) : (y < z ? y : z);
    }

    inline double Max() const
    {
        return x > z ? (x > y ? x : y) : (y > z ? y : z);
    }

    inline int MinElement() const
    {
        return x < z ? (x < y ? 0 : 1) : (y < z ? 1 : 2);
    }

    inline int MaxElement() const
    {
        return x > z ? (x > y ? 0 : 1) : (y > z ? 1 : 2);
    }

    inline int closest_axis() const
    {
        return Vector3D<double>(SIMD::AVX::Abs(fData)).MaxElement();
    }

    static Vector3D<double> Random()   /* unit Vector3D<double> in random direction */
    {
        double z = 2.0f * drand48() - 1.0f;
        double r = sqrt(1.0f - z * z);
        double t = 2.0f * (double)M_PI * drand48();
        return Vector3D<double>(r * cos(t), r * sin(t), z);
    }

protected:
    union {
        __m256d fData;
        struct {
            double x, y, z, w;
        };
    } __attribute__ ((aligned(32)));
};

inline Vector3D<double> operator-(const Vector3D<double>& v)
{
    return Vector3D<double>(Neg(v));
}

inline Vector3D<double> operator+(double s, const Vector3D<double>& v)
{
    return Vector3D<double>(_mm256_add_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator+(const Vector3D<double>& v, double s)
{
    return Vector3D<double>(_mm256_add_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator-(double s, const Vector3D<double>& v)
{
    return Vector3D<double>(_mm256_sub_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator-(const Vector3D<double>& v, double s)
{
    return Vector3D<double>(_mm256_sub_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator*(double s, const Vector3D<double>& v)
{
    return Vector3D<double>(_mm256_mul_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator*(const Vector3D<double>& v, double s)
{
    return Vector3D<double>(_mm256_mul_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator/(const Vector3D<double>& v, double s)
{
    return Vector3D<double>(_mm256_div_pd(v, _mm256_set1_pd(s)));
}

inline Vector3D<double> operator+(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(_mm256_add_pd(v1, v2));
}

inline Vector3D<double> operator-(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(_mm256_sub_pd(v1, v2));
}

inline double Dot(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    __m256d tmp = Dot3(v1, v2);
    double *ptr = (double*)(&tmp);
    return *ptr;
}

inline double  operator*(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Dot(v1,v2);
}

inline Vector3D<double> Dotv(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(Dot4(v1, v2));
}

inline Vector3D<double> Cross(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(Cross3(v1, v2));
}

inline Vector3D<double> operator^(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(Cross3(v1, v2));
}

/* multiply and divide element by element */

inline Vector3D<double> ElemMult(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(_mm256_mul_pd(v1, v2));
}

inline Vector3D<double> ElemDiv(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(_mm256_div_pd(v1, v2));
}

inline Vector3D<double> Min(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(_mm256_min_pd(v1, v2));
}

inline Vector3D<double> Max(const Vector3D<double>& v1, const Vector3D<double>& v2)
{
    return Vector3D<double>(_mm256_max_pd(v1, v2));
}

} /* namespace AVX */
} /* namespace SIMD */
} /* namespace VecCore */

#endif
#endif
