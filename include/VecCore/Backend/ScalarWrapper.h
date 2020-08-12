#ifndef VECCORE_BACKEND_SCALAR_WRAPPER_H
#define VECCORE_BACKEND_SCALAR_WRAPPER_H

#include <type_traits>

namespace vecCore {

class WrappedBool;
template <typename>
class MaskedScalar;
template <typename>
class WrappedScalar;

template <>
struct TypeTraits<WrappedBool> {
  using ScalarType = bool;
  using IndexType  = WrappedScalar<size_t>;
};

template <typename T>
struct TypeTraits<WrappedScalar<T>> {
  using ScalarType = T;
  using MaskType   = WrappedBool;
  using IndexType  = WrappedScalar<size_t>;
};

namespace backend {

template <typename T = Real_s>
class ScalarWrapperT {
public:
  using Real_v   = WrappedScalar<T>;
  using Float_v  = WrappedScalar<float>;
  using Double_v = WrappedScalar<double>;

  using Int_v   = WrappedScalar<int>;
  using Int16_v = WrappedScalar<int16_t>;
  using Int32_v = WrappedScalar<int32_t>;
  using Int64_v = WrappedScalar<int64_t>;

  using UInt_v   = WrappedScalar<unsigned int>;
  using UInt16_v = WrappedScalar<uint16_t>;
  using UInt32_v = WrappedScalar<uint32_t>;
  using UInt64_v = WrappedScalar<uint64_t>;
};

using ScalarWrapper = ScalarWrapperT<>;

} // namespace backend

class WrappedBool {
public:
  static constexpr size_t Size = 1;

  VECCORE_ATT_HOST_DEVICE
  WrappedBool() { /* uninitialized */}

  VECCORE_ATT_HOST_DEVICE
  WrappedBool(bool val) : fBool(val) {}

  VECCORE_ATT_HOST_DEVICE
  bool isFull() const { return fBool; }

  VECCORE_ATT_HOST_DEVICE
  bool isEmpty() const { return !fBool; }

  VECCORE_ATT_HOST_DEVICE
  static constexpr size_t size() { return 1; }

  VECCORE_ATT_HOST_DEVICE
  operator bool &() noexcept { return fBool; }

  VECCORE_ATT_HOST_DEVICE
  operator bool const &() const noexcept { return fBool; }

  VECCORE_ATT_HOST_DEVICE
  bool &operator[](int index)
  {
    assert(index == 0);
    (void)index;
    return fBool;
  }

  VECCORE_ATT_HOST_DEVICE
  bool operator[](int index) const
  {
    assert(index == 0);
    (void)index;
    return fBool;
  }

  VECCORE_ATT_HOST_DEVICE
  void store(bool *dest) const { *dest = fBool; }

private:
  bool fBool{};
};

template <class T>
class MaskedScalar {
public:
  using Mask = WrappedBool;

  VECCORE_ATT_HOST_DEVICE
  MaskedScalar() = delete;

  VECCORE_ATT_HOST_DEVICE
  MaskedScalar(T &ref, Mask mask = true) : fRef(ref), fMask(mask) {}

#define MASK_ASSIGN_OPERATOR(OP) \
  VECCORE_ATT_HOST_DEVICE        \
  T &operator OP(const T &ref)   \
  {                              \
    if (fMask) fRef OP ref;      \
    return fRef;                 \
  }

  MASK_ASSIGN_OPERATOR(=)
  MASK_ASSIGN_OPERATOR(+=)
  MASK_ASSIGN_OPERATOR(-=)
  MASK_ASSIGN_OPERATOR(*=)
  MASK_ASSIGN_OPERATOR(/=)
  MASK_ASSIGN_OPERATOR(%=)
  MASK_ASSIGN_OPERATOR(&=)
  MASK_ASSIGN_OPERATOR(^=)
  MASK_ASSIGN_OPERATOR(|=)
  MASK_ASSIGN_OPERATOR(<<=)
  MASK_ASSIGN_OPERATOR(>>=)

#undef MASK_ASSIGN_OPERATOR

private:
  T &fRef;
  Mask fMask;
};

template <class T>
class WrappedScalar {
public:
  using Type = T;
  using Mask = WrappedBool;

