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
  template <typename T>
  using Vector = Vc::Vector<T>;

  typedef Vc::Vector<Float_t> Float_v;
  typedef Vc::Vector<Double_t> Double_v;

  typedef Vc::Vector<Int_t> Int_v;
  typedef Vc::Vector<Int16_t> Int16_v;
  typedef Vc::Vector<Int32_t> Int32_v;
  typedef Vc::Vector<Int64_t> Int64_v;

  typedef Vc::Vector<UInt_t> UInt_v;
  typedef Vc::Vector<UInt16_t> UInt16_v;
  typedef Vc::Vector<UInt32_t> UInt32_v;
  typedef Vc::Vector<UInt64_t> UInt64_v;
};

} // namespace backend
} // namespace vecCore

#endif
#endif
