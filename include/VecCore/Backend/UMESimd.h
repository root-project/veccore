#ifndef VECCORE_BACKEND_UMESIMD_H
#define VECCORE_BACKEND_UMESIMD_H

#ifdef VECCORE_ENABLE_UMESIMD

#include <umesimd/UMESimd.h>

namespace vecCore {

namespace backend {

// a UME::SIMD backend with automatically chosen SIMD sizes
class UMESimd {
public:
  using Real_v   = UME::SIMD::SIMDVec<Real_s, SIMDWidth<Real_s>()>;
  using Float_v  = UME::SIMD::SIMDVec<float, SIMDWidth<float>()>;
  using Double_v = UME::SIMD::SIMDVec<double, SIMDWidth<double>()>;

  using Int_v   = UME::SIMD::SIMDVec<int, SIMDWidth<int>()>;
  using Int16_v = UME::SIMD::SIMDVec<int16_t, SIMDWidth<int16_t>()>;
  using Int32_v = UME::SIMD::SIMDVec<int32_t, SIMDWidth<int32_t>()>;
  using Int64_v = UME::SIMD::SIMDVec<int64_t, SIMDWidth<int64_t>()>;

  using UInt_v   = UME::SIMD::SIMDVec<unsigned int, SIMDWidth<unsigned int>()>;
  using UInt16_v = UME::SIMD::SIMDVec<uint16_t, SIMDWidth<uint16_t>()>;
  using UInt32_v = UME::SIMD::SIMDVec<uint32_t, SIMDWidth<uint32_t>()>;
  using UInt64_v = UME::SIMD::SIMDVec<uint64_t, SIMDWidth<uint64_t>()>;
};

} // namespace backend

} // namespace vecCore

// bring in common implementation
#include "UMESimdCommon.h"

#endif
#endif
