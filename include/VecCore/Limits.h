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
  VECCORE_ATT_HOST_DEVICE static constexpr bool Min() { return false; }
  VECCORE_ATT_HOST_DEVICE static constexpr bool Max() { return true; }
};

template <>
struct NumericLimits<char> {
  VECCORE_ATT_HOST_DEVICE static constexpr char Min() { return CHAR_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr char Max() { return CHAR_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr char Lowest() { return CHAR_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr char Highest() { return CHAR_MAX; }
};

template <>
struct NumericLimits<unsigned char> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Min() { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Max() { return UCHAR_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Lowest() { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned char Highest() { return UCHAR_MAX; }
};

template <>
struct NumericLimits<short> {
  VECCORE_ATT_HOST_DEVICE static constexpr short Min() { return SHRT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr short Max() { return SHRT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr short Lowest() { return SHRT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr short Highest() { return SHRT_MAX; }
};

template <>
struct NumericLimits<unsigned short> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Min() { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Max() { return USHRT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Lowest() { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned short Highest() { return USHRT_MAX; }
};

template <>
struct NumericLimits<int> {
  VECCORE_ATT_HOST_DEVICE static constexpr int Min() { return INT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr int Max() { return INT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr int Lowest() { return INT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr int Highest() { return INT_MAX; }
};

template <>
struct NumericLimits<unsigned int> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Min() { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Max() { return UINT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Lowest() { return 0; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned int Highest() { return UINT_MAX; }
};

template <>
struct NumericLimits<long> {
  VECCORE_ATT_HOST_DEVICE static constexpr long Min() { return LONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long Max() { return LONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr long Lowest() { return LONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long Highest() { return LONG_MAX; }
};

template <>
struct NumericLimits<unsigned long> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Min() { return 0UL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Max() { return ULONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Lowest() { return 0UL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long Highest() { return ULONG_MAX; }
};

template <>
struct NumericLimits<long long> {
  VECCORE_ATT_HOST_DEVICE static constexpr long long Min() { return LLONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long long Max() { return LLONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr long long Lowest() { return LLONG_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr long long Highest() { return LLONG_MAX; }
};

template <>
struct NumericLimits<unsigned long long> {
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Min() { return 0ULL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Max() { return ULLONG_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Lowest() { return 0ULL; }
  VECCORE_ATT_HOST_DEVICE static constexpr unsigned long long Highest() { return ULLONG_MAX; }
};

template <>
struct NumericLimits<float> {
  VECCORE_ATT_HOST_DEVICE static constexpr float Min() { return FLT_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Max() { return FLT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Lowest() { return -FLT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Highest() { return FLT_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Epsilon() { return FLT_EPSILON; }
  VECCORE_ATT_HOST_DEVICE static constexpr float Infinity() noexcept { return HUGE_VALF; }
};

template <>
struct NumericLimits<double> {
  VECCORE_ATT_HOST_DEVICE static constexpr double Min() { return DBL_MIN; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Max() { return DBL_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Lowest() { return -DBL_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Highest() { return DBL_MAX; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Epsilon() { return DBL_EPSILON; }
  VECCORE_ATT_HOST_DEVICE static constexpr double Infinity() noexcept { return HUGE_VAL; }
};

} // namespace vecCore

#endif
