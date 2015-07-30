#ifndef VECCORE_VECTOR3D_MIC_H
#define VECCORE_VECTOR3D_MIC_H

#include <core/util.h>
#include <simd/mic/mic.h>

#if defined(__MIC__)
namespace VecCore { namespace SIMD { namespace MIC {

/* use generic implementation for mic for the moment */

template <typename T> class Vector3D {
public:
    inline Vector3D() : x(T(0.0)), y(T(0.0)), z(T(0.0)) {}

    inline Vector3D(T s) : x(s), y(s), z(s) {}

    inline Vector3D(T a, T b, T c) : x(a), y(b), z(c) {}

    inline void set_zero() { x = T(0.0), y = T(0.0), z = T(0.0); }

    inline void Set(T s) { x = s, y = s, z = s; }
    inline void Set(T a, T b, T c) { x = a, y = b, z = c; }

    inline operator const T *() const { return (const T *)&x; }

    inline T& operator[](int k)       { return (&x)[k]; }
    inline T  operator[](int k) const { return (&x)[k]; }

    inline Vector3D& operator+=(T s) {
        x += s, y += s, z += s;
        return *this;
    }

    inline Vector3D& operator-=(T s) {
        x -= s, y -= s, z -= s;
        return *this;
    }

    inline Vector3D& operator*=(T s) {
        x *= s, y *= s, z *= s;
        return *this;
    }

    inline Vector3D& operator/=(T s) {
        x /= s, y /= s, z /= s;
        return *this;
    }

    inline Vector3D& operator+=(const Vector3D &v) {
        x += v.x, y += v.y, z += v.z;
        return *this;
    }

    inline Vector3D& operator-=(const Vector3D &v) {
        x -= v.x, y -= v.y, z -= v.z;
        return *this;
    }

    inline Vector3D& elem_mul(const Vector3D &v) {
        x *= v.x, y *= v.y, z *= v.z;
        return *this;
    }

    inline Vector3D& elem_div(const Vector3D &v) {
        x /= v.x, y /= v.y, z /= v.z;
        return *this;
    }

    inline T Norm() const { return std::sqrt(Norm2()); }
    inline T RNorm() const { return T(1.0) / Norm(); }
    inline T Norm2() const { return x * x + y * y + z * z; }

    inline Vector3D Normv() const { return Vector3D(Norm()); }
    inline Vector3D RNormv() const { return Vector3D(RNorm()); }
    inline Vector3D Norm2v() const { return Vector3D(Norm2()); }

    inline void Normalize() { (*this) *= RNorm(); }
    inline Vector3D Normalized() const {
        T s = RNorm();
        return Vector3D(s * x, s * y, s * z);
    }

    inline bool NearZero() const
    {
        return norm() < std::numeric_limits<T>::epsilon();
    }

    inline Vector3D Abs() const {
        return Vector3D(abs(x), abs(y), abs(z));
    }
    inline Vector3D Sgn() const {
        return Vector3D(sgn(x), sgn(y), sgn(z));
    }

    inline T Min() const { return VecCore::Min(x, y, z); }
    inline T Max() const { return VecCore::Max(x, y, z); }

    inline int MinIndex() const {
        return x < z ? (x < y ? 0 : 1) : (y < z ? 1 : 2);
    }
    inline int MaxIndex() const {
        return x > z ? (x > y ? 0 : 1) : (y > z ? 1 : 2);
    }

    inline int ClosestAxis() const { return Abs().MaxIndex(); }

protected:
    union {
        struct { T x, y, z, w; };
        T fData[4] __attribute__((aligned(4 * sizeof(T))));
    };
};

template <typename T> inline T Norm(const Vector3D<T> &v) { return v.Norm(); }
template <typename T> inline T Norm2(const Vector3D<T> &v) { return v.Norm2(); }
template <typename T> inline T RNorm(const Vector3D<T> &v) { return v.RNorm(); }

template <typename T> inline Vector3D<T> Normv(const Vector3D<T> &v)
{
  return v.Normv();
}

template <typename T> inline Vector3D<T> RNormv(const Vector3D<T> &v)
{
  return v.RNormv();
}

template <typename T> inline Vector3D<T> Norm2v(const Vector3D<T> &v)
{
  return v.Norm2v();
}

template <typename T> inline bool NearZero(const Vector3D<T> &v)
{
  return v.NearZero();
}

template <typename T> inline T Min(const Vector3D<T> &v) { return v.Min(); }
template <typename T> inline T Max(const Vector3D<T> &v) { return v.Max(); }

template <typename T> inline Vector3D<T> Abs(const Vector3D<T> &v)
{
  return v.Abs();
}

template <typename T> inline Vector3D<T> Sgn(const Vector3D<T> &v)
{
  return v.Sgn();
}

template <typename T> inline Vector3D<T> operator-(const Vector3D<T> &v)
{
  return Vector3D<T>(-v[0], -v[1], -v[2]);
}

template <typename T> inline Vector3D<T> operator*(T s, const Vector3D<T> &v)
{
  return Vector3D<T>(s * v.x, s * v.y, s * v.z);
}

template <typename T> inline Vector3D<T> operator*(const Vector3D<T> &v, T s)
{
  return Vector3D<T>(s * v.x, s * v.y, s * v.z);
}

template <typename T> inline Vector3D<T> operator/(const Vector3D<T> &v, T s)
{
  return Vector3D<T>(v.x / s, v.y / s, v.z / s);
}

template <typename T>
inline Vector3D<T> operator+(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

template <typename T>
inline Vector3D<T> operator-(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

template <typename T> inline T dot(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

template <typename T>
inline Vector3D<T> Dotv(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(Dot(v1, v2));
}

template <typename T>
inline Vector3D<T> Cross(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
    return Vector3D<T>(v1[1] * v2[2] - v1[2] * v2[1],
                       v1[2] * v2[0] - v1[0] * v2[2],
                       v1[0] * v2[1] - v1[1] * v2[0]);
}

template <typename T>
inline T operator*(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Dot(v1, v2);
}

template <typename T>
inline Vector3D<T> operator^(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Cross(v1, v2);
}

template <typename T>
inline Vector3D<T> ElemMult(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

template <typename T>
inline Vector3D<T> ElemDiv(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

template <typename T>
inline Vector3D<T> Min(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(Min(v1[0], v2[0]), Min(v1[1], v2[1]), Min(v1[2], v2[2]));
}

template <typename T>
inline Vector3D<T> Max(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Vector3D<T>(Max(v1[0], v2[0]), Max(v1[1], v2[1]), Max(v1[2], v2[2]));
}

template <typename T> inline T Angle(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return acos((v1 * v2) / sqrt(Norm2(v1) * Norm2(v2)));
}

} } }
#endif
#endif
