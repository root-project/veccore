#ifndef VECCORE_BACKEND_VC_SIMDARRAY_H
#define VECCORE_BACKEND_VC_SIMDARRAY_H

#ifdef VECCORE_ENABLE_VC

#include <cassert>
#include <cstdint>

#include <Vc/Vc>

namespace vecCore {
namespace backend {

template <>
template <typename T, std::size_t N>
struct ScalarType<typename Vc::SimdArray<T, N> > { typedef T Type; };

template <typename FloatT = float, std::size_t N = 16>
class VcSimdArray {
public:
  typedef bool Bool_t;
  typedef FloatT Real_t;
  typedef float Float_t;
  typedef double Double_t;

  typedef typename IntFor<Real_t>::Type Int_t;
  typedef typename UIntFor<Real_t>::Type UInt_t;

  typedef int16_t Int16_t;
  typedef int32_t Int32_t;
  typedef int64_t Int64_t;

  typedef uint16_t UInt16_t;
  typedef uint32_t UInt32_t;
  typedef uint64_t UInt64_t;

  typedef Vc::SimdArray<Real_t, N> Real_v;
  typedef Vc::SimdArray<Float_t, N> Float_v;
  typedef Vc::SimdArray<Double_t, N> Double_v;

  typedef Vc::SimdArray<Int_t, N> Int_v;
  typedef Vc::SimdArray<Int16_t, N> Int16_v;
  typedef Vc::SimdArray<Int32_t, N> Int32_v;
  typedef Vc::SimdArray<Int64_t, N> Int64_v;

  typedef Vc::SimdArray<UInt_t, N> UInt_v;
  typedef Vc::SimdArray<UInt16_t, N> UInt16_v;
  typedef Vc::SimdArray<UInt32_t, N> UInt32_v;
  typedef Vc::SimdArray<UInt64_t, N> UInt64_v;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
