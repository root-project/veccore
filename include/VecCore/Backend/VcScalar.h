#ifndef VECCORE_BACKEND_VC_SCALAR_H
#define VECCORE_BACKEND_VC_SCALAR_H

namespace vecCore {

template <typename T>
struct TypeTraits<Vc::Scalar::Mask<T>> {
  using IndexType  = size_t;
  using ScalarType = bool;
};

template <typename T>
struct TypeTraits<Vc::Scalar::Vector<T>> {
  using ScalarType = T;
  using MaskType   = typename Vc::Scalar::Vector<T>::MaskType;
  using IndexType  = typename Vc::Scalar::Vector<T>::IndexType;
};

namespace backend {

template <typename T = Real_s>
class VcScalarT {
public:
  using Real_v   = Vc::Scalar::Vector<T>;
  using Float_v  = Vc::Scalar::Vector<float>;
  using Double_v = Vc::Scalar::Vector<double>;

  using Int_v   = Vc::Scalar::Vector<int>;
  using Int16_v = Vc::Scalar::Vector<int16_t>;
  using Int32_v = Vc::Scalar::Vector<int32_t>;
  using Int64_v = Vc::Scalar::Vector<int64_t>;

  using UInt_v   = Vc::Scalar::Vector<unsigned int>;
  using UInt16_v = Vc::Scalar::Vector<uint16_t>;
  using UInt32_v = Vc::Scalar::Vector<uint32_t>;
  using UInt64_v = Vc::Scalar::Vector<uint64_t>;
};

using VcScalar = VcScalarT<>;

} // namespace backend

template <typename T>
VECCORE_FORCE_INLINE
bool MaskEmpty(const Vc::Scalar::Mask<T> &mask)
{
  return mask.isEmpty();
}

template <typename T>
VECCORE_FORCE_INLINE
bool MaskFull(const Vc::Scalar::Mask<T> &mask)
{
  return mask.isFull();
}

template <typename T>
struct IndexingImplementation<Vc::Scalar::Mask<T>> {
  using M = Vc::Scalar::Mask<T>;
  static inline bool Get(const M &mask, size_t i) { return mask[i]; }

  static inline void Set(M &mask, size_t i, const bool val) { mask[i] = val; }
};

template <typename T>
struct MaskingImplementation<Vc::Scalar::Vector<T>> {
  using M = Vc::Scalar::Mask<T>;
  using V = Vc::Scalar::Vector<T>;

  static inline void Assign(V &dst, M const &mask, V const &src) { dst(mask) = src; }

  static inline void Blend(V &dst, M const &mask, V const &src1, V const src2)
  {
    dst       = src2;
    dst(mask) = src1;
  }
};

inline namespace math {

template <typename T>
VECCORE_FORCE_INLINE
Vc::Scalar::Mask<T> IsInf(const Vc::Scalar::Vector<T> &x)
{
  return Vc::isinf(x);
}

}

} // namespace vecCore

#endif
