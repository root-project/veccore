#ifndef VECCORE_BACKEND_SCALAR_H
#define VECCORE_BACKEND_SCALAR_H

#include <cassert>
#include <cstdint>

namespace vecCore {
namespace backend {

template <typename T> struct IntFor;
template <> struct IntFor<float>  { typedef int32_t type; };
template <> struct IntFor<double> { typedef int64_t type; };

template <typename T> struct UIntFor;
template <> struct UIntFor<float>  { typedef uint32_t type; };
template <> struct UIntFor<double> { typedef uint64_t type; };

template <typename FloatT = float, bool EReturns = true>
class Basic {
private:
  // auxiliary classes declared private, so they are not exposed

  class BoolWrapper {
  public:
    BoolWrapper() { /* uninitialized */ }
    BoolWrapper(bool val) : fBool(val) {}

    bool isFull() const { return fBool; }
    bool isEmpty() const { return !fBool; }

    operator bool const &() noexcept { return fBool; }

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

    #define MASK_ASSIGN_OPERATOR(OP)                                           \
    T &operator OP(const T &ref)                                               \
    {                                                                          \
      if (fMask)                                                               \
        fRef OP ref;                                                           \
      return fRef;                                                             \
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
    enum { Size = 1 };

    ScalarWrapper() { /* uninitialized */ }

	/* allow type conversion at initialization */

    ScalarWrapper(int val) : fVal(static_cast<T>(val)) {}
    ScalarWrapper(unsigned int val) : fVal(static_cast<T>(val)) {}
    ScalarWrapper(float val) : fVal(static_cast<T>(val)) {}
    ScalarWrapper(double val) : fVal(static_cast<T>(val)) {}

    operator T() { return fVal; }
    operator T const &() const { return fVal; }

    MaskedScalar<T> operator()(Mask m)
    {
        return MaskedScalar<T>(fVal, m);
    }

    T &operator[](int index)
    {
      assert(index == 0);
      return fVal;
    }

    T const operator[](int index) const
    {
      assert(index == 0);
      return fVal;
    }

    void load(T const *const src) { fVal = *src; }
    void store(T const &val) { val = fVal; }

    #define SCALAR_WRAPPER_OPERATOR(OP)                                        \
    ScalarWrapper operator OP(const ScalarWrapper &x) const                    \
    {                                                                          \
      return ScalarWrapper(fVal OP x.fVal);                                    \
    }                                                                          \

    SCALAR_WRAPPER_OPERATOR(+)
    SCALAR_WRAPPER_OPERATOR(-)
    SCALAR_WRAPPER_OPERATOR(*)
    SCALAR_WRAPPER_OPERATOR(/)
    SCALAR_WRAPPER_OPERATOR(%)

    #undef SCALAR_WRAPPER_OPERATOR

  private:
    T fVal;
  };

public:
  static constexpr bool EarlyReturns = EReturns;

  typedef bool     Bool_t;
  typedef FloatT   Real_t;
  typedef float    Float_t;
  typedef double   Double_t;

  typedef typename IntFor<Real_t>::type  Int_t;
  typedef typename UIntFor<Real_t>::type UInt_t;

  typedef int16_t  Int16_t;
  typedef int32_t  Int32_t;
  typedef int64_t  Int64_t;

  typedef uint16_t UInt16_t;
  typedef uint32_t UInt32_t;
  typedef uint64_t UInt64_t;

  typedef ScalarWrapper<Real_t>   Real_v;
  typedef ScalarWrapper<Float_t>  Float_v;
  typedef ScalarWrapper<Double_t> Double_v;

  typedef ScalarWrapper<Int_t>    Int_v;
  typedef ScalarWrapper<Int16_t>  Int16_v;
  typedef ScalarWrapper<Int32_t>  Int32_v;
  typedef ScalarWrapper<Int64_t>  Int64_v;

  typedef ScalarWrapper<UInt_t>   UInt_v;
  typedef ScalarWrapper<UInt16_t> UInt16_v;
  typedef ScalarWrapper<UInt32_t> UInt32_v;
  typedef ScalarWrapper<UInt64_t> UInt64_v;
};

} // namespace backend
} // namespace vecCore

#endif
