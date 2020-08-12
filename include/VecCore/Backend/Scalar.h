#ifndef VECCORE_BACKEND_SCALAR_H
#define VECCORE_BACKEND_SCALAR_H

#include "Interface.h"
#include "Implementation.h"

namespace vecCore {

template <typename T>
struct TypeTraits {
  using ScalarType = T;
  using MaskType   = bool;
  using IndexType  = size_t;
};

namespace backend {

template <typename T = Real_s>
class ScalarT {
public:
  using Real_v   = T;
  using Float_v  = float;
  using Double_v = double;

  using Int_v   = int;
  using Int16_v = int16_t;
  using Int32_v = int32_t;
  using Int64_v = int64_t;

  using UInt_v   = unsigned int;
  using UInt16_v = uint16_t;
  using UInt32_v = uint32_t;
  using UInt64_v = uint64_t;
};

using Scalar = ScalarT<>;

} // namespace backend

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
bool MaskEmpty<bool>(const bool &mask)
{
  return !mask;
}

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
bool MaskFull<bool>(const bool &mask)
{
  return mask;
}

} // namespace vecCore

#endif
