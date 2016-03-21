#ifndef VECCORE_BACKEND_VC_SIMDARRAY_H
#define VECCORE_BACKEND_VC_SIMDARRAY_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {

template <typename T, size_t N> struct TypeTraits<Vc::SimdArray<T,N>> {
  using ScalarType = T;
  using MaskType   = typename Vc::SimdArray<T,N>::MaskType;
  using IndexType  = typename Vc::SimdArray<T,N>::IndexType;
};

namespace backend {

template <size_t N = 16>
class VcSimdArray {
public:
  using Real_v   = Vc::SimdArray<Real_t, N>;
  using Float_v  = Vc::SimdArray<Float_t, N>;
  using Double_v = Vc::SimdArray<Double_t, N>;

  using Int_v    = Vc::SimdArray<Int_t, N>;
  using Int16_v  = Vc::SimdArray<Int16_t, N>;
  using Int32_v  = Vc::SimdArray<Int32_t, N>;
  using Int64_v  = Vc::SimdArray<Int64_t, N>;

  using UInt_v   = Vc::SimdArray<UInt_t, N>;
  using UInt16_v = Vc::SimdArray<UInt16_t, N>;
  using UInt32_v = Vc::SimdArray<UInt32_t, N>;
  using UInt64_v = Vc::SimdArray<UInt64_t, N>;
};

} // namespace backend

template <typename T, size_t N>
VECCORE_FORCE_INLINE
Bool_t MaskEmpty(const Vc::SimdMaskArray<T, N> mask)
{
  return mask.isEmpty();
}

template <typename T, size_t N>
VECCORE_FORCE_INLINE
Bool_t MaskFull(const Vc::SimdMaskArray<T, N> mask)
{
  return mask.isFull();
}

template <typename T, size_t N>
VECCORE_FORCE_INLINE
void MaskedAssign(Vc::SimdArray<T, N>& dest,
                  Vc::SimdMaskArray<T, N> mask,
                  const Vc::SimdArray<T, N> &src)
{
  dest(mask) = src;
}

template <typename T, size_t N>
VECCORE_FORCE_INLINE
Vc::SimdArray<T, N> Blend(const Vc::SimdMaskArray<T, N> mask,
                          const Vc::SimdArray<T, N>& tval,
                          const Vc::SimdArray<T, N>& fval)
{
  typename Vc::SimdArray<T, N> tmp(fval);
  tmp(mask) = tval;
  return tmp;
}

} // namespace vecCore

#endif
#endif
