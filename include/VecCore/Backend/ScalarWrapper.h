#ifndef VECCORE_SCALAR_WRAPPER_H
#define VECCORE_SCALAR_WRAPPER_H

namespace vecCore {
namespace backend {

namespace details {

class BoolWrapper {
public:
  static constexpr size_t Size = 1;

  BoolWrapper() { /* uninitialized */ }
  BoolWrapper(bool val) : fBool(val) {}

  bool isFull() const { return fBool; }
  bool isEmpty() const { return !fBool; }

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
  typedef BoolWrapper Mask;
  MaskedScalar() = delete;
  MaskedScalar(T &ref, Mask mask = true) : fRef(ref), fMask(mask) {}

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

template <class T> class ScalarWrapper {
public:
  typedef T type;
  typedef BoolWrapper Mask;
  static constexpr size_t Size = 1;

  ScalarWrapper() { /* uninitialized */ }

  ScalarWrapper(const T& val) : fVal(val) {}
  ScalarWrapper(const T* const val_ptr) : fVal(*val_ptr) {}
  ScalarWrapper(const ScalarWrapper* const s) : fVal(s->val_ptr) {}

  /* allow type conversion from other scalar types at initialization */
  template <typename Type,
  class = typename std::enable_if<std::is_integral<Type>::value>::type>
  ScalarWrapper(const Type& val) : fVal(static_cast<T>(val)) {}

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
  ScalarWrapper operator OP(const ScalarWrapper &x) const {                    \
    return ScalarWrapper(fVal OP x.fVal);                                      \
  }                                                                            \
                                                                               \
  ScalarWrapper operator OP(const T &x) const {                                \
    return ScalarWrapper(fVal OP x);                                           \
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

} // namespace details
} // namespace backend
} // namespace vecCore

#endif
