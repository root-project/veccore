#ifndef VECCORE_BACKEND_VC_VECTOR_H
#define VECCORE_BACKEND_VC_VECTOR_H

#ifdef VECCORE_ENABLE_VC

#include <Vc/Vc>

namespace vecCore {
namespace backend {

template <typename T>
struct ScalarType<Vc::Vector<T> > { typedef T Type; };

class VcVector {
public:
  using Float_v  = Vc::Vector<Float_t>;
  using Double_v = Vc::Vector<Double_t>;

  using Int_v    = Vc::Vector<Int_t>;
  using Int16_v  = Vc::Vector<Int16_t>;
  using Int32_v  = Vc::Vector<Int32_t>;
  using Int64_v  = Vc::Vector<Int64_t>;

  using UInt_v   = Vc::Vector<UInt_t>;
  using UInt16_v = Vc::Vector<UInt16_t>;
  using UInt32_v = Vc::Vector<UInt32_t>;
  using UInt64_v = Vc::Vector<UInt64_t>;

  template <typename T> using Vector = Vc::Vector<T>;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
