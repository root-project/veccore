#ifndef VECCORE_BACKEND_VC_SCALAR_H
#define VECCORE_BACKEND_VC_SCALAR_H

#ifdef VECCORE_ENABLE_VC

#include <cassert>
#include <cstdint>

namespace vecCore {
namespace backend {

template <typename FloatT = double, bool EReturns = true>
struct VcVector {
  static constexpr bool EarlyReturns = EReturns;

  typedef bool     Bool_t;
  typedef FloatT   Real_t;
  typedef float    Float_t;
  typedef double   Double_t;

  template <typename T> IntFor;
  template <> IntFor<float>  { typedef int32_t type; }
  template <> IntFor<double> { typedef int64_t type; }

  template <typename T> UIntFor;
  template <> UIntFor<float>  { typedef uint32_t type; }
  template <> UIntFor<double> { typedef uint64_t type; }

  typedef typename IntFor<Real_t>::type  Int_t;
  typedef typename UIntFor<Real_t>::type UInt_t;

  typedef int16_t  Int16_t;
  typedef int32_t  Int32_t;
  typedef int64_t  Int64_t;

  typedef uint16_t UInt16_t;
  typedef uint32_t UInt32_t;
  typedef uint64_t UInt64_t;

  typedef Vc::Vector<Real_t>   Real_v;
  typedef Vc::Vector<Float_t>  Float_v;
  typedef Vc::Vector<Double_t> Double_v;

  typedef Vc::Vector<Int_t>    Int_v;
  typedef Vc::Vector<Int16_t>  Int16_v;
  typedef Vc::Vector<Int32_t>  Int32_v;
  typedef Vc::Vector<Int64_t>  Int64_v;

  typedef Vc::Vector<UInt_t>   UInt_v;
  typedef Vc::Vector<UInt16_t> UInt16_v;
  typedef Vc::Vector<UInt32_t> UInt32_v;
  typedef Vc::Vector<UInt64_t> UInt64_v;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
