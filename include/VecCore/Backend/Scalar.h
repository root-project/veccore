#ifndef VECCORE_BACKEND_SCALAR_H
#define VECCORE_BACKEND_SCALAR_H

namespace vecCore {

template <typename T> struct TypeTraits {
  using ScalarType = T;
  using MaskType   = Bool_s;
  using IndexType  = Size_s;
};

namespace backend {

class Scalar {
public:
  using Real_v   = Real_s;
  using Float_v  = Float_s;
  using Double_v = Double_s;

  using Int_v    = Int_s;
  using Int16_v  = Int16_s;
  using Int32_v  = Int32_s;
  using Int64_v  = Int64_s;

  using UInt_v   = UInt_s;
  using UInt16_v = UInt16_s;
  using UInt32_v = UInt32_s;
  using UInt64_v = UInt64_s;
};

} // namespace backend

template <>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
Bool_s MaskEmpty<Bool_s>(const Bool_s mask)
{
  return !mask;
}

template <>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
Bool_s MaskFull<Bool_s>(const Bool_s mask)
{
  return mask;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void MaskedAssign(T& dest, Bool_s mask, const T &src)
{
  if (mask) dest = src;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Blend(const Bool_s mask, const T& tval, const T& fval)
{
  return mask ? tval : fval;
}

} // namespace vecCore

#endif
