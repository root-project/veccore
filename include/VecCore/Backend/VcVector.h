#ifndef VECCORE_BACKEND_VC_VECTOR_H
#define VECCORE_BACKEND_VC_VECTOR_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {

template <typename T> struct TypeTraits<Vc::Vector<T>> {
  using ScalarType = T;
  using MaskType   = typename Vc::Vector<T>::MaskType;
  using IndexType  = typename Vc::Vector<T>::IndexType;
};

namespace backend {

class VcVector {
public:
  using Real_v   = Vc::Vector<Real_t>;
  using Float_v  = Vc::Vector<Float_t>;
  using Double_v = Vc::Vector<Double_t>;

  using Int_v    = Vc::Vector<Int_t>;
  using Int16_v  = Vc::Vector<Int16_t>;
  using Int32_v  = Vc::Vector<Int32_t>;
  using Int64_v  = Vc::Vector<Int64_t>;

  using UInt_v   = Vc::Vector<UInt_t>;
  using UInt16_v = Vc::Vector<UInt16_t>;
  using UInt32_v = Vc::Vector<UInt32_t>;
  using UInt64_v = Vc::Vector<UInt64_t>;
};

} // namespace backend

template <typename T>
VECCORE_FORCE_INLINE
Bool_t MaskEmpty(const Vc::Mask<T> mask)
{
  return mask.isEmpty();
}

template <typename T>
VECCORE_FORCE_INLINE
Bool_t MaskFull(const Vc::Mask<T> mask)
{
  return mask.isFull();
}

template <typename T>
VECCORE_FORCE_INLINE
void MaskedAssign(Vc::Vector<T>& dest, Vc::Mask<T> mask, const Vc::Vector<T> &src)
{
  dest(mask) = src;
}

template <typename T>
VECCORE_FORCE_INLINE
Vc::Vector<T> Blend(const Vc::Mask<T> mask, const Vc::Vector<T>& tval, const Vc::Vector<T>& fval)
{
  typename Vc::Vector<T> tmp(fval);
  tmp(mask) = tval;
  return tmp;
}

} // namespace vecCore

#endif
#endif
