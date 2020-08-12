// internal header; common parts to multiple UMESIMD backends
#ifndef VECCORE_UMESIMDCOMMON_H
#define VECCORE_UMESIMDCOMMON_H

namespace vecCore {

// type traits for UME::SIMD

template <uint32_t N>
struct TypeTraits<UME::SIMD::SIMDVecMask<N>> {
  using MaskType   = typename UME::SIMD::SIMDVecMask<N>;
  using IndexType  = int;
  using ScalarType = bool;
};

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
bool MaskFull(const UME::SIMD::SIMDVecMask<N> &cond)
{
  return cond.hland();
}

template <uint32_t N>
VECCORE_FORCE_INLINE
bool MaskEmpty(const UME::SIMD::SIMDVecMask<N> &cond)
{
  return !cond.hlor();
}

template <uint32_t N>
struct IndexingImplementation<UME::SIMD::SIMDVecMask<N>> {
  using M = UME::SIMD::SIMDVecMask<N>;

  VECCORE_FORCE_INLINE VECCORE_ATT_HOST_DEVICE static bool Get(const M &mask, int i) { return mask[i]; }

  VECCORE_FORCE_INLINE VECCORE_ATT_HOST_DEVICE static void Set(M &mask, int i, const bool val)
  {
    mask.insert(i, val);
  }
};

template <typename T, uint32_t N>
struct LoadStoreImplementation<UME::SIMD::SIMDVec_f<T, N>> {
  using V = UME::SIMD::SIMDVec_f<T, N>;

  template <typename S = Scalar<V>>
  static inline void Load(V &v, S const *ptr)
  {
    v.load(ptr);
  }

  template <typename S = Scalar<V>>
  static inline void Store(V const &v, S *ptr)
  {
    v.store(ptr);
  }
};

template <typename T, uint32_t N>
struct LoadStoreImplementation<UME::SIMD::SIMDVec_i<T, N>> {
  using V = UME::SIMD::SIMDVec_i<T, N>;

  template <typename S = Scalar<V>>
  static inline void Load(V &v, S const *ptr)
  {
    v.load(ptr);
  }

  template <typename S = Scalar<V>>
  static inline void Store(V const &v, S *ptr)
  {
    v.store(ptr);
  }
};

template <typename T, uint32_t N>
struct LoadStoreImplementation<UME::SIMD::SIMDVec_u<T, N>> {
  using V = UME::SIMD::SIMDVec_u<T, N>;

  template <typename S = Scalar<V>>
  static inline void Load(V &v, S const *ptr)
  {
    v.load(ptr);
  }

  template <typename S = Scalar<V>>
  static inline void Store(V const &v, S *ptr)
  {
    v.store(ptr);
  }
};

template <uint32_t N>
struct LoadStoreImplementation<UME::SIMD::SIMDVecMask<N>> {
  using M = UME::SIMD::SIMDVecMask<N>;

  template <typename S = Scalar<M>>
  static inline void Load(M &mask, S const *ptr)
  {
    mask.load(ptr);
  }

  template <typename S = Scalar<M>>
  static inline void Store(M const &mask, S *ptr)
  {
    mask.store(ptr);
  }
};

template <typename T, uint32_t N>
struct MaskingImplementation<UME::SIMD::SIMDVec_f<T, N>> {
  using V = UME::SIMD::SIMDVec_f<T, N>;
  using M = UME::SIMD::SIMDVecMask<N>;

  static inline void Assign(V &dst, M const &mask, V const &src) { dst.assign(mask, src); }

  static inline void Blend(V &dst, M const &mask, V const &src1, V const &src2) { dst = src2.blend(mask, src1); }
};

template <typename T, uint32_t N>
struct MaskingImplementation<UME::SIMD::SIMDVec_i<T, N>> {
  using V = UME::SIMD::SIMDVec_i<T, N>;
  using M = UME::SIMD::SIMDVecMask<N>;

  static inline void Assign(V &dst, M const &mask, V const &src) { dst.assign(mask, src); }

  static inline void Blend(V &dst, M const &mask, V const &src1, V const &src2) { dst = src2.blend(mask, src1); }
};

template <typename T, uint32_t N>
struct MaskingImplementation<UME::SIMD::SIMDVec_u<T, N>> {
  using V = UME::SIMD::SIMDVec_u<T, N>;
  using M = UME::SIMD::SIMDVecMask<N>;

  static inline void Assign(V &dst, M const &mask, V const &src) { dst.assign(mask, src); }

  static inline void Blend(V &dst, M const &mask, V const &src1, V const &src2) { dst = src2.blend(mask, src1); }
};

inline namespace math {

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
void SinCos(const UME::SIMD::SIMDVec_f<T, N> &x, UME::SIMD::SIMDVec_f<T, N> *s, UME::SIMD::SIMDVec_f<T, N> *c)
{
  *s = x.sin();
  *c = x.cos();
}

#define UMESIMD_MATH_UNARY_FUNCTION(F, f) \
template <typename T, uint32_t N>         \
VECCORE_FORCE_INLINE                      \
typename UME::SIMD::SIMDVec_f<T, N>       \
F(const UME::SIMD::SIMDVec_f<T, N> &x)    \
{ return x.f(); }

// UMESIMD_MATH_UNARY_FUNCTION(Abs, abs)  // Generic implementation is faster

// UMESIMD_MATH_UNARY_FUNCTION(Sin, sin) // broken
// UMESIMD_MATH_UNARY_FUNCTION(Cos, cos) // broken
UMESIMD_MATH_UNARY_FUNCTION(Tan, tan)
UMESIMD_MATH_UNARY_FUNCTION(ATan, atan)

// UMESIMD_MATH_UNARY_FUNCTION(Exp, exp) // broken
// UMESIMD_MATH_UNARY_FUNCTION(Log, log) // broken
// UMESIMD_MATH_UNARY_FUNCTION(Sqrt, sqrt) // slower than std::sqrt()
// UMESIMD_MATH_UNARY_FUNCTION(Rsqrt, rsqrt) // slower than std::sqrt(1/x)

UMESIMD_MATH_UNARY_FUNCTION(Round, round)
// UMESIMD_MATH_UNARY_FUNCTION(Floor, floor) // slower than std::floor()
// UMESIMD_MATH_UNARY_FUNCTION(Ceil, ceil) // slower than std::ceil()

#undef UMESIMD_MATH_UNARY_FUNCTION

template <typename T, uint32_t N>
VECCORE_FORCE_INLINE
UME::SIMD::SIMDVecMask<N> IsInf(const UME::SIMD::SIMDVec_f<T, N> &x)
{
  return x.isinf();
}

} // end namespace math
} // end namespace vecCore

#endif
