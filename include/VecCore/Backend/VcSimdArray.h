#ifndef VECCORE_BACKEND_VC_SIMDARRAY_H
#define VECCORE_BACKEND_VC_SIMDARRAY_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {
namespace backend {

template <typename T, std::size_t N>
struct ScalarType<typename Vc::SimdArray<T, N> > { typedef T Type; };

template <std::size_t N = 16>
class VcSimdArray {
public:
  template <typename T>
  using Vector = Vc::SimdArray<T, N>;

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
