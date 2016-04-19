#ifndef VECCORE_MATH_H
#define VECCORE_MATH_H

#include <cmath>

namespace vecCore {
namespace math {

// Abs, Min, Max, Sign

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Abs(T x) { return std::abs(x); }

template <class T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Min(const T a, const T b) { return Blend(a < b, a, b); }

template <class T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Max(const T a, const T b) { return Blend(a > b, a, b); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Min(T a, T b, T c) { return Min(a, Min(b, c)); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Max(T a, T b, T c) { return Max(a, Max(b, c)); }

template <typename T, template <typename> class Wrapper>
VECCORE_FORCE_INLINE VECCORE_CUDA_HOST_DEVICE
Wrapper<T> Min(const Wrapper<T> &a, const Wrapper<T> &b) { return Blend(a < b, a, b); }

template <typename T, template <typename> class Wrapper>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
Wrapper<T> Max(const Wrapper<T> &a, const Wrapper<T> &b) { return Blend(a > b, a, b); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Sign(const T &x) { return Blend(x < T(0), T(-1), T(1)); }

// Trigonometric Functions

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Sin(T x) { return std::sin(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Cos(T x) { return std::cos(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void SinCos(T x, T &s, T &c) { sincos(x, &s, &c); }

#ifdef __APPLE__
VECCORE_FORCE_INLINE
void sincosf(float x, float *s, float *c) { __sincosf(x, s, c); }

VECCORE_FORCE_INLINE
void sincos(double x, double *s, double *c) { __sincos(x, s, c); }
#endif

template <>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void SinCos(Float_s x, Float_s &s, Float_s &c) { sincosf(x, &s, &c); }

template <>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
void SinCos(Double_s x, Double_s &s, Double_s &c) { sincos(x, &s, &c); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Tan(T x) { return std::tan(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Asin(T x) { return std::asin(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Acos(T x) { return std::acos(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Atan(T x) { return std::atan(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Atan2(T x, T y) { return std::atan2(x, y); }

// Hyperbolic Functions

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Sinh(T x) { return std::sinh(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Cosh(T x) { return std::cosh(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Tanh(T x) { return std::tanh(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Asinh(T x) { return std::asinh(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Acosh(T x) { return std::acosh(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Atanh(T x) { return std::atanh(x); }

// Exponential and Logarithmic Functions

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Exp(T x) { return std::exp(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Frexp(T x, int *exp) { return std::frexp(x, exp); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Ldexp(T x, int exp) { return std::ldexp(x, exp); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Log(T x) { return std::log(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Log10(T x) { return std::log10(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Modf(T x, T *intpart) { return std::modf(x, intpart); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Exp2(T x) { return std::exp2(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Expm1(T x) { return std::expm1(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Ilogb(T x) { return std::ilogb(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Log1p(T x) { return std::log1p(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Log2(T x) { return std::log2(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Logb(T x) { return std::logb(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Scalbn(T x, int n) { return std::scalbn(x, n); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Scalbln(T x, long int n) { return std::scalbln(x, n); }

// Power Functions

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Sqrt(T x) { return std::sqrt(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Cbrt(T x) { return std::cbrt(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Pow(T x, T y) { return std::pow(x, y); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Hypot(T x, T y) { return std::hypot(x, y); }

// Rounding and Remainder Functions

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Ceil(T x) { return std::ceil(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Floor(T x) { return std::floor(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Fmod(T x, T y) { return std::fmod(x, y); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Trunc(T x) { return std::trunc(x); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
T Round(T x) { return std::round(x); }

// Miscellaneous Utilities

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
constexpr T Deg(const T &x) { return (x * T(180.0 / M_PI)); }

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_CUDA_HOST_DEVICE
constexpr T Rad(const T &x) { return (x * T(M_PI / 180.0)); }

} // namespace math
} // namespace vecCore

#endif
