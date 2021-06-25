#ifndef _TEST_H
#define _TEST_H

#undef NDEBUG

#include <VecCore/VecCore>

#include <gtest/gtest.h>
#include <type_traits>

using namespace testing;

template <class Backend>
using IntTypes = Types<typename Backend::Int16_v, typename Backend::Int32_v>;

template <class Backend>
using UIntTypes = Types<typename Backend::UInt16_v, typename Backend::UInt32_v>;

template <class Backend>
using FloatTypes = Types<typename Backend::Float_v, typename Backend::Double_v>;

template <class Backend>
using VectorTypes =
    Types<typename Backend::Float_v, typename Backend::Double_v,
          typename Backend::Int16_v, typename Backend::UInt16_v,
          typename Backend::Int32_v, typename Backend::UInt32_v>;

template <class T> class VectorTypeTest : public Test {
public:
  using Scalar_t = typename vecCore::Scalar<T>;
  using Vector_t = T;
};

#endif
