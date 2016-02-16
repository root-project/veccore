#ifndef VECCORE_BACKEND_BASIC_H
#define VECCORE_BACKEND_BASIC_H

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "ScalarWrapper.h"

namespace vecCore {
namespace backend {

template <typename T> struct IntFor;
template <> struct IntFor<float> { typedef int32_t Type; };
template <> struct IntFor<double> { typedef int64_t Type; };

template <typename T> struct UIntFor;
template <> struct UIntFor<float> { typedef uint32_t Type; };
template <> struct UIntFor<double> { typedef uint64_t Type; };

template <typename FloatT = float>
class Basic {
public:
  typedef bool Bool_t;
  typedef FloatT Real_t;
  typedef float Float_t;
  typedef double Double_t;

  typedef typename IntFor<Real_t>::Type Int_t;
  typedef typename UIntFor<Real_t>::Type UInt_t;

  typedef int16_t Int16_t;
  typedef int32_t Int32_t;
  typedef int64_t Int64_t;

  typedef uint16_t UInt16_t;
  typedef uint32_t UInt32_t;
  typedef uint64_t UInt64_t;

  typedef details::ScalarWrapper<Real_t> Real_v;
  typedef details::ScalarWrapper<Float_t> Float_v;
  typedef details::ScalarWrapper<Double_t> Double_v;

  typedef details::ScalarWrapper<Int_t> Int_v;
  typedef details::ScalarWrapper<Int16_t> Int16_v;
  typedef details::ScalarWrapper<Int32_t> Int32_v;
  typedef details::ScalarWrapper<Int64_t> Int64_v;

  typedef details::ScalarWrapper<UInt_t> UInt_v;
  typedef details::ScalarWrapper<UInt16_t> UInt16_v;
  typedef details::ScalarWrapper<UInt32_t> UInt32_v;
  typedef details::ScalarWrapper<UInt64_t> UInt64_v;
};

} // namespace backend
} // namespace vecCore

#endif
