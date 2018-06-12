#ifndef VECCORE_BACKEND_VC_VECTOR_H
#define VECCORE_BACKEND_VC_VECTOR_H

namespace vecCore {

template <typename T>
struct TypeTraits<Vc::Mask<T>> {
  using IndexType  = size_t;
  using ScalarType = Bool_s;
};

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
struct IndexingImplementation<Vc::Mask<T>> {
  using M = Vc::Mask<T>;
  static inline Bool_s Get(const M &mask, size_t i) { return mask[i]; }

  static inline void Set(M &mask, size_t i, const Bool_s val) { mask[i] = val; }
};

template <typename T>
struct LoadStoreImplementation<Vc::Vector<T>> {
   using V = Vc::Vector<T>;
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

template <typename T>
struct LoadStoreImplementation<Vc::Mask<T>> {
  using M = Vc::Mask<T>;

  template <typename S = Scalar<T>>
  static inline void Load(M &mask, Bool_s const *ptr)
  {
    mask.load(ptr);
  }

  template <typename S = Scalar<T>>
  static inline void Store(M const &mask, S *ptr)
  {
    mask.store(ptr);
  }
};

template <typename T>
struct MaskingImplementation<Vc::Vector<T>> {
  using M = Vc::Mask<T>;
  using V = Vc::Vector<T>;

  static inline void Assign(V &dst, M const &mask, V const &src) { dst(mask) = src; }

  static inline void Blend(V &dst, M const &mask, V const &src1, V const src2)
  {
    dst       = src2;
    dst(mask) = src1;
  }
};

namespace math {

template <typename T>
VECCORE_FORCE_INLINE
Vc::Vector<T> CopySign(const Vc::Vector<T> &x, const Vc::Vector<T> &y)
{
  return Vc::copysign(x, y);
}

#define VC_MATH_UNARY_FUNCTION(F, f)                \
template <typename T>                               \
VECCORE_FORCE_INLINE                                \
Vc::Vector<T> F(const Vc::Vector<T> &x)             \
{ return Vc::f(x); }                                \

VC_MATH_UNARY_FUNCTION(Exp, exp)
VC_MATH_UNARY_FUNCTION(Log, log)
VC_MATH_UNARY_FUNCTION(Log2, log2)
VC_MATH_UNARY_FUNCTION(Log10, log10)
VC_MATH_UNARY_FUNCTION(Sqrt, sqrt)

VC_MATH_UNARY_FUNCTION(Sin, sin)
VC_MATH_UNARY_FUNCTION(Cos, cos)
VC_MATH_UNARY_FUNCTION(ASin, asin)
VC_MATH_UNARY_FUNCTION(ATan, atan)

// VC_MATH_UNARY_FUNCTION(Floor, floor) // broken
// VC_MATH_UNARY_FUNCTION(Ceil, ceil)   // broken
// VC_MATH_UNARY_FUNCTION(Trunc, trunc) // broken

#undef VC_MATH_UNARY_FUNCTION

template <typename T>
VECCORE_FORCE_INLINE
Vc::Vector<T> Tan(const Vc::Vector<T> &x)
{
  Vc::Vector<T> s, c;
  Vc::sincos(x, &s, &c);
  return s / c;
}

template <typename T>
VECCORE_FORCE_INLINE
Vc::Mask<T> IsInf(const Vc::Vector<T> &x)
{
  return Vc::isinf(x);
}

}

} // namespace vecCore

#endif
