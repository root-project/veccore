/// \author Johannes de Fine Licht (johannes.definelicht@cern.ch)

#ifndef VECCORE_BASE_VECTOR3D_H_
#define VECCORE_BASE_VECTOR3D_H_

#include <VecCoreGlobal.h>
#include <BackendGlobal.h>

namespace veccore {

VECCORE_DEVICE_FORWARD_DECLARE(template <typename T> class Vector3D;)

inline namespace VECCORE_IMPL_NAMESPACE {

using ::veccore::backend::scalar::Abs;
using ::veccore::backend::vector::Abs;
using ::veccore::backend::scalar::Cos;
using ::veccore::backend::vector::Cos;
using ::veccore::backend::vector::Sin;
using ::veccore::backend::vector::Sin;
using ::veccore::backend::scalar::Sqrt;
using ::veccore::backend::vector::Sqrt;
using ::veccore::backend::scalar::ATan2;
using ::veccore::backend::vector::ATan2;
using ::veccore::backend::scalar::MaskedAssign;
using ::veccore::backend::vector::MaskedAssign;

template <typename T> class Vector3D {
private:

  T fVec[3];

public:
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(const T a, const T b, const T c) : fVec{a, b, c} {}

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D() : fVec{T(0.0), T(0.0), T(0.0)} {}

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(const T a) : fVec{a, a, a} {}

  template <typename Type>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(Vector3D<Type> const &v)
      : fVec{T(v[0]), T(v[1]), T(v[2])} {}

  template <typename Type>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D &operator=(Vector3D const &v) {
    fVec[0] = static_cast<Type>(v[0]);
    fVec[1] = static_cast<Type>(v[1]);
    fVec[2] = static_cast<Type>(v[2]);

    return *this;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  operator const T *() const { return reinterpret_cast<const T*>(&fVec); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T &operator[](const int index) { return fVec[index]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T const &operator[](const int index) const { return fVec[index]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T &x() { return fVec[0]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T const &x() const { return fVec[0]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T &y() { return fVec[1]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T const &y() const { return fVec[1]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T &z() { return fVec[2]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T const &z() const { return fVec[2]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void Set(T const &a, T const &b, T const &c) {
    fVec[0] = a;
    fVec[1] = b;
    fVec[2] = c;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void Set(const T a) { Set(a, a, a); }

  // For UVector3 compatibility. Is equal to normal multiplication.
  // TODO: check if there are implicit dot products in USolids...
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> MultiplyByComponents(Vector3D<T> const &v) const { return *this * v; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Perp2() const { return fVec[0]*fVec[0] + fVec[1]*fVec[1]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Perp() const { return Sqrt(Perp2()); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Mag2() const { return Dot(*this, *this); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Mag() const { return Sqrt(Mag2()); }

  // SW: this function should be deprecated
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Length() const { return Mag(); }

  // should be deprecated as same as Mag2
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Length2() const { return Mag2(); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void Normalize() { *this *= (1. / Mag()); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> Normalized() const { return Vector3D<T>(*this) * (1. / Mag()); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Phi() const { return ATan2(fVec[1], fVec[0]); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Theta() const { return ACos(fVec[2], Mag()); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static Vector3D<T> FromCylindrical(T r, T phi, T z) { return Vector3D<T>(r * Cos(phi), r * Sin(phi), z); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static Vector3D ConvertFromSpherical(const T r, const T theta, const T phi) {
    return Vector3D(r*Sin(theta)*Cos(phi), r*Sin(theta)*Sin(phi), r*Cos(theta));
  }

  /// Maps each vector entry to a function that manipulates the entry type.
  /// \param f A function of type "T f(const T&)" to map over entries.
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void Map(T (*f)(const T &)) {
    fVec[0] = f(fVec[0]);
    fVec[1] = f(fVec[1]);
    fVec[2] = f(fVec[2]);
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> Abs() const { return Vector3D<T>(Abs(fVec[0]), Abs(fVec[1]), Abs(fVec[2])); }

  template <typename BoolType>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void MaskedAssign(Vector3D<BoolType> const &condition,
                    Vector3D<T> const &value) {
    fVec[0] = (condition[0]) ? value[0] : fVec[0];
    fVec[1] = (condition[1]) ? value[1] : fVec[1];
    fVec[2] = (condition[2]) ? value[2] : fVec[2];
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Min() const {
    T min = (fVec[1] < fVec[0]) ? fVec[1] : fVec[0];
    min = (fVec[2] < min) ? fVec[2] : min;
    return min;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Max() const {
    T max = (fVec[1] > fVec[0]) ? fVec[1] : fVec[0];
    max = (fVec[2] > max) ? fVec[2] : max;
    return max;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> &FixZeroes() {
    for (int i = 0; i < 3; ++i) {
      MaskedAssign(Abs(fVec[i]) < kTolerance, 0., &fVec[i]);
    }
    return *this;
  }

// Inplace binary operators

#define VECTOR3D_INPLACE_OPERATOR(OP)                                          \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> &operator OP(const Vector3D<T> &v) {                             \
    fVec[0] OP v.fVec[0];                                                      \
    fVec[1] OP v.fVec[1];                                                      \
    fVec[2] OP v.fVec[2];                                                      \
    return *this;                                                              \
  }                                                                            \
                                                                               \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> &operator OP(const T &scalar) {                                  \
    fVec[0] OP scalar;                                                         \
    fVec[1] OP scalar;                                                         \
    fVec[2] OP scalar;                                                         \
    return *this;                                                              \
  }

  VECTOR3D_INPLACE_OPERATOR(+=)
  VECTOR3D_INPLACE_OPERATOR(-=)
  VECTOR3D_INPLACE_OPERATOR(*=)
  VECTOR3D_INPLACE_OPERATOR(/=)
#undef VECTOR3D_INPLACE_OPERATOR

};

#define VECTOR3D_BINARY_OPERATOR(OP)                                           \
  template <typename T>                                                        \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> operator OP(Vector3D<T> const &lhs, const T rhs) {               \
    return Vector3D<T>(lhs[0] OP rhs, lhs[1] OP rhs, lhs[2] OP rhs);           \
  }                                                                            \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> operator OP(const T lhs, Vector3D<T> const &rhs) {               \
    return Vector3D<T>(rhs[0] OP lhs, rhs[1] OP lhs, rhs[2] OP lhs);           \
  }                                                                            \
                                                                               \
  template <typename T>                                                        \
  VECCORE_INLINE                                                               \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  Vector3D<T> operator OP(const Vector3D<T> &lhs, const Vector3D<T> &rhs) {    \
    return Vector3D<T>(lhs[0] OP rhs[0], lhs[1] OP rhs[1], lhs[2] OP rhs[2]);  \
  }                                                                            \

VECTOR3D_BINARY_OPERATOR(+)
VECTOR3D_BINARY_OPERATOR(-)
VECTOR3D_BINARY_OPERATOR(*) // These are probably only good for scalar,
VECTOR3D_BINARY_OPERATOR(/) // what is a vector divided by another?
#undef VECTOR3D_BINARY_OPERATOR

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Vector3D<T> operator-(Vector3D<T> const &v) {
  return Vector3D<T>(-v[0], -v[1], -v[2]);
}

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
T Dot(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Vector3D<T> Cross(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
    return Vector3D<T>(v1[1] * v2[2] - v1[2] * v2[1],
                       v1[2] * v2[0] - v1[0] * v2[2],
                       v1[0] * v2[1] - v1[1] * v2[0]);
}

} // End inline namespace
} // End global namespace

#endif
