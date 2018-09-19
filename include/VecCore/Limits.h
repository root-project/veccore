#ifndef VECCORE_LIMITS_H
#define VECCORE_LIMITS_H

#include "Backend/Interface.h"

#include <cfloat>
#include <climits>
#include <cmath>

namespace vecCore {

template <typename T>
struct NumericLimits {
  using ScalarT = Scalar<T>;

  static constexpr T Min() noexcept { return T(NumericLimits<ScalarT>::Min()); }
  static constexpr T Max() noexcept { return T(NumericLimits<ScalarT>::Max()); }

  static constexpr T Lowest() noexcept { return T(NumericLimits<ScalarT>::Lowest()); }
  static constexpr T Highest() noexcept { return T(NumericLimits<ScalarT>::Highest()); }

  static constexpr T Epsilon() noexcept { return T(NumericLimits<ScalarT>::Epsilon()); }
  static constexpr T Infinity() noexcept { return T(NumericLimits<ScalarT>::Infinity()); }
};

template <>
struct NumericLimits<bool> {
  VECCORE_ATT_HOST_DEVICE static constexpr bool Min() noexcept { return false; }
  VECCORE_ATT_HOST_DEVICE static constexpr bool Max() noexcept { return true; }
};

template <>
struct NumericLimits<char> {
  VECCORE_ATT_HOST_DEVICE static constexpr char Min() noexcept { return CHAR_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr char Max() noexcept { return CHAR_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr char Lowest() noexcept { return CHAR_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr char Highest() noexcept { return CHAR_MAX; }
};

template <>
struct NumericLimits<unsigned char> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Min() noexcept { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Max() noexcept { return UCHAR_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Lowest() noexcept { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Highest() noexcept { return UCHAR_MAX; }
};

template <>
struct NumericLimits<short> {
  VECCORE_ATT_HOST_DEVICE static constexpr short Min() noexcept { return SHRT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr short Max() noexcept { return SHRT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr short Lowest() noexcept { return SHRT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr short Highest() noexcept { return SHRT_MAX; }
};

template <>
struct NumericLimits<unsigned short> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Min() noexcept { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Max() noexcept { return USHRT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Lowest() noexcept { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Highest() noexcept { return USHRT_MAX; }
};

template <>
struct NumericLimits<int> {
  VECCORE_ATT_HOST_DEVICE static constexpr int Min() noexcept { return INT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr int Max() noexcept { return INT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr int Lowest() noexcept { return INT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr int Highest() noexcept { return INT_MAX; }
};

template <>
struct NumericLimits<unsigned int> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Min() noexcept { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Max() noexcept { return UINT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Lowest() noexcept { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Highest() noexcept { return UINT_MAX; }
};

template <>
struct NumericLimits<long> {
  VECCORE_ATT_HOST_DEVICE static constexpr long Min() noexcept { return LONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long Max() noexcept { return LONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr long Lowest() noexcept { return LONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long Highest() noexcept { return LONG_MAX; }
};

template <>
struct NumericLimits<unsigned long> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Min() noexcept { return 0UL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Max() noexcept { return ULONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Lowest() noexcept { return 0UL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Highest() noexcept { return ULONG_MAX; }
};

template <>
struct NumericLimits<long long> {
  VECCORE_ATT_HOST_DEVICE static constexpr long long Min() noexcept { return LLONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long long Max() noexcept { return LLONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr long long Lowest() noexcept { return LLONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long long Highest() noexcept { return LLONG_MAX; }
};

template <>
struct NumericLimits<unsigned long long> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Min() noexcept { return 0ULL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Max() noexcept { return ULLONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Lowest() noexcept { return 0ULL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Highest() noexcept { return ULLONG_MAX; }
};

template <>
struct NumericLimits<float> {
  VECCORE_ATT_HOST_DEVICE static constexpr float Min() noexcept { return FLT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Max() noexcept { return FLT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Lowest() noexcept { return -FLT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Highest() noexcept { return FLT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Epsilon() noexcept { return FLT_EPSILON; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Infinity() noexcept { return HUGE_VALF; }
};

template <>
struct NumericLimits<double> {
  VECCORE_ATT_HOST_DEVICE static constexpr double Min() noexcept { return DBL_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Max() noexcept { return DBL_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Lowest() noexcept { return -DBL_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Highest() noexcept { return DBL_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Epsilon() noexcept { return DBL_EPSILON; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Infinity() noexcept { return HUGE_VAL; }
};

} // namespace vecCore

#endif