  static constexpr size_t Size = 1;

  VECCORE_ATT_HOST_DEVICE
  WrappedScalar() { /* uninitialized */}

  VECCORE_ATT_HOST_DEVICE
  WrappedScalar(const T &val) : fVal(val) {}

  VECCORE_ATT_HOST_DEVICE
  WrappedScalar(const T *const val_ptr) : fVal(*val_ptr) {}

  VECCORE_ATT_HOST_DEVICE
  WrappedScalar(const WrappedScalar *const s) : fVal(s->val_ptr) {}

  /* allow type conversion from other scalar types at initialization */
  template <typename Type, class = typename std::enable_if<std::is_integral<Type>::value>::type>
  VECCORE_ATT_HOST_DEVICE
  WrappedScalar(const Type &val) : fVal(static_cast<T>(val))
  {
  }

  VECCORE_ATT_HOST_DEVICE
  static constexpr size_t size() { return 1; }

  VECCORE_ATT_HOST_DEVICE
  operator T &() noexcept { return fVal; }

  VECCORE_ATT_HOST_DEVICE
  operator T const &() const noexcept { return fVal; }

  VECCORE_ATT_HOST_DEVICE
  MaskedScalar<T> operator()(Mask m) { return MaskedScalar<T>(fVal, m); }

  VECCORE_ATT_HOST_DEVICE
  T &operator[](int index)
  {
    assert(index == 0);
    (void)index;
    return fVal;
  }

  VECCORE_ATT_HOST_DEVICE
  T const operator[](int index) const
  {
    assert(index == 0);
    (void)index;
    return fVal;
  }

  VECCORE_ATT_HOST_DEVICE
  void load(T const *const src) { fVal = *src; }

  VECCORE_ATT_HOST_DEVICE
  void store(T &dest) const { dest = fVal; }

  VECCORE_ATT_HOST_DEVICE
  void store(T *dest) const { *dest = fVal; }

#define SCALAR_WRAPPER_OPERATOR(OP)                                                                 \
  VECCORE_FORCE_INLINE                                                                              \
  VECCORE_ATT_HOST_DEVICE                                                                           \
  WrappedScalar operator OP(const WrappedScalar &x) const { return WrappedScalar(fVal OP x.fVal); } \
                                                                                                    \
  VECCORE_FORCE_INLINE                                                                              \
  VECCORE_ATT_HOST_DEVICE                                                                           \
  WrappedScalar operator OP(const T &x) const { return WrappedScalar(fVal OP x); }

  SCALAR_WRAPPER_OPERATOR(+)
  SCALAR_WRAPPER_OPERATOR(-)
  SCALAR_WRAPPER_OPERATOR(*)
  SCALAR_WRAPPER_OPERATOR(/)
  SCALAR_WRAPPER_OPERATOR(%)

#undef SCALAR_WRAPPER_OPERATOR

private:
  T fVal;
};

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
bool MaskEmpty<WrappedBool>(const WrappedBool &mask)
{
  return !mask;
}

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
bool MaskFull<WrappedBool>(const WrappedBool &mask)
{
  return mask;
}

template <typename T>
struct MaskingImplementation<WrappedScalar<T>> {
  VECCORE_FORCE_INLINE
  VECCORE_ATT_HOST_DEVICE
  static void Assign(WrappedScalar<T> &dst, WrappedBool const &mask, WrappedScalar<T> const &src)
  {
    if (mask) dst = src;
  }

  VECCORE_FORCE_INLINE
  VECCORE_ATT_HOST_DEVICE
  static void Blend(WrappedScalar<T> &dst, WrappedBool const &mask, WrappedScalar<T> const &src1,
                    WrappedScalar<T> const &src2)
  {
    dst = mask ? src1 : src2;
  }
};

} // namespace vecCore

#endif
