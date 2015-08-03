#ifndef VECCORE_MATRIX_3X3_H
#define VECCORE_MATRIX_3X3_H

#include <math/Vector3D.h>
#include <math/Quaternion.h>

namespace VecCore {
namespace Math {

template <typename T> class Matrix3x3 {
private:
  T fData[3][3]
#if defined(VECCORE_CUDA) && defined(VECCORE_CUDA_DEVICE)
  __attribute__((aligned(VECCORE_CUDA_ALIGN)));
#elif defined(VECCORE_ENABLE_SIMD)
  __attribute__((aligned(VECCORE_SIMD_ALIGN)));
#else
  ;
#endif

public:
  Matrix3x3() {}

  Matrix3x3(const T a, const T b, const T c,
            const T d, const T e, const T f,
            const T g, const T h, const T i)
    : fData{{a, b, c}, {d, e, f}, {g, h, i}} {}

  explicit Matrix3x3(T s)
    : fData{{s, T(0.0), T(0.0)}, {T(0.0), s, T(0.0)}, {T(0.0), T(0.0), s}} {}

  Matrix3x3(const T x, const T y, const T z)
    : fData{{x, T(0.0), T(0.0)}, {T(0.0), y, T(0.0)}, {T(0.0), T(0.0), z}} {}

  explicit Matrix3x3(const Vector3D<T>& v)
    : fData{{v[0], T(0.0), T(0.0)}, {T(0.0), v[1], T(0.0)}, {T(0.0), T(0.0), v[2]}} {}

  Matrix3x3(const Vector3D<T>& v1, const Vector3D<T>& v2, const Vector3D<T>& v3)
    : fData{{v1[0], v1[1], v1[2]}, {v2[0], v2[1], v2[2]}, {v3[0], v3[1], v3[2]}} {}

  Matrix3x3(const Matrix3x3& M) : fData(M.fData) {}

  Matrix3x3(const Quaternion<T>& q)
  {
    T x = q[0], y = q[1], z = q[2], w = q[3];

    T x2 = x*x, y2 = y*y, z2 = z*z;
    T xy = x*y, xz = x*z, yz = y*z;
    T wx = w*x, wy = w*y, wz = w*z;

    fData[0][0] = 1-2*(y2+z2); fData[0][1] =  2*(xy-wz);  fData[0][2] =  2*(xz+wy);
    fData[1][0] =  2*(xy+wz);  fData[1][1] = 1-2*(x2+z2); fData[1][2] =  2*(yz-wx);
    fData[2][0] =  2*(xz-wy);  fData[2][1] =  2*(yz+wx);  fData[2][2] = 1-2*(x2+y2);
  }

  Vector3D<T> & operator[](int i)
  {
    return reinterpret_cast<Vector3D<T> >(fData[i]);
  }

  Vector3D<T> const & operator[](int i) const
  {
    return reinterpret_cast<Vector3D<T> >(fData[i]);
  }

  operator const Vector3D<T> *() const
  {
    return reinterpret_cast<Vector3D<T>*>(&fData[0]);
  }

  static Matrix3x3 Diagonal(const T x, const T y, const T z)
  {
    return Matrix3x3(x, y, z);
  }

  static Matrix3x3 Identity()
  {
    return Matrix3x3::Diagonal(T(1.0), T(1.0), T(1.0));
  }

  T Det() const;
  Matrix3x3 Transpose() const;
  Matrix3x3 Inverse() const;
};

template <typename T>
Matrix3x3<T>& operator+(Matrix3x3<T> const &M)
{
  return M;
}

template <typename T>
Matrix3x3<T> operator-(Matrix3x3<T> const &M)
{
  return Matrix3x3<T>(-M[0][0], -M[0][1], -M[0][2],
                      -M[1][0], -M[1][1], -M[1][2],
                      -M[2][0], -M[2][1], -M[2][2]);
}

#define SCALAR_MATRIX3x3_OPERATOR(OP)                                          \
  template <typename T>                                                        \
  Matrix3x3<T> operator OP(const T s, const Matrix3x3<T>& M)                   \
  {                                                                            \
    return Matrix3x3<T>(s OP M[0][0], s OP M[0][1], s OP M[0][2],              \
                        s OP M[1][0], s OP M[1][1], s OP M[1][2],              \
                        s OP M[2][0], s OP M[2][1], s OP M[2][2]);             \
  }                                                                            \

#define MATRIX3x3_SCALAR_OPERATOR(OP)                                          \
  template <typename T>                                                        \
  Matrix3x3<T> operator OP(const Matrix3x3<T>& M, const T s)                   \
  {                                                                            \
    return Matrix3x3<T>(M[0][0] OP s, M[0][1] OP s, M[0][2] OP s,              \
                        M[1][0] OP s, M[1][1] OP s, M[1][2] OP s,              \
                        M[2][0] OP s, M[2][1] OP s, M[2][2] OP s);             \
  }                                                                            \

#define MATRIX3x3_SCALAR_COMPOUND_ASSIGMENT_OPERATOR(OP)                       \
  template <typename T>                                                        \
  Matrix3x3<T>& operator OP(Matrix3x3<T>& M, const T s)                        \
  {                                                                            \
    for (int i = 0; i < 3; i++)                                                \
      for (int j = 0; j < 3; j++)                                              \
        M[i][j] OP s;                                                          \
    return M;                                                                  \
  }                                                                            \

#define MATRIX3x3_MATRIX3x3_COMPOUND_ASSIGMENT_OPERATOR(OP)                    \
  template <typename T>                                                        \
  Matrix3x3<T>& operator OP(Matrix3x3<T>& A, const Matrix3x3<T>& B)            \
  {                                                                            \
    for (int i = 0; i < 3; i++)                                                \
      for (int j = 0; j < 3; j++)                                              \
        A[i][j] OP B[i][j];                                                    \
    return A;                                                                  \
  }                                                                            \

#define MATRIX3x3_MATRIX3x3_BINARY_OPERATOR(OP)                                \
  template <typename T>                                                        \
  Matrix3x3<T> operator OP(Matrix3x3<T>& A, const Matrix3x3<T>& B)             \
  {                                                                            \
    Matrix3x3<T> M(T(0.0));                                                    \
    for (int i = 0; i < 3; i++)                                                \
      for (int j = 0; j < 3; j++)                                              \
        M[i][j] = A[i][j] OP B[i][j];                                          \
    return M;                                                                  \
  }                                                                            \

SCALAR_MATRIX3x3_OPERATOR(*)

MATRIX3x3_SCALAR_OPERATOR(*)
MATRIX3x3_SCALAR_OPERATOR(/)

MATRIX3x3_SCALAR_COMPOUND_ASSIGMENT_OPERATOR(*=)
MATRIX3x3_SCALAR_COMPOUND_ASSIGMENT_OPERATOR(/=)

MATRIX3x3_MATRIX3x3_COMPOUND_ASSIGMENT_OPERATOR(+=)
MATRIX3x3_MATRIX3x3_COMPOUND_ASSIGMENT_OPERATOR(-=)

MATRIX3x3_MATRIX3x3_BINARY_OPERATOR(+)
MATRIX3x3_MATRIX3x3_BINARY_OPERATOR(-)

#undef SCALAR_MATRIX3x3_OPERATOR
#undef MATRIX3x3_SCALAR_OPERATOR
#undef MATRIX3x3_SCALAR_COMPOUND_ASSIGMENT_OPERATOR
#undef MATRIX3x3_MATRIX3x3_COMPOUND_ASSIGMENT_OPERATOR
#undef MATRIX3x3_MATRIX3x3_BINARY_OPERATOR

template <typename T>
Vector3D<T> operator*(const Matrix3x3<T>& A, const Vector3D<T>& v)
{
  return Vector3D<T>(A[0][0]*v[0] + A[0][1]*v[1] + A[0][2]*v[2],
                     A[1][0]*v[0] + A[1][1]*v[1] + A[1][2]*v[2],
                     A[2][0]*v[0] + A[2][1]*v[1] + A[2][2]*v[2]);
}

template <typename T>
Matrix3x3<T> operator*(const Matrix3x3<T>& A, const Matrix3x3<T>& B)
{
  return Matrix3x3<T>(A[0][0]*B[0][0] + A[0][1]*B[1][0] + A[0][2]*B[2][0],
                      A[0][0]*B[0][1] + A[0][1]*B[1][1] + A[0][2]*B[2][1],
                      A[0][0]*B[0][2] + A[0][1]*B[1][2] + A[0][2]*B[2][2],
                      A[1][0]*B[0][0] + A[1][1]*B[1][0] + A[1][2]*B[2][0],
                      A[1][0]*B[0][1] + A[1][1]*B[1][1] + A[1][2]*B[2][1],
                      A[1][0]*B[0][2] + A[1][1]*B[1][2] + A[1][2]*B[2][2],
                      A[2][0]*B[0][0] + A[2][1]*B[1][0] + A[2][2]*B[2][0],
                      A[2][0]*B[0][1] + A[2][1]*B[1][1] + A[2][2]*B[2][1],
                      A[2][0]*B[0][2] + A[2][1]*B[1][2] + A[2][2]*B[2][2]);
}

template <typename T>
Matrix3x3<T>& operator*=(const Matrix3x3<T>& A, const Matrix3x3<T>& B)
{
  A = A * B;
  return A;
}

template <typename T>
T det(const Matrix3x3<T>& M)
{
  return M[0][0] * (M[1][1]*M[2][2] - M[2][1]*M[1][2])
       + M[0][1] * (M[2][0]*M[1][2] - M[1][0]*M[2][2])
       + M[0][2] * (M[1][0]*M[2][1] - M[2][0]*M[1][1]);
}

template <typename T>
Matrix3x3<T> transpose(const Matrix3x3<T>& M)
{
  return Matrix3x3<T>(M[0][0], M[1][0], M[2][0],
                      M[0][1], M[1][1], M[2][1],
                      M[0][2], M[1][2], M[2][2]);
}

template <typename T>
Matrix3x3<T> inverse(const Matrix3x3<T>& M)
{
  return T(1.0/det(M)) * Matrix3x3<T>(M[1][1]*M[2][2] - M[1][2]*M[2][1],
                                      M[0][2]*M[2][1] - M[0][1]*M[2][2],
                                      M[0][1]*M[1][2] - M[0][2]*M[1][1],
                                      M[1][2]*M[2][0] - M[1][0]*M[2][2],
                                      M[0][0]*M[2][2] - M[0][2]*M[2][0],
                                      M[0][2]*M[1][0] - M[0][0]*M[1][2],
                                      M[1][0]*M[2][1] - M[1][1]*M[2][0],
                                      M[0][1]*M[2][0] - M[0][0]*M[2][1],
                                      M[0][0]*M[1][1] - M[0][1]*M[1][0]);
}

} // namespace Math
} // namespace VecCore

#endif
