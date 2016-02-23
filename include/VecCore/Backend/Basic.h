#ifndef VECCORE_BACKEND_BASIC_H
#define VECCORE_BACKEND_BASIC_H

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "ScalarWrapper.h"

namespace vecCore {
namespace backend {

class Basic {
public:
  template <typename T>
  using Vector = details::ScalarWrapper<T>;

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
