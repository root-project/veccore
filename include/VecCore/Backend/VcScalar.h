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
  template <typename T>
  using Vector = Vc::Scalar::Vector<T>;

  typedef Vc::Scalar::Vector<Float_t> Float_v;
  typedef Vc::Scalar::Vector<Double_t> Double_v;

  typedef Vc::Scalar::Vector<Int_t> Int_v;
  typedef Vc::Scalar::Vector<Int16_t> Int16_v;
  typedef Vc::Scalar::Vector<Int32_t> Int32_v;
  typedef Vc::Scalar::Vector<Int64_t> Int64_v;

  typedef Vc::Scalar::Vector<UInt_t> UInt_v;
  typedef Vc::Scalar::Vector<UInt16_t> UInt16_v;
  typedef Vc::Scalar::Vector<UInt32_t> UInt32_v;
  typedef Vc::Scalar::Vector<UInt64_t> UInt64_v;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
