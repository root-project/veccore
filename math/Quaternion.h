#ifndef VECCORE_QUATERNION_H
#define VECCORE_QUATERNION_H

#include <math/Vector3D.h>

namespace VecCore {
namespace Math {

template <typename T> class Quaternion {
private:
  T fData[4]
#if defined(VECCORE_CUDA) && defined(VECCORE_CUDA_DEVICE)
  __attribute__((aligned(VECCORE_CUDA_ALIGN)));
#elif defined(VECCORE_ENABLE_SIMD)
  __attribute__((aligned(VECCORE_SIMD_ALIGN)));
#else
  ;
#endif

public:
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Quaternion() : fData{T(0.0), T(0.0), T(0.0), T(0.0)} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Quaternion(const T x, const T y, const T z, const T w) : fData{x, y, z, w} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Quaternion(const T * q) : fData{q[0], q[1], q[2], q[3]} {}

  template <typename Type>
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Quaternion(Quaternion<Type> const &q) : fData{T(q[0]), T(q[1]), T(q[2]), T(q[3])} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Quaternion(const Vector3D<T>& axis, const T& angle) {
    T s = sin(angle / 2.0) / Norm(axis);
    fData[0] = axis[0] * s;
    fData[1] = axis[1] * s;
    fData[2] = axis[2] * s;
    fData[3] = cos(angle / 2.0);
  }

  template <typename Type>
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Quaternion& operator=(Quaternion<Type> const &q) {
    fData[0] = static_cast<T>(q[0]);
    fData[1] = static_cast<T>(q[1]);
    fData[2] = static_cast<T>(q[2]);
    fData[3] = static_cast<T>(q[3]);
    return *this;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Set(const T x, const T y, const T z, const T w)
  {
    fData[0] = x;
    fData[1] = y;
    fData[2] = z;
    fData[3] = w;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Set(const Vector3D<T>& axis, const T& angle)
  {
    T s = sin(angle / 2.0) / Norm(axis);
    fData[0] = axis[0] * s;
    fData[1] = axis[1] * s;
    fData[2] = axis[2] * s;
    fData[3] = cos(angle / 2.0);
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  operator const T *() const { return &fData[0]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T& operator[](int i)       { return fData[i]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T  operator[](int i) const { return fData[i]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Conjugate()
  {
    fData[0] = -fData[0];
    fData[1] = -fData[1];
    fData[2] = -fData[2];
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Invert()
  {
    Conjugate();
    *this /= (fData[0]*fData[0] + fData[1]*fData[1] + fData[2]*fData[2] + fData[3]*fData[3]);
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Normalize()
  {
    *this /= sqrt(fData[0]*fData[0] + fData[1]*fData[1] + fData[2]*fData[2] + fData[3]*fData[3]);
  }

  static Quaternion Identity() { return Quaternion(T(0.0), T(0.0), T(0.0), T(1.0)); }
};

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> operator+(Quaternion<T> &q)
{
  return q;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> operator-(Quaternion<T> const &q)
{
  return Quaternion<T>(-q[0], -q[1], -q[2], -q[3]);
}

#define QUATERNION_SCALAR_VECTOR_OPERATOR(OP)                                  \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Quaternion<T> operator OP(const T &s, const Quaternion<T> &q)                \
  {                                                                            \
    return Quaternion<T>(s OP q[0], s OP q[1], s OP q[2], s OP q[3]);          \
  }                                                                            \

#define QUATERNION_VECTOR_SCALAR_OPERATOR(OP)                                  \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Quaternion<T> operator OP(const Quaternion<T> &q, const T &s)                \
  {                                                                            \
    return Quaternion<T>(q[0] OP s, q[1] OP s, q[2] OP s, q[3] OP s);          \
  }                                                                            \

#define QUATERNION_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(OP)                     \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Quaternion<T> &operator OP(const Quaternion<T> &q, const T &s)               \
  {                                                                            \
    q[0] OP s; q[1] OP s; q[2] OP s; q[3] OP s;                                \
    return q;                                                                  \
  }                                                                            \

#define QUATERNION_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(OP)                     \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Quaternion<T>& operator OP(const Quaternion<T> &q1, const Quaternion<T> &q2) \
  {                                                                            \
    q1[0] OP q2[0]; q1[1] OP q2[1]; q1[2] OP q2[2]; q1[3] OP q2[3];            \
    return q1;                                                                 \
  }                                                                            \

#define QUATERNION_BINARY_OPERATOR(OP)                                         \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Quaternion<T> operator OP(const Quaternion<T> &q1, const Quaternion<T> &q2)  \
  {                                                                            \
    return Quaternion<T>(q1[0] OP q2[0], q1[1] OP q2[1],                       \
                         q1[2] OP q2[2], q1[3] OP q2[3]);                      \
  }                                                                            \

QUATERNION_SCALAR_VECTOR_OPERATOR(*)

QUATERNION_VECTOR_SCALAR_OPERATOR(*)
QUATERNION_VECTOR_SCALAR_OPERATOR(/)

QUATERNION_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(*=)
QUATERNION_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(/=)

QUATERNION_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(+=)
QUATERNION_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(-=)

QUATERNION_BINARY_OPERATOR(+)
QUATERNION_BINARY_OPERATOR(-)

#undef QUATERNION_SCALAR_VECTOR_OPERATOR
#undef QUATERNION_VECTOR_SCALAR_OPERATOR
#undef QUATERNION_BINARY_OPERATOR

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
T Dot(const Quaternion<T> &q1, const Quaternion<T> &q2)
{
  return q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2] + q1[3]*q2[3];
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> Cross(const Quaternion<T> &q1, const Quaternion<T> &q2)
{
  T x = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
  T y = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
  T z = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
  T w = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
  return Quaternion<T>(x, y, z, w);
}

template <typename T, unsigned int n = 1>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
T Norm(const Quaternion<T>& q)
{
  switch (n) {
    case 1:
      return sqrt(Dot(q, q));
    case 2:
      return Dot(q, q);
    default:
      return pow(Dot(q, q), n/2.0);
  }
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> Conjugate(const Quaternion<T>& q)
{
  return Quaternion<T>(-q[0], -q[1], -q[2], q[3]);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> Inverse(const Quaternion<T>& q)
{
  return Conjugate(q) / Dot(q, q);
}

#ifdef QUATERNION_SIMPLE_SYNTAX_OPERATORS
// denote conjugate of q as ~q in code
template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> operator~(const Quaternion<T>& q)
{
  return Conjugate(q);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> operator *(const Quaternion<T> &q1, const Quaternion<T> &q2)
{
  return Dot(q1, q2);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T> operator ^(const Quaternion<T> &q1, const Quaternion<T> &q2)
{
  Cross(q1, q2);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Quaternion<T>& operator^=(Quaternion<T>& q1, const Quaternion<T>& q2)
{
  q1 = Cross(q1, q2);
  return q1;
}
#endif

} // namespace Math
} // namespace VecCore

#endif
