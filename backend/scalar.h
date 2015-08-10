#ifndef VECCORE_SCALAR_BACKEND_H
#define VECCORE_SCALAR_BACKEND_H

#include <cassert>

#include <cmath>
#include <cstdint>

#ifdef __APPLE__
#define sincos(x) __sincos(x)
#define sincosf(x) __sincosf(x)
#endif

namespace VecCore {
namespace Backend {

template <typename FloatT = double, bool EExit = true> class Scalar {
private:
  // auxiliary classes declared private, so they are not exposed

  class BoolWrapper {
  public:
    BoolWrapper() { /* uninitialized */
    }
    BoolWrapper(bool val) : fBool(val) {}

    bool isFull() { return fBool; }
    bool isEmpty() { return !fBool; }

    operator bool &() noexcept { return fBool; }

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
                                                                               \
    return fRef;                                                               \
  }

    MASK_ASSIGN_OPERATOR(= )
    MASK_ASSIGN_OPERATOR(+= )
    MASK_ASSIGN_OPERATOR(-= )
    MASK_ASSIGN_OPERATOR(*= )
    MASK_ASSIGN_OPERATOR(/= )
    MASK_ASSIGN_OPERATOR(%= )
    MASK_ASSIGN_OPERATOR(&= )
    MASK_ASSIGN_OPERATOR(^= )
    MASK_ASSIGN_OPERATOR(|= )
    MASK_ASSIGN_OPERATOR(<<= )
    MASK_ASSIGN_OPERATOR(>>= )

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
    ScalarWrapper(T val) : fVal(val) {}
    ScalarWrapper(T &ref) : fVal(ref) {}

    operator T &()       { return fVal; }
    operator T &() const { return fVal; }

    // mimic Vc library interface

    MaskedScalar<T> operator()(Mask m) { return MaskedScalar<T>(fVal, m); }

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

#define SCALAR_WRAPPER_OPERATOR(OP)                                            \
  ScalarWrapper operator OP(const ScalarWrapper &x) const                      \
  {                                                                            \
    return ScalarWrapper(fVal OP x.fVal);                                      \
  }                                                                            \

    SCALAR_WRAPPER_OPERATOR(+)
    SCALAR_WRAPPER_OPERATOR(-)
    SCALAR_WRAPPER_OPERATOR(*)
    SCALAR_WRAPPER_OPERATOR(/ )
    SCALAR_WRAPPER_OPERATOR(% )

#undef SCALAR_WRAPPER_OPERATOR

  private:
    T fVal;
  };

public:
  static const bool EarlyExit = EExit;

  // floating point types

  typedef FloatT Real_t;
  typedef float_t Float_t;
  typedef double_t Double_t;

  typedef ScalarWrapper<Real_t> Real_v;
  typedef ScalarWrapper<Float_t> Float_v;
  typedef ScalarWrapper<Double_t> Double_v;

  // integer types

  typedef int Int_t;
  typedef int16_t Int16_t;
  typedef int32_t Int32_t;
  typedef int64_t Int64_t;

  typedef ScalarWrapper<Int_t> Int_v;
  typedef ScalarWrapper<Int16_t> Int16_v;
  typedef ScalarWrapper<Int32_t> Int32_v;
  typedef ScalarWrapper<Int64_t> Int64_v;

  typedef unsigned int UInt_t;
  typedef uint16_t UInt16_t;
  typedef uint32_t UInt32_t;
  typedef uint64_t UInt64_t;

  typedef ScalarWrapper<UInt_t> UInt_v;
  typedef ScalarWrapper<UInt16_t> UInt16_v;
  typedef ScalarWrapper<UInt32_t> UInt32_v;
  typedef ScalarWrapper<UInt64_t> UInt64_v;
};
}
}

#endif
