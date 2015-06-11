/// \author Johannes de Fine Licht (johannes.definelicht@cern.ch)

#ifndef VECCORE_BASE_VECTOR3D_H_
#define VECCORE_BASE_VECTOR3D_H_

#include "VecCoreGlobal.h"
#include "BackendGlobal.h"

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

  template <typename TypeOther>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(Vector3D<TypeOther> const &other)
      : fVec{T(other[0]), T(other[1]), T(other[2])} {}

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D &operator=(Vector3D const &other) {
    fVec[0] = other[0];
    fVec[1] = other[1];
    fVec[2] = other[2];
    return *this;
  }

  /**
   * Contains no check for correct indexing to avoid impairing performance.
   * @param index Index of content in the range [0-2].
   */
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T &operator[](const int index) { return fVec[index]; }

  /**
   * Contains no check for correct indexing to avoid impairing performance.
   * @param index Index of content in the range [0-2].
   */
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
  void Set(T const &a, T const &b, T const &c) {
    fVec[0] = a;
    fVec[1] = b;
    fVec[2] = c;
  }

  VECCORE_CUDA_HEADER_BOTH
  void Set(const T a) { Set(a, a, a); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Perp2() const { return fVec[0] * fVec[0] + fVec[1] * fVec[1]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  T Perp() const { return Sqrt(Perp2()); }

  template <typename Type2>
  /// The dot product of two Vector3D<T> objects
  /// \return T (where T is float, double, or various SIMD vector types)
  /// TODO: make nonstatic standalone function
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static T Dot(Vector3D<T> const &left, Vector3D<Type2> const &right) {
    return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
  }

  template <typename Type2>
  /// The dot product of two Vector3D<T> objects
  /// \return T (where T is float, double, or various SIMD vector types)
  /// note: outer form of Dot should be preferred -- this function exists for backward (UVector3) compatibility
  /// but is actually not used in USolids: can be deprecated
  VECCORE_CUDA_HEADER_BOTH VECCORE_INLINE T Dot(Vector3D<Type2> const &right) const {
    return Dot(*this, right);
  }

  // For UVector3 compatibility. Is equal to normal multiplication.
  // TODO: check if there are implicit dot products in USolids...
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> MultiplyByComponents(Vector3D<T> const &other) const { return *this * other; }

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

  /// The cross (vector) product of two Vector3D<T> objects
  /// \return T (where T is float, double, or various SIMD vector types)
  template <class FirstType, class SecondType>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static Vector3D<T> Cross(Vector3D<FirstType> const &left,
                           Vector3D<SecondType> const &right) {
    return Vector3D<T>(left[1] * right[2] - left[2] * right[1], left[2] * right[0] - left[0] * right[2],
                       left[0] * right[1] - left[1] * right[0]);
  }

  /// The cross (vector) product of two Vector3D<T> objects
  /// \return T (where T is float, double, or various SIMD vector types)
  /// TODO: can be deprecated
  template <class OtherType>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> Cross(Vector3D<OtherType> const &right) const {
    return Cross<T, OtherType>(*this, right);
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
  static Vector3D<T> FromCylindrical(T r, T phi, T z) { return Vector3D<T>(r * Cos(phi), r * Sin(phi), z); }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<T> &FixZeroes() {
    for (int i = 0; i < 3; ++i) {
      MaskedAssign(Abs(fVec[i]) < kTolerance, 0., &fVec[i]);
    }
    return *this;
  }

// Inplace binary operators

#define VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(OPERATOR)                          \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> &operator OPERATOR(const Vector3D<T> &other) {                   \
    fVec[0] OPERATOR other.fVec[0];                                          \
    fVec[1] OPERATOR other.fVec[1];                                          \
    fVec[2] OPERATOR other.fVec[2];                                          \
    return *this;                                                              \
  }                                                                            \
  template <typename OtherType>                                                \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> &operator OPERATOR(const Vector3D<OtherType> &other) {           \
    fVec[0] OPERATOR other[0];                                                \
    fVec[1] OPERATOR other[1];                                                \
    fVec[2] OPERATOR other[2];                                                \
    return *this;                                                              \
  }                                                                            \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  VECCORE_INLINE                                                               \
  Vector3D<T> &operator OPERATOR(const T &scalar) {                            \
    fVec[0] OPERATOR scalar;                                                  \
    fVec[1] OPERATOR scalar;                                                  \
    fVec[2] OPERATOR scalar;                                                  \
    return *this;                                                              \
  }

  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(+= )
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(-= )
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(*= )
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(/= )
#undef VECTOR3D_TEMPLATE_INPLACE_BINARY_OP

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  // conversion to boolean: Why is this useful --> can potentially be removed
  operator bool() const { return fVec[0] && fVec[1] && fVec[2]; }
};

#define VECTOR3D_BINARY_OP(OPERATOR, INPLACE)                                  \
  template <typename T, typename OtherType>                                    \
  VECCORE_INLINE                                                               \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  Vector3D<T> operator OPERATOR(const Vector3D<T> &lhs,                        \
                                const Vector3D<OtherType> &rhs) {              \
    Vector3D<T> result(lhs);                                                   \
    result INPLACE rhs;                                                        \
    return result;                                                             \
  }                                                                            \
  template <typename T, typename ScalarType>                                   \
  VECCORE_INLINE                                                               \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  Vector3D<T> operator OPERATOR(Vector3D<T> const &lhs,                        \
                                const ScalarType rhs) {                        \
    Vector3D<T> result(lhs);                                                   \
    result INPLACE rhs;                                                        \
    return result;                                                             \
  }                                                                            \
  template <typename T, typename ScalarType>                                   \
  VECCORE_INLINE                                                               \
  VECCORE_CUDA_HEADER_BOTH                                                     \
  Vector3D<T> operator OPERATOR(const ScalarType lhs,                          \
                                Vector3D<T> const &rhs) {                      \
    Vector3D<T> result(lhs);                                                   \
    result INPLACE rhs;                                                        \
    return result;                                                             \
  }

VECTOR3D_BINARY_OP(+, += )
VECTOR3D_BINARY_OP(-, -= )
VECTOR3D_BINARY_OP(*, *= )
VECTOR3D_BINARY_OP(/, /= )
#undef VECTOR3D_BINARY_OP

template <typename T>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Vector3D<T> operator-(Vector3D<T> const &fVec) {
  return Vector3D<T>(-fVec[0], -fVec[1], -fVec[2]);
}

VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Vector3D<bool> operator!(Vector3D<bool> const &fVec) { return Vector3D<bool>(!fVec[0], !fVec[1], !fVec[2]); }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#define VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP(OPERATOR)                                               \
  VECCORE_CUDA_HEADER_BOTH                                                                         \
  VECCORE_INLINE                                                                                   \
  Vector3D<bool> operator OPERATOR(Vector3D<bool> const &lhs, Vector3D<bool> const &rhs) {         \
    return Vector3D<bool>(lhs[0] OPERATOR rhs[0], lhs[1] OPERATOR rhs[1], lhs[2] OPERATOR rhs[2]); \
  }
VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP(&&)
VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP(||)
#undef VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP
#pragma GCC diagnostic pop
} // End inline namespace
} // End global namespace

#endif
