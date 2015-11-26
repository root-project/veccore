#ifndef VECCORE_BACKEND_VC_H
#define VECCORE_BACKEND_VC_H

#include <cstdint>

#include <Vc/Vc>

namespace vecCore {
namespace backend {

template <typename> class Vector;

template <> class Vector<float> {
  public:
    static const bool EarlyExit = true;

    typedef bool                 Bool_t;
    typedef float                Real_t;
    typedef float                Float_t;
    typedef double               Double_t;

    typedef Vc::Vector<Real_t>   Real_v;
    typedef Vc::Vector<Float_t>  Float_v;
    typedef Vc::Vector<Double_t> Double_v;

    typedef int32_t              Int_t;
    typedef int16_t              Int16_t;
    typedef int32_t              Int32_t;
    typedef int64_t              Int64_t;

    typedef Vc::Vector<Int_t>    Int_v;
    typedef Vc::Vector<Int16_t>  Int16_v;
    typedef Vc::Vector<Int32_t>  Int32_v;
    typedef Vc::Vector<Int64_t>  Int64_v;

    typedef uint32_t             UInt_t;
    typedef uint16_t             UInt16_t;
    typedef uint32_t             UInt32_t;
    typedef uint64_t             UInt64_t;

    typedef Vc::Vector<UInt_t>   UInt_v;
    typedef Vc::Vector<UInt16_t> UInt16_v;
    typedef Vc::Vector<UInt32_t> UInt32_v;
    typedef Vc::Vector<UInt64_t> UInt64_v;
};

template <> class Vector<double> {
  public:
    static const bool EarlyExit = true;

    typedef bool                 Bool_t;
    typedef double               Real_t;
    typedef float                Float_t;
    typedef double               Double_t;

    typedef Vc::Vector<Real_t>   Real_v;
    typedef Vc::Vector<Float_t>  Float_v;
    typedef Vc::Vector<Double_t> Double_v;

    typedef int64_t              Int_t;
    typedef int16_t              Int16_t;
    typedef int32_t              Int32_t;
    typedef int64_t              Int64_t;

    typedef Vc::Vector<Int_t>    Int_v;
    typedef Vc::Vector<Int16_t>  Int16_v;
    typedef Vc::Vector<Int32_t>  Int32_v;
    typedef Vc::Vector<Int64_t>  Int64_v;

    typedef uint64_t             UInt_t;
    typedef uint16_t             UInt16_t;
    typedef uint32_t             UInt32_t;
    typedef uint64_t             UInt64_t;

    typedef Vc::Vector<UInt_t>   UInt_v;
    typedef Vc::Vector<UInt16_t> UInt16_v;
    typedef Vc::Vector<UInt32_t> UInt32_v;
    typedef Vc::Vector<UInt64_t> UInt64_v;
};

}
}

#endif
