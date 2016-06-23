#ifndef VECCORE_BACKEND_VC_VECTOR_H
#define VECCORE_BACKEND_VC_VECTOR_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

#ifndef Vc_IMPL_Scalar

namespace vecCore {

template <typename T>
struct TypeTraits<Vc::Vector<T>> {
  using ScalarType = T;
  using MaskType   = typename Vc::Vector<T>::MaskType;
  using IndexType  = typename Vc::Vector<T>::IndexType;
};

namespace backend {

template <typename T = Real_s>
class VcVectorT {
public:
  using Real_v   = Vc::Vector<T>;
  using Float_v  = Vc::Vector<Float_s>;
  using Double_v = Vc::Vector<Double_s>;

  using Int_v   = Vc::Vector<Int_s>;
  using Int16_v = Vc::Vector<Int16_s>;
  using Int32_v = Vc::Vector<Int32_s>;
  using Int64_v = Vc::Vector<Int64_s>;

  using UInt_v   = Vc::Vector<UInt_s>;
  using UInt16_v = Vc::Vector<UInt16_s>;
  using UInt32_v = Vc::Vector<UInt32_s>;
  using UInt64_v = Vc::Vector<UInt64_s>;
};

using VcVector = VcVectorT<>;

} // namespace backend

template <typename T>
VECCORE_FORCE_INLINE
Bool_s MaskEmpty(const Vc::Mask<T> &mask)
{
  return mask.isEmpty();
}

template <typename T>
VECCORE_FORCE_INLINE
Bool_s MaskFull(const Vc::Mask<T> &mask)
{
  return mask.isFull();
}

template <typename T>
VECCORE_FORCE_INLINE
void MaskedAssign(Vc::Vector<T> &dest, const Vc::Mask<T> &mask, const Vc::Vector<T> &src)
{
  dest(mask) = src;
}

template <typename T>
VECCORE_FORCE_INLINE
Vc::Vector<T> Blend(const Vc::Mask<T> &mask, const Vc::Vector<T> &tval, const Vc::Vector<T> &fval)
{
  typename Vc::Vector<T> tmp(fval);
  tmp(mask) = tval;
  return tmp;
}

namespace math {

template <typename T>
VECCORE_FORCE_INLINE
Vc::Vector<T> Pow(const Vc::Vector<T> &x, const Vc::Vector<T> &y)
{
  return Vc::exp(Vc::log(x) * y);
}

template <typename T>
VECCORE_FORCE_INLINE
Vc::Vector<T> Tan(const Vc::Vector<T> &x)
{
  Vc::Vector<T> s, c;
  Vc::sincos(x, &s, &c);
  return s / c;
}
}

} // namespace vecCore

#endif
#endif
#endif
