#ifndef VECCORE_BACKEND_SCALAR_WRAPPER_H
#define VECCORE_BACKEND_SCALAR_WRAPPER_H

#include <cassert>
#include <type_traits>

namespace vecCore {

class WrappedBool;
template <typename> class MaskedScalar;
template <typename> class WrappedScalar;

namespace backend {

class ScalarWrapper {
public:
  using Real_v   = WrappedScalar<Real_t>;
  using Float_v  = WrappedScalar<Float_t>;
  using Double_v = WrappedScalar<Double_t>;

  using Int_v    = WrappedScalar<Int_t>;
  using Int16_v  = WrappedScalar<Int16_t>;
  using Int32_v  = WrappedScalar<Int32_t>;
  using Int64_v  = WrappedScalar<Int64_t>;

  using UInt_v   = WrappedScalar<UInt_t>;
  using UInt16_v = WrappedScalar<UInt16_t>;
  using UInt32_v = WrappedScalar<UInt32_t>;
  using UInt64_v = WrappedScalar<UInt64_t>;

  template <typename T> using Mask_v  = WrappedBool;
  template <typename T> using Index_v = WrappedScalar<Int_t>;

};

} // namespace backend

class WrappedBool {
public:
  static constexpr size_t Size = 1;

  WrappedBool() { /* uninitialized */ }
  WrappedBool(bool val) : fBool(val) {}

  bool isFull() const { return fBool; }
  bool isEmpty() const { return !fBool; }

  static constexpr size_t size() { return 1; }

  operator bool &() noexcept { return fBool; }
  operator bool const &() const noexcept { return fBool; }

  bool &operator[](int index) {
    assert(index == 0);
    return fBool;
  }

  bool operator[](int index) const {
    assert(index == 0);
    return fBool;
  }

private:
  bool fBool;
};

template <class T> class MaskedScalar {
public:
  using Mask = WrappedBool;

  MaskedScalar() = delete;
  MaskedScalar(T &ref, Mask mask = true)
    : fRef(ref), fMask(mask) {}

#define MASK_ASSIGN_OPERATOR(OP)                                               \
  T &operator OP(const T &ref) {                                               \
    if (fMask)                                                                 \
      fRef OP ref;                                                             \
    return fRef;                                                               \
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

template <class T> class WrappedScalar {
public:
  using Type = T;
  using Mask = WrappedBool;

  static constexpr size_t Size = 1;

  WrappedScalar() { /* uninitialized */ }

  WrappedScalar(const T& val) : fVal(val) {}
  WrappedScalar(const T* const val_ptr) : fVal(*val_ptr) {}
  WrappedScalar(const WrappedScalar* const s) : fVal(s->val_ptr) {}

  /* allow type conversion from other scalar types at initialization */
  template <typename Type,
  class = typename std::enable_if<std::is_integral<Type>::value>::type>
  WrappedScalar(const Type& val) : fVal(static_cast<T>(val)) {}

  static constexpr size_t size() { return 1; }

  operator T &() noexcept { return fVal; }
  operator T const &() const noexcept { return fVal; }

  MaskedScalar<T> operator()(Mask m) { return MaskedScalar<T>(fVal, m); }

  T &operator[](int index) {
    assert(index == 0);
    return fVal;
  }

  T const operator[](int index) const {
    assert(index == 0);
    return fVal;
  }

  void load(T const *const src) { fVal = *src; }
  void store(T const &val) { val = fVal; }

#define SCALAR_WRAPPER_OPERATOR(OP)                                            \
  VECCORE_FORCE_INLINE                                                         \
  WrappedScalar operator OP(const WrappedScalar &x) const {                    \
    return WrappedScalar(fVal OP x.fVal);                                      \
  }                                                                            \
                                                                               \
  VECCORE_FORCE_INLINE                                                         \
  WrappedScalar operator OP(const T &x) const {                                \
    return WrappedScalar(fVal OP x);                                           \
  }

  SCALAR_WRAPPER_OPERATOR(+)
  SCALAR_WRAPPER_OPERATOR(-)
  SCALAR_WRAPPER_OPERATOR(*)
  SCALAR_WRAPPER_OPERATOR(/)
  SCALAR_WRAPPER_OPERATOR(%)

#undef SCALAR_WRAPPER_OPERATOR

private:
  T fVal;
};

template <typename T>
struct ScalarType<WrappedScalar<T>> { using Type = T; };

template <>
VECCORE_FORCE_INLINE
Bool_t IsEmpty<WrappedBool>(const WrappedBool mask)
{
  return !mask;
}

template <>
VECCORE_FORCE_INLINE
Bool_t MaskFull<WrappedBool>(const WrappedBool mask)
{
  return mask;
}

template <typename T>
VECCORE_FORCE_INLINE
void MaskedAssign(WrappedScalar<T>& dest,
                WrappedBool mask,
                const WrappedScalar<T> &src)
{
  if (mask) dest = src;
}

template <typename T>
VECCORE_FORCE_INLINE
WrappedScalar<T> Blend(const WrappedBool mask,
                       const WrappedScalar<T>& tval,
                       const WrappedScalar<T>& fval)
{
  return mask ? tval : fval;
}

} // namespace vecCore

#endif
