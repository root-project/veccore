#ifndef VECCORE_BACKEND_SCALAR_H
#define VECCORE_BACKEND_SCALAR_H

namespace vecCore {
namespace backend {

class Scalar {
public:
  using Real_v   = Real_t;
  using Float_v  = Float_t;
  using Double_v = Double_t;

  using Int_v    = Int_t;
  using Int16_v  = Int16_t;
  using Int32_v  = Int32_t;
  using Int64_v  = Int64_t;

  using UInt_v   = UInt_t;
  using UInt16_v = UInt16_t;
  using UInt32_v = UInt32_t;
  using UInt64_v = UInt64_t;

  template <typename T> using Mask_v  = Bool_t;
  template <typename T> using Index_v = UInt64_t;

  template <typename T>
  static constexpr UInt64_t VectorSize(const T& x)
  {
    return 1UL;
  }
};

} // namespace backend
} // namespace vecCore

#endif
