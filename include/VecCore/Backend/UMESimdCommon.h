// internal header; common parts to multiple UMESIMD backends
#ifndef VECCORE_UMESIMDCOMMON_H
#define VECCORE_UMESIMDCOMMON_H

namespace vecCore {

// type traits for UME::SIMD
template <typename T, uint32_t N>
struct TypeTraits<UME::SIMD::SIMDVec_f<T, N>> {
  using ScalarType = T;
  using MaskType   = typename UME::SIMD::SIMDVecMask<N>;
  using IndexType  = typename UME::SIMD::SIMDVec_u<uint32_t, N>;
};

template <typename T, uint32_t N>
struct TypeTraits<UME::SIMD::SIMDVec_i<T, N>> {
  using ScalarType = T;
  using MaskType   = typename UME::SIMD::SIMDVecMask<N>;
  using IndexType  = typename UME::SIMD::SIMDVec_u<uint32_t, N>;
};

template <typename T, uint32_t N>
struct TypeTraits<UME::SIMD::SIMDVec_u<T, N>> {
  using ScalarType = T;
  using MaskType   = typename UME::SIMD::SIMDVecMask<N>;
  using IndexType  = typename UME::SIMD::SIMDVec_u<uint32_t, N>;
};


// backend functions for UME::SIMD

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
void MaskedAssign(UME::SIMD::SIMDVec_u<T, N>& dest,
                  UME::SIMD::SIMDVecMask<N> mask,
                  const UME::SIMD::SIMDVec_u<T, N> &src)
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
  return tval.blend(mask, fval);
}

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
typename UME::SIMD::SIMDVec_i<T, N>
Blend(const UME::SIMD::SIMDVecMask<N> mask,
      const UME::SIMD::SIMDVec_i<T, N>& tval,
      const UME::SIMD::SIMDVec_i<T, N>& fval)
{
  return tval.blend(mask, fval);
}


template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
typename UME::SIMD::SIMDVec_u<T, N>
Blend(const UME::SIMD::SIMDVecMask<N> mask,
      const UME::SIMD::SIMDVec_u<T, N>& tval,
      const UME::SIMD::SIMDVec_u<T, N>& fval)
{
  return tval.blend(mask, fval);
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


} // end namespace math
} // end namespace vecCore


#endif
