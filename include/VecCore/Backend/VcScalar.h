#ifndef VECCORE_BACKEND_VC_SCALAR_H
#define VECCORE_BACKEND_VC_SCALAR_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {

template <typename T>
struct ScalarType<Vc::Scalar::Vector<T>> { using Type = T; };

namespace backend {

class VcScalar {
public:
  using Real_v   = Vc::Scalar::Vector<Real_t>;
  using Float_v  = Vc::Scalar::Vector<Float_t>;
  using Double_v = Vc::Scalar::Vector<Double_t>;

  using Int_v    = Vc::Scalar::Vector<Int_t>;
  using Int16_v  = Vc::Scalar::Vector<Int16_t>;
  using Int32_v  = Vc::Scalar::Vector<Int32_t>;
  using Int64_v  = Vc::Scalar::Vector<Int64_t>;

  using UInt_v   = Vc::Scalar::Vector<UInt_t>;
  using UInt16_v = Vc::Scalar::Vector<UInt16_t>;
  using UInt32_v = Vc::Scalar::Vector<UInt32_t>;
  using UInt64_v = Vc::Scalar::Vector<UInt64_t>;

  template <typename T>
  using Mask_v = typename Vc::Scalar::Vector<typename ScalarType<T>::Type>::MaskType;

  template <typename T>
  using Index_v = typename Vc::Scalar::Vector<typename ScalarType<T>::Type>::IndexType;

  template <typename T>
  static constexpr UInt64_t VectorSize(const T& x)
  {
    return Vc::Scalar::Vector<typename ScalarType<T>::Type>::Size;
  }
};

} // namespace backend

template <typename T>
VECCORE_FORCE_INLINE
Bool_t IsEmpty(const Vc::Scalar::Mask<T> mask)
{
  return mask.isEmpty();
}

template <typename T>
VECCORE_FORCE_INLINE
Bool_t IsFull(const Vc::Scalar::Mask<T> mask)
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

} // namespace vecCore

#endif
#endif
