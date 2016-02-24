#ifndef VECCORE_BACKEND_VC_SCALAR_H
#define VECCORE_BACKEND_VC_SCALAR_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {
namespace backend {

template <typename T>
struct ScalarType<Vc::Scalar::Vector<T> > { typedef T Type; };

class VcScalar {
public:
  using Float_v  = Vc::Scalar::Vector<Float_t>;
  using Double_v = Vc::Scalar::Vector<Double_t>;

  using Int_v    = Vc::Scalar::Vector<Int_t>;
  using Int16_v  = Vc::Scalar::Vector<Int16_t>;
  using Int32_v  = Vc::Scalar::Vector<Int32_t>;
  using Int64_v  = Vc::Scalar::Vector<Int64_t>;

  using UInt_v   = Vc::Scalar::Vector<UInt_t>;
  using UInt16_v = Vc::Scalar::Vector<UInt16_t>;
  using UInt32_v = Vc::Scalar::Vector<UInt32_t>;
  using UInt64_v = Vc::Scalar::Vector<UInt64_t>;

  template <typename T> using Vector = Vc::Scalar::Vector<T>;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
