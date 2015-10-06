#ifndef VECCORE_POINT3D_H
#define VECCORE_POINT3D_H

#include "Vector3D.h"

namespace VecCore {
namespace Math {

template <typename T> class Point3D {
public:
  union { struct { T x, y, z; }; T fData[3]; }
#if defined(VECCORE_CUDA) && defined(VECCORE_CUDA_DEVICE)
  __attribute__((aligned(VECCORE_CUDA_ALIGN)));
#elif defined(VECCORE_ENABLE_SIMD)
  __attribute__((aligned(VECCORE_SIMD_ALIGN)));
#else
  ;
#endif

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Point3D() : fData{T(0.0), T(0.0), T(0.0)} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Point3D(const T a, const T b, const T c) : fData{a, b, c} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Point3D(Point3D const &p) : fData{p[0], p[1], p[2]} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Point3D &operator=(Point3D const p) {
    fData[0] = p[0];
    fData[1] = p[1];
    fData[2] = p[2];
    return *this;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Set(T const &a, T const &b, T const &c) {
    fData[0] = a;
    fData[1] = b;
    fData[2] = c;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T &operator[](const int index) { return fData[index]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T const &operator[](const int index) const { return fData[index]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  operator const T *() const { return reinterpret_cast<const T*>(&fData); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Norm() const { return sqrt(Dot(*this, *this)); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Norm2() const { return Dot(*this, *this); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Perp2() const { return fData[0]*fData[0] + fData[1]*fData[1]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Perp() const { return Sqrt(Perp2()); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Phi() const { return atan2(fData[1], fData[0]); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Theta() const { return acos(fData[2], Norm()); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  static Point3D FromCylindrical(T r, T phi, T z) {
    return Point3D(r * cos(phi), r * sin(phi), z);
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  static Point3D FromSpherical(const T r, const T theta, const T phi) {
    return Point3D(r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta));
  }

  /// Maps each vector entry to a function that manipulates the entry type.
  /// \param f A function of type "T f(const T&)" to map over entries.
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Map(T (*f)(const T &)) {
    fData[0] = f(fData[0]);
    fData[1] = f(fData[1]);
    fData[2] = f(fData[2]);
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Min() const {
    T min = (fData[1] < fData[0]) ? fData[1] : fData[0];
    min = (fData[2] < min) ? fData[2] : min;
    return min;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Max() const {
    T max = (fData[1] > fData[0]) ? fData[1] : fData[0];
    max = (fData[2] > max) ? fData[2] : max;
    return max;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void FixZeroes() {
    for (int i = 0; i < 3; ++i) {
      if (Abs(fData[i]) < T(1.0e-6))
	      fData[i] = T(0.0);
    }
  }
};

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T>& operator+(Point3D<T> const &p)
{
  return p;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator-(Point3D<T> const &p)
{
  return Point3D<T>(-p[0], -p[1], -p[2]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> &operator *=(const Point3D<T> &p, const T &s)
{
  p[0] *= s; p[1] *= s; p[2] *= s; return p;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> &operator /=(const Point3D<T> &p, const T &s)
{
  p[0] /= s; p[1] /= s; p[2] /= s; return p;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T>& operator +=(const Point3D<T> &p, const Vector3D<T> &v)
{
  p[0] += v[0]; p[1] += v[1]; p[2] += v[2]; return p;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T>& operator -=(const Point3D<T> &p, const Vector3D<T> &v)
{
  p[0] -= v[0]; p[1] -= v[1]; p[2] -= v[2]; return p;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator *(const T &s, const Point3D<T> &p)
{
  return Point3D<T>(s * p[0], s * p[1], s * p[2]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator *(const Point3D<T> &p, const T &s)
{
  return Point3D<T>(p[0] * s, p[1] * s, p[2] * s);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator /(const Point3D<T> &p, const T &s)
{
  return Point3D<T>(p[0] / s, p[1] / s, p[2] / s);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator +(const Point3D<T> &p1, const Point3D<T> &p2)
{
  return Point3D<T>(p1[0] + p2[0], p1[1] + p2[1], p1[2] + p2[2]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator +(const Point3D<T> &p, const Vector3D<T> &v)
{
  return Point3D<T>(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T> operator -(const Point3D<T> &p1, const Point3D<T> &p2)
{
  return Vector3D<T>(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator -(const Point3D<T> &p, const Vector3D<T> &v)
{
  return Point3D<T>(p[0] - v[0], p[1] - v[1], p[2] - v[2]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
T Dot(const Point3D<T> &p1, const Point3D<T> &p2)
{
    return p1[0]*p2[0] + p1[1]*p2[1] + p1[2]*p2[2];
}

#ifdef VECCORE_SIMPLE_SYNTAX_OPERATORS
template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> operator *(const Point3D<T> &p1, const Point3D<T> &p2)
{
  return Dot(p1, p2);
}
#endif

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Point3D<T> Abs(const Point3D<T> &p)
{
  return Point3D<T>(abs(p[0]), abs(p[1]), abs(p[2]));
}

template <typename T, unsigned int n = 1>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
T Norm(const Point3D<T>& p)
{

  // compiler optimizes out dead code here

  switch (n) {
    case 0:
      return Abs(p).Max();
    case 1:
      return sqrt(Dot(p, p));
    case 2:
      return Dot(p, p);
    default:
      return pow(pow(p[0],n) + pow(p[1],n) + pow(p[2],n), 1.0/n);
  }
}

}
}

#endif
