#ifndef VECCORE_MATH_H
#define VECCORE_MATH_H

#include <cmath>
#include <limits>
#include <type_traits>

#ifdef __APPLE__
#define sincos(x) __sincos(x)
#define sincosf(x) __sincosf(x)
#endif

namespace vecCore {
namespace math {

// Abs, Min, Max, Sign

template <typename T> VECCORE_FORCE_INLINE T Abs(T x) { return std::abs(x); }

template <class T> VECCORE_FORCE_INLINE T Min(T a, T b) { return Blend(a < b, a, b); }
template <class T> VECCORE_FORCE_INLINE T Max(T a, T b) { return Blend(a > b, a, b); }

template <typename T> VECCORE_FORCE_INLINE T Min(T a, T b, T c) { return Min(a, Min(b,c)); }
template <typename T> VECCORE_FORCE_INLINE T Max(T a, T b, T c) { return Max(a, Max(b,c)); }

template <typename T> VECCORE_FORCE_INLINE void SetMin(T& a, const T& b) { if (a > b) a = b; }
template <typename T> VECCORE_FORCE_INLINE void SetMax(T& a, const T& b) { if (a < b) a = b; }

template <typename T> VECCORE_FORCE_INLINE constexpr T Sgn(const T& x) { return Blend(x < T(0), T(-1), T(1)); }

// Trigonometric Functions

template <typename T> VECCORE_FORCE_INLINE T Sin(T x) { return std::sin(x); }
template <typename T> VECCORE_FORCE_INLINE T Cos(T x) { return std::cos(x); }
template <typename T> VECCORE_FORCE_INLINE T Tan(T x) { return std::tan(x); }
template <typename T> VECCORE_FORCE_INLINE T Asin(T x) { return std::asin(x); }
template <typename T> VECCORE_FORCE_INLINE T Acos(T x) { return std::acos(x); }
template <typename T> VECCORE_FORCE_INLINE T Atan(T x) { return std::atan(x); }
template <typename T> VECCORE_FORCE_INLINE T Atan2(T x, T y) { return std::atan2(x, y); }
template <typename T> VECCORE_FORCE_INLINE T SinCos(T x) { return sincos(x); }

// Hyperbolic Functions

template <typename T> VECCORE_FORCE_INLINE T Sinh(T x) { return std::sinh(x); }
template <typename T> VECCORE_FORCE_INLINE T Cosh(T x) { return std::cosh(x); }
template <typename T> VECCORE_FORCE_INLINE T Tanh(T x) { return std::tanh(x); }
template <typename T> VECCORE_FORCE_INLINE T Asinh(T x) { return std::asinh(x); }
template <typename T> VECCORE_FORCE_INLINE T Acosh(T x) { return std::acosh(x); }
template <typename T> VECCORE_FORCE_INLINE T Atanh(T x) { return std::atanh(x); }

// Exponential and Logarithmic Functions

template <typename T> VECCORE_FORCE_INLINE T Exp(T x) { return std::exp(x); }
template <typename T> VECCORE_FORCE_INLINE T Frexp(T x, int* exp) { return std::frexp(x, exp); }
template <typename T> VECCORE_FORCE_INLINE T Ldexp(T x, int exp) { return std::ldexp(x, exp); }
template <typename T> VECCORE_FORCE_INLINE T Log(T x) { return std::log(x); }
template <typename T> VECCORE_FORCE_INLINE T Log10(T x) { return std::log10(x); }
template <typename T> VECCORE_FORCE_INLINE T Modf(T x, T* intpart) { return std::modf(x, intpart); }
template <typename T> VECCORE_FORCE_INLINE T Exp2(T x) { return std::exp2(x); }
template <typename T> VECCORE_FORCE_INLINE T Expm1(T x) { return std::expm1(x); }
template <typename T> VECCORE_FORCE_INLINE T Ilogb(T x) { return std::ilogb(x); }
template <typename T> VECCORE_FORCE_INLINE T Log1p(T x) { return std::log1p(x); }
template <typename T> VECCORE_FORCE_INLINE T Log2(T x) { return std::log2(x); }
template <typename T> VECCORE_FORCE_INLINE T Logb(T x) { return std::logb(x); }
template <typename T> VECCORE_FORCE_INLINE T Scalbn(T x, int n) { return std::scalbn(x, n); }
template <typename T> VECCORE_FORCE_INLINE T Scalbln(T x, long int n) { return std::scalbln(x, n); }

// Power Functions

template <typename T> VECCORE_FORCE_INLINE T Sqrt(T x) { return std::sqrt(x); }
template <typename T> VECCORE_FORCE_INLINE T Cbrt(T x) { return std::cbrt(x); }
template <typename T> VECCORE_FORCE_INLINE T Pow(T x, T y) { return std::pow(x, y); }
template <typename T> VECCORE_FORCE_INLINE T Hypot(T x, T y) { return std::hypot(x, y); }

// Rounding and Remainder Functions

template <typename T> VECCORE_FORCE_INLINE T Ceil(T x) { return std::ceil(x); }
template <typename T> VECCORE_FORCE_INLINE T Floor(T x) { return std::floor(x); }
template <typename T> VECCORE_FORCE_INLINE T Fmod(T x, T y) { return std::fmod(x, y); }
template <typename T> VECCORE_FORCE_INLINE T Trunc(T x) { return std::trunc(x); }
template <typename T> VECCORE_FORCE_INLINE T Round(T x) { return std::round(x); }
template <typename T> VECCORE_FORCE_INLINE T Remainder(T x, T y) { return std::remainder(x, y); }
template <typename T> VECCORE_FORCE_INLINE T Remquo(T numer, T denom, T *quot) {
  return std::remquo(numer, denom, quot);
}

// Miscellaneous Utilities

template <typename T> VECCORE_FORCE_INLINE T Fma(T x) { return std::fma(x); }
template <typename T> VECCORE_FORCE_INLINE constexpr T Deg(const T& x) { return (x * static_cast<T>(180.0 / M_PI)); }
template <typename T> VECCORE_FORCE_INLINE constexpr T Rad(const T& x) { return (x * static_cast<T>(M_PI / 180.0)); }

template <typename T>
VECCORE_FORCE_INLINE
bool Infinitesimal(const T& x, const T epsilon = T(1.0e-6))
{
  static_assert(std::is_floating_point<T>::value,
                "Infinitesimal() is intended only for floating point types");

  return Abs(x) < epsilon;
}

template <typename T>
VECCORE_FORCE_INLINE
bool AlmostEqual(const T& x, const T& y, const T epsilon = T(1.0e-6))
{
  static_assert(std::is_floating_point<T>::value,
                "AlmostEqual() is intended only for floating point types");

  if (x == y)
    return true;

  return Abs(x - y) < (Abs(x) + Abs(y)) * epsilon;
}

template <class T>
VECCORE_FORCE_INLINE
bool InRange(const T& x, const T& min, const T& max)
{
  return Blend(min < max, (x >= min) && (x <= max), (x >= max) && (x <= min));
}

template <class T>
VECCORE_FORCE_INLINE
T Clamp(const T& x, const T& xmin, const T& xmax)
{
  return Max<T>(xmin, Min<T>(x, xmax));
}

} // namespace math
} // namespace vecCore

#endif
