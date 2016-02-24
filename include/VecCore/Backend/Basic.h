#ifndef VECCORE_BACKEND_BASIC_H
#define VECCORE_BACKEND_BASIC_H

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "ScalarWrapper.h"

namespace vecCore {

template <>
VECCORE_FORCE_INLINE
Bool_t IsEmpty<Bool_t>(const Bool_t mask)
{
  return !mask;
}

template <>
VECCORE_FORCE_INLINE
Bool_t IsFull<Bool_t>(const Bool_t mask)
{
  return mask;
}

template <typename T, typename = std::enable_if<std::is_scalar<T>::value>>
VECCORE_FORCE_INLINE
void MaskAssign(T& dest, Bool_t mask, const T &src)
{
  if (mask) dest = src;
}

template <typename T, typename = std::enable_if<std::is_scalar<T>::value>>
VECCORE_FORCE_INLINE
T Blend(const Bool_t mask, const T& tval, const T& fval)
{
  return mask ? tval : fval;
}

namespace backend {

class Basic {
public:
  using Float_v  = ScalarWrapper<Float_t>;
  using Double_v = ScalarWrapper<Double_t>;

  using Int_v    = ScalarWrapper<Int_t>;
  using Int16_v  = ScalarWrapper<Int16_t>;
  using Int32_v  = ScalarWrapper<Int32_t>;
  using Int64_v  = ScalarWrapper<Int64_t>;

  using UInt_v   = ScalarWrapper<UInt_t>;
  using UInt16_v = ScalarWrapper<UInt16_t>;
  using UInt32_v = ScalarWrapper<UInt32_t>;
  using UInt64_v = ScalarWrapper<UInt64_t>;

  template <typename T> using Vector = ScalarWrapper<T>;
};

} // namespace backend
} // namespace vecCore

#endif
