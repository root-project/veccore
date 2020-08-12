#ifndef VECCORE_BACKEND_UMESIMDARRAY_H
#define VECCORE_BACKEND_UMESIMDARRAY_H

#ifdef VECCORE_ENABLE_UMESIMD

#include <umesimd/UMESimd.h>

namespace vecCore {

namespace backend {

// a UME backend with fixed-size types
template <int N = 16>
class UMESimdArray {
public:
  using Real_v   = UME::SIMD::SIMDVec<Real_s, N>;
  using Float_v  = UME::SIMD::SIMDVec<float, N>;
  using Double_v = UME::SIMD::SIMDVec<double, N>;

  using Int_v   = UME::SIMD::SIMDVec<int, N>;
  using Int16_v = UME::SIMD::SIMDVec<int16_t, N>;
  using Int32_v = UME::SIMD::SIMDVec<int32_t, N>;
  using Int64_v = UME::SIMD::SIMDVec<int64_t, N>;

  using UInt_v   = UME::SIMD::SIMDVec<unsigned int, N>;
  using UInt16_v = UME::SIMD::SIMDVec<uint16_t, N>;
  using UInt32_v = UME::SIMD::SIMDVec<uint32_t, N>;
  using UInt64_v = UME::SIMD::SIMDVec<uint64_t, N>;
};

} // namespace backend

} // namespace vecCore

// bring in common implementation
#include "UMESimdCommon.h"

#endif
#endif
