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

template <typename T> T Abs(T x) { return std::abs(x); }

template <typename T> constexpr T Min(T a, T b) { return a < b ? a : b; }
template <typename T> constexpr T Max(T a, T b) { return a > b ? a : b; }
template <typename T> constexpr T Min(T a, T b, T c) { return a < b ? Min(a, c) : Min(b, c); }
template <typename T> constexpr T Max(T a, T b, T c) { return a > b ? Max(a, c) : Max(b, c); }

template <typename T> void SetMin(T& a, const T& b) { if (a > b) a = b; }
template <typename T> void SetMax(T& a, const T& b) { if (a < b) a = b; }

template <typename T> constexpr T Sgn(const T& x)
{
    return (static_cast<T>(0) < x) - (x < static_cast<T>(0));
}

// Trigonometric Functions

template <typename T> T Sin(T x) { return std::sin(x); }
template <typename T> T Cos(T x) { return std::cos(x); }
template <typename T> T Tan(T x) { return std::tan(x); }
template <typename T> T Asin(T x) { return std::asin(x); }
template <typename T> T Acos(T x) { return std::acos(x); }
template <typename T> T Atan(T x) { return std::atan(x); }
template <typename T> T Atan2(T x, T y) { return std::atan2(x, y); }
template <typename T> T SinCos(T x) { return sincos(x); }

// Hyperbolic Functions

template <typename T> T Sinh(T x) { return std::sinh(x); }
template <typename T> T Cosh(T x) { return std::cosh(x); }
template <typename T> T Tanh(T x) { return std::tanh(x); }
template <typename T> T Asinh(T x) { return std::asinh(x); }
template <typename T> T Acosh(T x) { return std::acosh(x); }
template <typename T> T Atanh(T x) { return std::atanh(x); }

// Exponential and Logarithmic Functions

template <typename T> T Exp(T x) { return std::exp(x); }
template <typename T> T Frexp(T x, int* exp) { return std::frexp(x, exp); }
template <typename T> T Ldexp(T x, int exp) { return std::ldexp(x, exp); }
template <typename T> T Log(T x) { return std::log(x); }
template <typename T> T Log10(T x) { return std::log10(x); }
template <typename T> T Modf(T x, T* intpart) { return std::modf(x, intpart); }
template <typename T> T Exp2(T x) { return std::exp2(x); }
template <typename T> T Expm1(T x) { return std::expm1(x); }
template <typename T> T Ilogb(T x) { return std::ilogb(x); }
template <typename T> T Log1p(T x) { return std::log1p(x); }
template <typename T> T Log2(T x) { return std::log2(x); }
template <typename T> T Logb(T x) { return std::logb(x); }
template <typename T> T Scalbn(T x, int n) { return std::scalbn(x, n); }
template <typename T> T Scalbln(T x, long int n) { return std::scalbln(x, n); }

// Power Functions

template <typename T> T Sqrt(T x) { return std::sqrt(x); }
template <typename T> T Cbrt(T x) { return std::cbrt(x); }
template <typename T> T Pow(T x, T y) { return std::pow(x, y); }
template <typename T> T Hypot(T x, T y) { return std::hypot(x, y); }

// Rounding and Remainder Functions

template <typename T> T Ceil(T x) { return std::ceil(x); }
template <typename T> T Floor(T x) { return std::floor(x); }
template <typename T> T Fmod(T x, T y) { return std::fmod(x, y); }
template <typename T> T Trunc(T x) { return std::trunc(x); }
template <typename T> T Round(T x) { return std::round(x); }
template <typename T> T Remainder(T x, T y) { return std::remainder(x, y); }
template <typename T> T Remquo(T numer, T denom, T *quot) { return std::remquo(numer, denom, quot); }

// Miscellaneous Utilities

template <typename T> T Fma(T x) { return std::fma(x); }
template <typename T> constexpr T Deg(const T& x) { return (x * static_cast<T>(180.0 / M_PI)); }
template <typename T> constexpr T Rad(const T& x) { return (x * static_cast<T>(M_PI / 180.0)); }

template <typename T>
bool Infinitesimal(const T& x)
{
    static_assert(std::is_floating_point<T>::value,
        "Infinitesimal() is intended only for floating point types");

    return std::abs(x) < std::numeric_limits<T>::epsilon();
}

template <typename T>
bool AlmostEqual(const T& x, const T& y)
{
    static_assert(std::is_floating_point<T>::value,
        "AlmostEqual() is intended only for floating point types");

    if (x == y)
        return true;

    return std::abs(x - y) < (std::abs(x) + std::abs(y)) * std::numeric_limits<T>::epsilon();
}

template <class T>
inline bool InRange(const T& x, const T& min, const T& max)
{
    return min < max ? (x >= min) && (x <= max)
                     : (x >= max) && (x <= min);
}

template <class T>
T Clamp(const T& x, const T& xmin, const T& xmax)
{
    return Max<T>(xmin, Min<T>(x, xmax));
}

} // namespace math
} // namespace vecCore

#endif
