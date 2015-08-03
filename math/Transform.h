#ifndef VECCORE_MATH_TRANSFORM_H
#define VECCORE_MATH_TRANSFORM_H

#include "math/Point3D.h"
#include "math/Vector3D.h"
#include "math/Quaternion.h"
#include "math/Matrix3x3.h"

namespace VecCore {
namespace Math    {

template <typename T, class Rotation> class Transform;

template <typename T>
class Transform<T, Matrix3x3<T> > {
private:
  Point3D<T> fOrigin;
  Matrix3x3<T> fBasis;
  unsigned char fType;

  enum {
    IDENTITY    = 0x0,
    TRANSLATION = 0x1,
    ROTATION    = 0x2,
    SCALING     = 0x4,
    LINEAR      = ROTATION | SCALING,
    AFFINE      = LINEAR | TRANSLATION
  };

public:
  Transform() : fBasis(T(1.0)), fOrigin(T(0.0)), fType(IDENTITY) {}

  Transform(const Matrix3x3<T>& M, const Point3D<T>& x) : fBasis(M), fOrigin(x)
  {
    fType = ROTATION;

    if (abs(det(M) - T(1.0)) > T(1.0e-6))
      fType |= SCALING;

    if (Norm(x) > T(1.0e-6))
      fType |= TRANSLATION;
  }

  Transform(const Quaternion<T>& q, const Point3D<T>& x) : fBasis(q), fOrigin(x)
  {
    fType = Norm(x) > T(1.0e-6) ? ROTATION | TRANSLATION : ROTATION;
  }

  template <typename T>
  Transform<T, Matrix3x3<T> >(const Transform& A)
    : fBasis(A.fBasis), fOrigin(A.fOrigin), fType(A.fType) {}

  const Point3D<T>& GetOrigin()  const { return fOrigin; }
  const Matrix3x3<T>& GetBasis() const { return  fBasis; }

  void SetIdentity();

  void SetOrigin(const Point3d<T>& p);
  void SetBasis(const Matrix3x3<T>& M);
  void SetRotation(const Quaternion& q);

  void Translate(const Vector& v);

  void Rotate(const Quaternion& q);

  void Scale(const real_t s);
  void Scale(const Vector& s);
  void Scale(real_t x, real_t y, real_t z);

  void invert(const Transform& t);

  Transform  operator*(const Transform& t);
  Transform& operator*=(const Transform& t);

  Point3D<T>  operator()(const Point3D<T>& x) const;

  /* vectors do not get translated */
  Vector3D<T> operator()(const Vector3D<T>& x) const;

  private:
  Matrix basis; Point origin; unsigned int fType;

  void mult(const Transform& t1, const Transform& t2);
  void mult_inverse_left(const Transform& t1, const Transform& t2);
  friend Transform inverse(const Transform& t);
};

Transform inverse(const Transform& t);

inline Transform& Transform::operator*=(const Transform& t)
{
  origin += basis * t.origin;
  basis *= t.basis;
  fType |= t.fType;
  return *this;
}

inline Transform Transform::operator*(const Transform& t)
{
  return Transform(basis * t.basis, (*this)(t.origin));
}

inline Point Transform::operator()(const Point& x) const
{
  return Point(basis*x + origin);
}

inline Point Transform::operator*(const Point& x) const
{
  return (*this)(x);
}

inline Vector Transform::operator()(const Vector& v) const
{
  return Vector(basis * v);
}

inline Vector Transform::operator*(const Vector& v) const
{
  return (*this)(v);
}

inline Matrix Transform::operator()(const Matrix& M) const
{
  Matrix MM = basis * M * (fType & SCALING ? transpose(basis) : inverse(basis));
  return MM;
}

inline void Transform::set_identity()
{
  basis.set_identity(); origin.set_zero();
  fType = IDENTITY;
}

inline void Transform::translate(const Vector& v)
{
  origin -= basis * v; fType |= TRANSLATION;
}

inline void Transform::rotate(const Quaternion& q)
{
  if (norm(q) < EPSILON) return;
  basis *= Matrix(q); fType |= ROTATION;
}

inline void Transform::scale(const real_t s)
{
  basis *= Matrix(s); fType |= SCALING;
}

inline void Transform::scale(const Vector& v)
{
  basis *= Matrix(v); fType |= SCALING;
}

inline void Transform::scale(real_t x, real_t y, real_t z)
{
  basis *= Matrix(x, y, z); fType |= SCALING;
}

inline const Matrix& Transform::get_basis() const { return basis; }
inline const Point& Transform::get_origin() const { return origin; }

inline void Transform::set_basis(const Matrix& M) { basis = M; }
inline void Transform::set_origin(const Point& p) { origin = p; }
inline void Transform::set_rotation(const Quaternion& q) { basis.set_rotation(q); }

inline void Transform::invert(const Transform& t)
{
  basis = t.fType & SCALING ? inverse(t.basis) : transpose(t.basis);
  origin = -(basis * t.origin); fType = t.fType;
}

inline void Transform::mult(const Transform& t1, const Transform& t2)
{
  basis = t1.basis * t2.basis; origin = t1(t2.origin);
  fType = t1.fType | t2.fType;
}

inline void Transform::mult_inverse_left(const Transform& t1, const Transform& t2)
{
  Vector v = t2.origin - t1.origin;
  if (t1.fType & SCALING) {
    Matrix inv = inverse(t1.basis); basis = inv * t2.basis; origin = inv * v;
  }
  else {
    basis = mult_transpose_left(t1.basis, t2.basis); origin = v * t1.basis;
  }
  fType = t1.fType | t2.fType;
}

inline Transform inverse(const Transform& t)
{
  Matrix basis = t.fType & Transform::SCALING ? inverse(t.basis) : transpose(t.basis);
  Point origin = -(basis * t.origin);
  return Transform(basis, origin);
}

}

#endif
