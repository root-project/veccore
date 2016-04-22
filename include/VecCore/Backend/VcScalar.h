#ifndef VECCORE_BACKEND_VC_SCALAR_H
#define VECCORE_BACKEND_VC_SCALAR_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {

template <typename T> struct TypeTraits<Vc::Scalar::Vector<T>> {
  using ScalarType = T;
  using MaskType   = typename Vc::Scalar::Vector<T>::MaskType;
  using IndexType  = typename Vc::Scalar::Vector<T>::IndexType;
};

namespace backend {

class VcScalar {
public:
  using Real_v   = Vc::Scalar::Vector<Real_s>;
  using Float_v  = Vc::Scalar::Vector<Float_s>;
  using Double_v = Vc::Scalar::Vector<Double_s>;

  using Int_v    = Vc::Scalar::Vector<Int_s>;
  using Int16_v  = Vc::Scalar::Vector<Int16_s>;
  using Int32_v  = Vc::Scalar::Vector<Int32_s>;
  using Int64_v  = Vc::Scalar::Vector<Int64_s>;

  using UInt_v   = Vc::Scalar::Vector<UInt_s>;
  using UInt16_v = Vc::Scalar::Vector<UInt16_s>;
  using UInt32_v = Vc::Scalar::Vector<UInt32_s>;
  using UInt64_v = Vc::Scalar::Vector<UInt64_s>;
};

} // namespace backend

template <typename T>
VECCORE_FORCE_INLINE
Bool_s MaskEmpty(const Vc::Scalar::Mask<T> mask)
{
  return mask.isEmpty();
}

template <typename T>
VECCORE_FORCE_INLINE
Bool_s MaskFull(const Vc::Scalar::Mask<T> mask)
{
  return mask.isFull();
}

template <typename T>
VECCORE_FORCE_INLINE
void MaskedAssign(Vc::Scalar::Vector<T>& dest,
                  Vc::Scalar::Mask<T> mask,
                  const Vc::Scalar::Vector<T> &src)
{
  dest(mask) = src;
}

template <typename T>
VECCORE_FORCE_INLINE
Vc::Scalar::Vector<T> Blend(const Vc::Scalar::Mask<T> mask,
                            const Vc::Scalar::Vector<T>& tval,
                            const Vc::Scalar::Vector<T>& fval)
{
  typename Vc::Scalar::Vector<T> tmp(tval);
  tmp(mask) = fval;
  return tmp;
}

namespace math {

template <typename T>
VECCORE_FORCE_INLINE
Vc::Scalar::Vector<T> Pow(Vc::Scalar::Vector<T> x, Vc::Scalar::Vector<T> y)
{
  return std::exp(std::log(x[0]) * y[0]);
}

template <typename T>
VECCORE_FORCE_INLINE
Vc::Scalar::Vector<T> Tan(Vc::Scalar::Vector<T> x)
{
  return std::tan(x[0]);
}

}

} // namespace vecCore

#endif
#endif
