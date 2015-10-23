#ifndef VECCORE_MATH_TRANSFORM_H
#define VECCORE_MATH_TRANSFORM_H

#include "Point3D.h"
#include "Vector3D.h"
#include "Quaternion.h"
#include "Matrix3x3.h"

namespace veccore {

template <typename T, template <typename> class Rotation>
class Transform3D {
private:
  Point3D<T> fOrigin;
  Rotation<T> fRotation;
  T fScaling;

public:
  Transform3D()
    : fOrigin(Point3D<T>(T(0.0), T(0.0), T(0.0))),
      fRotation(Rotation<T>::Identity()),
      fScaling(T(1.0)) {}

  Transform3D(const Point3D<T>& x, const Rotation<T>& R)
    : fOrigin(x), fRotation(R), fScaling(T(1.0)) {}

  Transform3D(const Point3D<T>& x, const Rotation<T>& R, const T s)
    : fOrigin(x), fRotation(R), fScaling(s) {}

  Transform3D(const Transform3D& t)
    : fOrigin(t.fOrigin), fRotation(t.fRotation), fScaling(t.fScaling) {}

  Transform3D& operator=(const Transform3D& t)
  {
    fOrigin = t.fOrigin;
    fRotation = t.fRotation;
    fScaling = t.fScaling;
    return *this;
  }

  const Point3D<T>& GetOrigin() const { return fOrigin; }
  const Rotation<T>& GetRotation() const { return fRotation; }
  const T& GetScaling() const { return fScaling; }

  void SetOrigin(const Point3D<T>& x) { fOrigin = x; }
  void SetRotation(const Rotation<T>& R) { fRotation = R; }
  void SetScaling(const T s) { fScaling = s; }

  void SetIdentity()
  {
    fOrigin = Point3D<T>(T(0.0), T(0.0), T(0.0));
    fRotation = Rotation<T>::Identity();
    fScaling = T(1.0);
  }

  void Translate(const Vector3D<T>& v) { fOrigin += v; }

  void Rotate(const Rotation<T>& R) { fRotation *= R; }

  void Scale(const T s) { fScaling *= s; }

  Point3D<T> operator()(const Point3D<T>& p) const
  {
    return fScaling * (fRotation * p) + fOrigin;
  }

  Vector3D<T> operator()(const Vector3D<T>& v) const
  {
    return fScaling * (fRotation * v);
  }
};

template <typename T, template <typename> class Rotation>
Transform3D<T, Rotation> Inverse(const Transform3D<T, Rotation>& t)
{
  T s = T(1.0)/t.GetScaling();
  Rotation<T> InvR = Inverse(t.GetRotation());
  return Transform3D<T, Rotation>(-(InvR * (s * t.GetOrigin())), InvR, s);
}

} // namespace veccore

#endif
