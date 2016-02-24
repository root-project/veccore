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
  using Float_v = Vc::SimdArray<Float_t, N>;
  using Double_v = Vc::SimdArray<Double_t, N>;

  using Int_v = Vc::SimdArray<Int_t, N>;
  using Int16_v = Vc::SimdArray<Int16_t, N>;
  using Int32_v = Vc::SimdArray<Int32_t, N>;
  using Int64_v = Vc::SimdArray<Int64_t, N>;

  using UInt_v = Vc::SimdArray<UInt_t, N>;
  using UInt16_v = Vc::SimdArray<UInt16_t, N>;
  using UInt32_v = Vc::SimdArray<UInt32_t, N>;
  using UInt64_v = Vc::SimdArray<UInt64_t, N>;

  template <typename T> using Vector = Vc::SimdArray<T, N>;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
