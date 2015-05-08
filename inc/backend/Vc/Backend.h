/// \file vc/backend.h

#ifndef VECCORE_BACKEND_VCBACKEND_H_
#define VECCORE_BACKEND_VCBACKEND_H_

#include "VecCoreGlobal.h"
#include "backend/scalar/Backend.h"

#include <Vc/Vc>

namespace VecCore {

namespace Backend {

namespace Vector {

inline namespace VECGEOM_IMPL_NAMESPACE {


template< typename Real_t = VecCore::DefaultPrecision_t >
struct kVc {
  // The convention for type names is
  // _t is a type but it should be a primitive or otherwise scalar type
  // _v means that this type could potentially be a vector type ( but not necessarily )

  // define the base primitive type
  typedef Real_t                      Real_t;
  // we could define the other base primitive types
  typedef double                      Double_t;
  typedef double                      Float_t;

  // define vector types for wanted precision
  typedef Vc::Vector<Real_t>          Real_v;
  typedef Vc::Vector<Real_t>::Mask    Bool_v; // ( should we call this RealBool )

  // define vector types for standard double + float types
  typedef Vc::Vector<double>          Double_v;
  typedef Vc::Vector<float>           Float_v;
  typedef Vc::Vector<double>::Mask    DoubleBool_v;
  typedef Vc::Vector<float>::Mask     FloatBool_v;

  typedef Vc::int_v                   Int_v;
  typedef Int_v                       Inside_v;

  // alternative typedefs ( might supercede above typedefs )
  // We can no longer define Double_t like this this !!
  //typedef Vc::int_v                   Int_t;
  //typedef Vc::Vector<Precision>       Double_t;
  //typedef Vc::Vector<Precision>::Mask Bool_t;
  typedef Vc::Vector<Real_t>          Index_t;

  // numeric constants of this backend
  const static Real_v kOne;
  const static Real_v kZero;
  const static Bool_v kTrue;
  const static Bool_v kFalse;

  // what about numeric constants for Double_v or Float_v
  // could by like
  const static Double_v kDoubleOne;
  const static Double_v kDoubleZero;
  const static DoubleBool_v kDoubleTrue;
  const static DoubleBool_v kDoubleFalse;

  // other properties of this backend
  constexpr static bool early_returns = false;

  // the VectorSizes
  constexpr int kRealVectorSize = Real_v::Size;
  constexpr int kDoubleVectorSize = Double_v::Size;
  constexpr int kFloatVectorSize = Float_v::Size;
  // ... in principle also kIntVectorSize ...
};

// these have to be replace by something like BackendInt, BackendReal, ...
typedef kVc::int_v       VcInt;
typedef kVc::precision_v VcPrecision;
typedef kVc::bool_v      VcBool;
typedef kVc::inside_v    VcInside;

template <typename Type>
VECGEOM_INLINE
void CondAssign(typename Vc::Vector<Type>::Mask const &cond,
                Vc::Vector<Type> const &thenval,
                Vc::Vector<Type> const &elseval,
                Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
  (*output)(!cond) = elseval;
}

template <typename Type>
VECGEOM_INLINE
void CondAssign(typename Vc::Vector<Type>::Mask const &cond,
                Type const &thenval,
                Type const &elseval,
                Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
  (*output)(!cond) = elseval;
}

template <typename Type>
VECGEOM_INLINE
void MaskedAssign(typename Vc::Vector<Type>::Mask const &cond,
                  Vc::Vector<Type> const &thenval,
                  Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
}

template <typename Type>
VECGEOM_INLINE
void MaskedAssign(typename Vc::Vector<Type>::Mask const &cond,
                  Type const &thenval,
                  Vc::Vector<Type> *const output) {
  (*output)(cond) = thenval;
}

// VECGEOM_INLINE
// void MaskedAssign(VcBool const &cond,
//                   const kScalar::int_v thenval,
//                   VcInt *const output) {
//   (*output)(VcInt::Mask(cond)) = thenval;
// }

VECGEOM_INLINE
void MaskedAssign(VcBool const &cond,
                  const Inside_t thenval,
                  VcInside *const output) {
  (*output)(VcInside::Mask(cond)) = thenval;
}


VECGEOM_INLINE
bool IsFull(VcBool const &cond) {
  return cond.isFull();
}

VECGEOM_INLINE
bool Any(VcBool const &cond) {
  return !cond.isEmpty();
}

VECGEOM_INLINE
bool IsEmpty(VcBool const &cond) {
  return cond.isEmpty();
}

VECGEOM_INLINE
VcPrecision Abs(VcPrecision const &val) {
  return Vc::abs(val);
}

VECGEOM_INLINE
VcPrecision Sqrt(VcPrecision const &val) {
  return Vc::sqrt(val);
}

VECGEOM_INLINE
VcPrecision ATan2(VcPrecision const &y, VcPrecision const &x) {
  return Vc::atan2(y, x);
}


VECGEOM_INLINE
VcPrecision sin(VcPrecision const &x) {
  return Vc::sin(x);
}

VECGEOM_INLINE
VcPrecision cos(VcPrecision const &x) {
  return Vc::cos(x);
}

VECGEOM_INLINE
VcPrecision tan(VcPrecision const &radians) {
  // apparently Vc does not have a tan function
  //  return Vc::tan(radians);
  // emulating it for the moment
  VcPrecision s,c;
  Vc::sincos(radians,&s,&c);
  return s/c;
}

VECGEOM_INLINE
Precision Pow(Precision const &x, Precision arg) {
   return std::pow(x,arg);
}

VECGEOM_INLINE
VcPrecision Min(VcPrecision const &val1, VcPrecision const &val2) {
  return Vc::min(val1, val2);
}

VECGEOM_INLINE
VcPrecision Max(VcPrecision const &val1, VcPrecision const &val2) {
  return Vc::max(val1, val2);
}

VECGEOM_INLINE
VcInt Min(VcInt const &val1, VcInt const &val2) {
  return Vc::min(val1, val2);
}

VECGEOM_INLINE
VcInt Max(VcInt const &val1, VcInt const &val2) {
  return Vc::max(val1, val2);
}


VECGEOM_INLINE
VcPrecision Floor( VcPrecision const &val ){
  return Vc::floor( val );
}

} // End inline namespace

} // end Vector namespace

} // end Backend namespace

} // End global namespace


#endif // VECCORE_BACKEND_VCBACKEND_H_
