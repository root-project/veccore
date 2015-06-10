/// \file vector3d.h
/// \author Johannes de Fine Licht (johannes.definelicht@cern.ch)

#ifndef VECCORE_BASE_VECTOR3D_H_
#define VECCORE_BASE_VECTOR3D_H_

#include "VecCoreGlobal.h"
#include "BackendGlobal.h"
using VecCore::Backend::Vector::Abs;
using VecCore::Backend::Scalar::Abs;
using VecCore::Backend::Vector::MaskedAssign;
using VecCore::Backend::Scalar::MaskedAssign;
using VecCore::Backend::Vector::Sin;
using VecCore::Backend::Scalar::Cos;
using VecCore::Backend::Vector::Sqrt;
using VecCore::Backend::Scalar::Sqrt;
using VecCore::Backend::Vector::ATan2;
using VecCore::Backend::Scalar::ATan2;


//#ifndef VECCORE_NVCC
//  #if (defined(VECCORE_VC) || defined(VECCORE_VC_ACCELERATION))
    //#include <Vc/Vc>
  //#endif
//#endif

#include "base/AlignedBase.h"

#include <cstdlib>
#include <ostream>
#include <string>

namespace VecCore {

VECCORE_DEVICE_FORWARD_DECLARE( template <typename Type> class Vector3D; )

inline namespace VECCORE_IMPL_NAMESPACE {

/**
 * @brief Three dimensional vector class supporting most arithmetic operations.
 * @details If vector acceleration is enabled, the scalar template instantiation
 *          will use vector instructions for operations when possible.
 */
template <typename Type>
class Vector3D {
        //: public AlignedBase {

  typedef Vector3D<Type> VecType;

private:

  Type vec[3];

public:

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(const Type a, const Type b, const Type c) {
    vec[0] = a;
    vec[1] = b;
    vec[2] = c;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D() {
    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(const Type a) : vec{a,a,a} {}

  template <typename TypeOther>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D(Vector3D<TypeOther> const &other) : vec{ Type(other[0]), Type(other[1]), Type(other[2]) } {}

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D& operator=(Vector3D const &other) {
    vec[0] = other[0];
    vec[1] = other[1];
    vec[2] = other[2];
    return *this;
  }


//  /**
//   * Constructs a vector from an std::string of the same format as output by the
//   * "<<"-operator for outstreams.
//   * @param str String formatted as "(%d, %d, %d)".
//   */
//  VECCORE_CUDA_HEADER_HOST
//  Vector3D(std::string const &str) {
//    int begin = 1, end = str.find(",");
//    vec[0] = std::atof(str.substr(begin, end-begin).c_str());
//    begin = end + 2;
//    end = str.find(",", begin);
//    vec[1] = std::atof(str.substr(begin, end-begin).c_str());
//    begin = end + 2;
//    end = str.find(")", begin);
//    vec[2] = std::atof(str.substr(begin, end-begin).c_str());
//  }


  /**
   * Contains no check for correct indexing to avoid impairing performance.
   * @param index Index of content in the range [0-2].
   */
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type& operator[](const int index) {
    return vec[index];
  }

  /**
   * Contains no check for correct indexing to avoid impairing performance.
   * @param index Index of content in the range [0-2].
   */
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type const& operator[](const int index) const {
    return vec[index];
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type& x() { return vec[0]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type const& x() const { return vec[0]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type& y() { return vec[1]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type const& y() const { return vec[1]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type& z() { return vec[2]; }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type const& z() const { return vec[2]; }

  VECCORE_CUDA_HEADER_BOTH
  void Set(Type const &a, Type const &b, Type const &c) {
    vec[0] = a;
    vec[1] = b;
    vec[2] = c;
  }

  VECCORE_CUDA_HEADER_BOTH
  void Set(const Type a) {
    Set(a, a, a);
  }

  /// \return the length squared perpendicular to z direction
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Perp2() const {
    return vec[0]*vec[0]+vec[1]*vec[1];
  }

  /// \return the length perpendicular to z direction
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Perp() const {
    return Sqrt(Perp2());
  }

  template <typename Type2>
  ///The dot product of two Vector3D<T> objects
  /// \return T (where T is float, double, or various SIMD vector types)
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static
  Type Dot(Vector3D<Type> const &left, Vector3D<Type2> const &right) {
    return left[0]*right[0] + left[1]*right[1] + left[2]*right[2];
  }

  template <typename Type2>
  /// The dot product of two Vector3D<T> objects
  /// \return T (where T is float, double, or various SIMD vector types)
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Dot(Vector3D<Type2> const &right) const {
    return Dot(*this, right);
  }

  // For UVector3 compatibility. Is equal to normal multiplication.
  // TODO: check if there are implicit dot products in USolids...
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  VecType MultiplyByComponents(VecType const &other) const {
    return *this * other;
  }

  /// \return Squared magnitude of the vector.
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Mag2() const {
    return Dot(*this, *this);
  }

  /// \return Magnitude of the vector.
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Mag() const {
    return Sqrt(Mag2());
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Length() const {
    return Mag();
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Length2() const {
    return Mag2();
  }

  /// Normalizes the vector by dividing each entry by the length.
  /// \sa Vector3D::Length()
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void Normalize() {
    *this *= (1. / Length());
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<Type> Normalized() const {
    return Vector3D<Type>(*this) * (1. / Length());
  }

  /// \return Azimuthal angle between -pi and pi.
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Phi() const {
    //Type output = 0;
    //vecgeom::MaskedAssign(vec[0] != 0. || vec[1] != 0.,
    //                      ATan2(vec[1], vec[0]), &output);
    //return output;
    return ATan2(vec[1], vec[0]);
  }

  /// The cross (vector) product of two Vector3D<T> objects
  /// \return Type (where Type is float, double, or various SIMD vector types)
  template <class FirstType, class SecondType>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static Vector3D<Type> Cross(
      Vector3D<FirstType> const &left,
      Vector3D<SecondType> const &right) {
    return Vector3D<Type>(left[1]*right[2] - left[2]*right[1],
                          left[2]*right[0] - left[0]*right[2],
                          left[0]*right[1] - left[1]*right[0]);
  }

  /// The cross (vector) product of two Vector3D<T> objects
  /// \return Type (where Type is float, double, or various SIMD vector types)
  template <class OtherType>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<Type> Cross(Vector3D<OtherType> const &right) const {
    return Cross<Type, OtherType>(*this, right);
  }

  /// Maps each vector entry to a function that manipulates the entry type.
  /// \param f A function of type "Type f(const Type&)" to map over entries.
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void Map(Type (*f)(const Type&)) {
    vec[0] = f(vec[0]);
    vec[1] = f(vec[1]);
    vec[2] = f(vec[2]);
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Vector3D<Type> Abs() const {
    return Vector3D<Type>(Abs(vec[0]),
                          Abs(vec[1]),
                          Abs(vec[2]));
  }

  template <typename BoolType>
  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  void MaskedAssign(Vector3D<BoolType> const &condition,
                    Vector3D<Type> const &value) {
    vec[0] = (condition[0]) ? value[0] : vec[0];
    vec[1] = (condition[1]) ? value[1] : vec[1];
    vec[2] = (condition[2]) ? value[2] : vec[2];
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Min() const {
    Type min = (vec[1] < vec[0]) ? vec[1] : vec[0];
    min = (vec[2] < min) ? vec[2] : min;
    return min;
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  Type Max() const {
    Type max = (vec[1] > vec[0]) ? vec[1] : vec[0];
    max = (vec[2] > max) ? vec[2] : max;
    return max;
  }

//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  VecType Unit() const {
//    const Precision mag2 = Mag2();
//    VecType output(*this);
//    output /= Sqrt(mag2 + kMinimum);
//    return output;
//  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  static VecType FromCylindrical(Type r, Type phi, Type z) {
    return VecType(r*Cos(phi), r*Sin(phi), z);
  }

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  VecType& FixZeroes() {
    for (int i = 0; i < 3; ++i) {
      MaskedAssign(Abs(vec[i]) < kTolerance, 0., &vec[i]);
    }
    return *this;
  }

  // Inplace binary operators

  #define VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(OPERATOR) \
  VECCORE_CUDA_HEADER_BOTH \
  VECCORE_INLINE \
  VecType& operator OPERATOR(const VecType &other) { \
    vec[0] OPERATOR other.vec[0]; \
    vec[1] OPERATOR other.vec[1]; \
    vec[2] OPERATOR other.vec[2]; \
    return *this; \
  } \
  template <typename OtherType> \
  VECCORE_CUDA_HEADER_BOTH \
  VECCORE_INLINE \
  VecType& operator OPERATOR(const Vector3D<OtherType> &other) { \
    vec[0] OPERATOR other[0]; \
    vec[1] OPERATOR other[1]; \
    vec[2] OPERATOR other[2]; \
    return *this; \
  } \
  VECCORE_CUDA_HEADER_BOTH \
  VECCORE_INLINE \
  VecType& operator OPERATOR(const Type &scalar) { \
    vec[0] OPERATOR scalar; \
    vec[1] OPERATOR scalar; \
    vec[2] OPERATOR scalar; \
    return *this; \
  }
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(+=)
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(-=)
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(*=)
  VECTOR3D_TEMPLATE_INPLACE_BINARY_OP(/=)
  #undef VECTOR3D_TEMPLATE_INPLACE_BINARY_OP

  VECCORE_CUDA_HEADER_BOTH
  VECCORE_INLINE
  operator bool() const {
    return vec[0] && vec[1] && vec[2];
  }

};

template <typename T>
std::ostream& operator<<(std::ostream& os, Vector3D<T> const &vec) {
  os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
  return os;
}

//#ifdef VECCORE_VC_ACCELERATION
//
///// This is a template specialization of class Vector3D<double> or
///// Vector3D<float> that can provide internal vectorization of common vector
///// operations.
//
//
//
//template <>
//class Vector3D<Precision> : public AlignedBase {
//
//  typedef Vector3D<Precision> VecType;
//  typedef Vector3D<bool> BoolType;
//  typedef Vc::Vector<Precision> Base_t;
//
//private:
//
//  Vc::Memory<Vc::Vector<Precision>, 3> mem;
//
//public:
//
//  Precision* AsArray() {
//    return &mem[0];
//  }
//
// Vector3D(const Precision a, const Precision b, const Precision c) : mem() {
//    mem[0] = a;
//    mem[1] = b;
//    mem[2] = c;
//  }
//
//  // Performance issue in Vc with: mem = a;
//  VECCORE_INLINE
//  Vector3D(const Precision a) : Vector3D(a, a, a) {}
//
//  VECCORE_INLINE
//  Vector3D() : Vector3D(0, 0, 0) {}
//
//  VECCORE_INLINE
//    Vector3D(Vector3D const &other) : mem() {
//    //for( int i=0; i < 1 + 3/Base_t::Size; i++ )
////      {
//         //Base_t v1 = other.mem.vector(i);
//         //this->mem.vector(i)=v1;
//       //}
//     mem[0]=other.mem[0];
//     mem[1]=other.mem[1];
//     mem[2]=other.mem[2];
//  }
//
//  VECCORE_INLINE
//  Vector3D & operator=( Vector3D const & rhs )
//   {
//
//      //for( int i=0; i < 1 + 3/Base_t::Size; i++ )
//      //{
//         //Base_t v1 = rhs.mem.vector(i);
//         //this->mem.vector(i)=v1;
//       //}
//      // the following line must not be used: this is a bug in Vc
//      // this->mem = rhs.mem;
//      mem[0]=rhs.mem[0];
//      mem[1]=rhs.mem[1];
//      mem[2]=rhs.mem[2];
//      return *this;
//   }
//
//  Vector3D(std::string const &str) : mem() {
//    int begin = 1, end = str.find(",");
//    mem[0] = atof(str.substr(begin, end-begin).c_str());
//    begin = end + 2;
//    end = str.find(",", begin);
//    mem[1] = atof(str.substr(begin, end-begin).c_str());
//    begin = end + 2;
//    end = str.find(")", begin);
//    mem[2] = atof(str.substr(begin, end-begin).c_str());
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision& operator[](const int index) {
//    return mem[index];
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  const Precision& operator[](const int index) const {
//    return mem[index];
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision& x() { return mem[0]; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  const Precision& x() const { return mem[0]; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision& y() { return mem[1]; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  const Precision& y() const { return mem[1]; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision& z() { return mem[2]; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  const Precision& z() const { return mem[2]; }
//
//  VECCORE_CUDA_HEADER_BOTH
//  void Set(const Precision x, const Precision y, const Precision z) {
//    mem[0] = x;
//    mem[1] = y;
//    mem[2] = z;
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  void Set(const Precision x) {
//    Set(x, x, x);
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Length() const {
//    return sqrt(mem[0]*mem[0] + mem[1]*mem[1] + mem[2]*mem[2]);
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Mag2() const {
//      return Dot(*this,*this);
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Mag() const {
//    return Sqrt(Mag2());
//  }
//
//  // TODO: study if we gain from internal vectorization here.
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Perp2() const {
//    return mem[0]*mem[0] + mem[1]*mem[1];
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Perp() const {
//    return Sqrt(Perp2());
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  void Normalize() {
//    *this *= 1. / Length();
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Vector3D<Precision> Normalized() const;
//
//  VECCORE_INLINE
//  void Map(Precision (*f)(const Precision&)) {
//    mem[0] = f(mem[0]);
//    mem[1] = f(mem[1]);
//    mem[2] = f(mem[2]);
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  void MaskedAssign(Vector3D<bool> const &condition,
//                    Vector3D<Precision> const &value) {
//    mem[0] = (condition[0]) ? value[0] : mem[0];
//    mem[1] = (condition[1]) ? value[1] : mem[1];
//    mem[2] = (condition[2]) ? value[2] : mem[2];
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Min() const {
//    return std::min(std::min(mem[0], mem[1]), mem[2]);
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Max() const {
//    return std::max(std::max(mem[0], mem[1]), mem[2]);
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  VecType Unit() const {
//    const Precision mag2 = Mag2();
//    VecType output(*this);
//    output /= Sqrt(mag2 + kMinimum);
//    return output;
//  }
//
//  /// \return Azimuthal angle between -pi and pi.
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Phi() const {
//    return (mem[0] != 0. || mem[1] != 0.) ? ATan2(mem[1], mem[0]) : 0.;
//  }
//
//  /// \return The dot product of two Vector3D<Precision> objects.
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  static
//  Precision Dot(Vector3D<Precision> const &left,
//                Vector3D<Precision> const &right) {
//    // TODO: This function should be internally vectorized (if proven to be
//    //       beneficial)
//
//    // To avoid to initialize the padding component, we can not use mem.vector's
//    // multiplication and addition since it would accumulate also the (random) padding component
//    return left.mem[0]*right.mem[0] + left.mem[1]*right.mem[1] + left.mem[2]*right.mem[2];
//  }
//
//  /// \return The dot product with another Vector3D<Precision> object.
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Precision Dot(Vector3D<Precision> const &right) const {
//    return Dot(*this, right);
//  }
//
//  // For UVector3 compatibility. Is equal to normal multiplication.
//  // TODO: check if there are implicit dot products in USolids...
//  VECCORE_INLINE
//  VecType MultiplyByComponents(VecType const &other) const;
//
//  /// The cross product of two Vector3D<T> objects.
//  /// \return Type (where Type is float, double, or various SIMD vector types)
//  template <class FirstType, class SecondType>
//  VECCORE_INLINE
//  static Vector3D<Precision> Cross(
//      Vector3D<FirstType> const &left,
//      Vector3D<SecondType> const &right) {
//    return Vector3D<Precision>(left[1]*right[2] - left[2]*right[1],
//                               left[2]*right[0] - left[0]*right[2],
//                               left[0]*right[1] - left[1]*right[0]);
//  }
//
//  /// The cross product with another Vector3D<T> objects
//  /// \return Type (where Type is float, double, or various SIMD vector types)
//  template <class OtherType>
//  VECCORE_INLINE
//  Vector3D<Precision> Cross(Vector3D<OtherType> const &right) const {
//    return Cross<Precision, OtherType>(*this, right);
//  }
//
//  /// Returns absolute value of the vector (as a vector).
//  VECCORE_INLINE
//  Vector3D<Precision> Abs() const {
//    VecType tmp;
//    for (unsigned i = 0; i < 1 + 3/Vc::Vector<Precision>::Size; ++i) {
//      Base_t v = this->mem.vector(i);
//      tmp.mem.vector(i) = Vc::abs( v );
//    }
//    return tmp;
//  }
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  Vector3D<Precision>& FixZeroes() {
//    for (int i = 0; i < 3; ++i) {
//      if (std::abs(mem.scalar(i)) < kTolerance) mem.scalar(i) = 0;
//    }
//    return *this;
//  }
//
//  // Inplace binary operators
//
//  #define VECTOR3D_ACCELERATED_INPLACE_BINARY_OP(OPERATOR) \
//  VECCORE_CUDA_HEADER_BOTH \
//  VecType& operator OPERATOR(const VecType &other) { \
//    for (unsigned i = 0; i < 1 + 3/Vc::Vector<Precision>::Size; ++i) { \
//      this->mem.vector(i) OPERATOR other.mem.vector(i); \
//    } \
//    return *this; \
//  } \
//  VECCORE_CUDA_HEADER_BOTH \
//  VecType& operator OPERATOR(const Precision &scalar) { \
//    for (unsigned i = 0; i < 1 + 3/Vc::Vector<Precision>::Size; ++i) { \
//      this->mem.vector(i) OPERATOR scalar; \
//    } \
//    return *this; \
//  }
//  VECTOR3D_ACCELERATED_INPLACE_BINARY_OP(+=)
//  VECTOR3D_ACCELERATED_INPLACE_BINARY_OP(-=)
//  VECTOR3D_ACCELERATED_INPLACE_BINARY_OP(*=)
//  VECTOR3D_ACCELERATED_INPLACE_BINARY_OP(/=)
//  #undef VECTOR3D_ACCELERATED_INPLACE_BINARY_OP
//
//  VECCORE_CUDA_HEADER_BOTH
//  VECCORE_INLINE
//  static VecType FromCylindrical(Precision r, Precision phi, Precision z) {
//    return VecType(r*cos(phi), r*sin(phi), z);
//  }
//
//};
//#else
////#pragma message "using normal Vector3D.h"
//#endif // VECCORE_VC_ACCELERATION


#define VECTOR3D_BINARY_OP(OPERATOR, INPLACE) \
template <typename Type, typename OtherType> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
Vector3D<Type> operator OPERATOR(const Vector3D<Type> &lhs, \
                                 const Vector3D<OtherType> &rhs) { \
  Vector3D<Type> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename Type, typename ScalarType> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
Vector3D<Type> operator OPERATOR(Vector3D<Type> const &lhs, \
                                 const ScalarType rhs) { \
  Vector3D<Type> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename Type, typename ScalarType> \
VECCORE_INLINE \
VECCORE_CUDA_HEADER_BOTH \
Vector3D<Type> operator OPERATOR(const ScalarType lhs, \
                                 Vector3D<Type> const &rhs) { \
  Vector3D<Type> result(lhs); \
  result INPLACE rhs; \
  return result; \
}
VECTOR3D_BINARY_OP(+, +=)
VECTOR3D_BINARY_OP(-, -=)
VECTOR3D_BINARY_OP(*, *=)
VECTOR3D_BINARY_OP(/, /=)
#undef VECTOR3D_BINARY_OP

//VECCORE_INLINE
//VECCORE_CUDA_HEADER_BOTH
//bool operator==(
//    Vector3D<Precision> const &lhs,
//    Vector3D<Precision> const &rhs) {
//  return Abs(lhs[0] - rhs[0]) < kTolerance &&
//         Abs(lhs[1] - rhs[1]) < kTolerance &&
//         Abs(lhs[2] - rhs[2]) < kTolerance;
//}

//VECCORE_INLINE
//VECCORE_CUDA_HEADER_BOTH
//Vector3D<bool> operator!=(
//    Vector3D<Precision> const &lhs,
//    Vector3D<Precision> const &rhs) {
//  return !(lhs == rhs);
//}

template <typename Type>
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Vector3D<Type> operator-(Vector3D<Type> const &vec) {
  return Vector3D<Type>(-vec[0], -vec[1], -vec[2]);
}

VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
Vector3D<bool> operator!(Vector3D<bool> const &vec) {
  return Vector3D<bool>(!vec[0], !vec[1], !vec[2]);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#define VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP(OPERATOR) \
VECCORE_CUDA_HEADER_BOTH \
VECCORE_INLINE \
Vector3D<bool> operator OPERATOR(Vector3D<bool> const &lhs, \
                                 Vector3D<bool> const &rhs) { \
  return Vector3D<bool>(lhs[0] OPERATOR rhs[0], \
                        lhs[1] OPERATOR rhs[1], \
                        lhs[2] OPERATOR rhs[2]); \
}
VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP(&&)
VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP(||)
#undef VECTOR3D_SCALAR_BOOLEAN_LOGICAL_OP
#pragma GCC diagnostic pop

//#ifdef VECCORE_VC
//
//VECCORE_INLINE
//Vector3D<VcBool> operator!(Vector3D<VcBool> const &vec) {
//  return Vector3D<VcBool>(!vec[0], !vec[1], !vec[2]);
//}
//
//VECCORE_INLINE
//VcBool operator==(
//    Vector3D<VcPrecision> const &lhs,
//    Vector3D<VcPrecision> const &rhs) {
//  return Abs(lhs[0] - rhs[0]) < kTolerance &&
//         Abs(lhs[1] - rhs[1]) < kTolerance &&
//         Abs(lhs[2] - rhs[2]) < kTolerance;
//}
//
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Weffc++"
//#define VECTOR3D_VC_BOOLEAN_LOGICAL_OP(OPERATOR) \
//VECCORE_INLINE \
//Vector3D<VcBool> operator OPERATOR( \
//    Vector3D<VcBool> const &lhs, \
//    Vector3D<VcBool> const &rhs) { \
//  return Vector3D<VcBool>(lhs[0] OPERATOR rhs[0], \
//                          lhs[1] OPERATOR rhs[1], \
//                          lhs[2] OPERATOR rhs[2]); \
//}
//VECTOR3D_VC_BOOLEAN_LOGICAL_OP(&&)
//VECTOR3D_VC_BOOLEAN_LOGICAL_OP(||)
//#undef VECTOR3D_VC_BOOLEAN_LOGICAL_OP
//#pragma GCC diagnostic pop
//
//#endif // VECCORE_VC


#ifdef VECCORE_VC_ACCELERATION

Vector3D<Precision> Vector3D<Precision>::Normalized() const {
  return Vector3D<Precision>(*this) * (1. / Length());
}

Vector3D<Precision> Vector3D<Precision>::MultiplyByComponents(
    VecType const &other) const {
  return (*this) * other;
}

#endif

} // End inline namespace

} // End global namespace

#endif // VECCORE_BASE_VECTOR3D_H_
