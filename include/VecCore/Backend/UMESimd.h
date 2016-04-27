#ifndef VECCORE_BACKEND_UMESIMD_H
#define VECCORE_BACKEND_UMESIMD_H

#ifdef VECCORE_ENABLE_UMESIMD

#include <umesimd/UMESimd.h>

namespace vecCore {

template <typename T, uint32_t N>
struct TypeTraits<UME::SIMD::SIMDVec_f<T, N>> {
  using ScalarType = T;
  using MaskType   = typename UME::SIMD::SIMDVecMask<N>;
  using IndexType  = typename UME::SIMD::SIMDVec_i<uint32_t, N>;
};

template <typename T, uint32_t N>
struct TypeTraits<UME::SIMD::SIMDVec_i<T, N>> {
  using ScalarType = T;
  using MaskType   = typename UME::SIMD::SIMDVecMask<N>;
  using IndexType  = typename UME::SIMD::SIMDVec_i<uint32_t, N>;
};

namespace backend {

template <uint32_t N = 16>
class UMESimd {
public:
  using Real_v   = UME::SIMD::SIMDVec<Real_s, N>;
  using Float_v  = UME::SIMD::SIMDVec<Float_s, N>;
  using Double_v = UME::SIMD::SIMDVec<Double_s, N>;

  using Int_v    = UME::SIMD::SIMDVec<Int_s, N>;
  using Int16_v  = UME::SIMD::SIMDVec<Int16_s, N>;
  using Int32_v  = UME::SIMD::SIMDVec<Int32_s, N>;
  using Int64_v  = UME::SIMD::SIMDVec<Int64_s, N>;

  using UInt_v   = UME::SIMD::SIMDVec<UInt_s, N>;
  using UInt16_v = UME::SIMD::SIMDVec<UInt16_s, N>;
  using UInt32_v = UME::SIMD::SIMDVec<UInt32_s, N>;
  using UInt64_v = UME::SIMD::SIMDVec<UInt64_s, N>;
};

} // namespace backend

template <uint32_t N>
VECCORE_FORCE_INLINE
Bool_s MaskFull(const UME::SIMD::SIMDVecMask<N> cond)
{
  return cond.hland();
}

template <uint32_t N>
VECCORE_FORCE_INLINE
Bool_s MaskEmpty(const UME::SIMD::SIMDVecMask<N> cond)
{
  return !cond.hlor();
}

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
void MaskedAssign(UME::SIMD::SIMDVec_f<T, N>& dest,
                  UME::SIMD::SIMDVecMask<N> mask,
                  const UME::SIMD::SIMDVec_f<T, N> &src)
{
  dest.assign(mask, src);
}

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
void MaskedAssign(UME::SIMD::SIMDVec_i<T, N>& dest,
                  UME::SIMD::SIMDVecMask<N> mask,
                  const UME::SIMD::SIMDVec_i<T, N> &src)
{
  dest.assign(mask, src);
}

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
typename UME::SIMD::SIMDVec_f<T, N>
Blend(const UME::SIMD::SIMDVecMask<N> mask,
      const UME::SIMD::SIMDVec_f<T, N>& tval,
      const UME::SIMD::SIMDVec_f<T, N>& fval)
{
  UME::SIMD::SIMDVec_f<T, N> tmp(fval);
  tmp.assign(mask, tval);
  return tmp;
}

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
typename UME::SIMD::SIMDVec_i<T, N>
Blend(const UME::SIMD::SIMDVecMask<N> mask,
      const UME::SIMD::SIMDVec_i<T, N>& tval,
      const UME::SIMD::SIMDVec_i<T, N>& fval)
{
  UME::SIMD::SIMDVec<T, N> tmp(fval);
  tmp.assign(mask, tval);
  return tmp;
}

namespace math {

#define UMESIMD_INT_FUNC(f, name)                                               \
  template <typename T, uint32_t N>                                             \
  VECCORE_FORCE_INLINE                                                          \
  typename UME::SIMD::SIMDVec_i<T, N> f(UME::SIMD::SIMDVec_i<T, N> x) {         \
    return x.name();                                                            \
  }

#define UMESIMD_REAL_FUNC(f, name)                                              \
  template <typename T, uint32_t N>                                             \
  VECCORE_FORCE_INLINE                                                          \
  typename UME::SIMD::SIMDVec_f<T, N> f(UME::SIMD::SIMDVec_f<T, N> x) {         \
    return x.name();                                                            \
  }

UMESIMD_REAL_FUNC(Abs, abs)
UMESIMD_REAL_FUNC(Exp, exp)
UMESIMD_REAL_FUNC(Log, log)
UMESIMD_REAL_FUNC(Sin, sin)
UMESIMD_REAL_FUNC(Cos, cos)
UMESIMD_REAL_FUNC(Tan, tan)
UMESIMD_REAL_FUNC(Atan, atan)
UMESIMD_REAL_FUNC(Sqrt, sqrt)
UMESIMD_REAL_FUNC(Round, round)
UMESIMD_REAL_FUNC(Floor, floor)
UMESIMD_REAL_FUNC(Ceil, ceil)

}

} // namespace vecCore

#endif
#endif
