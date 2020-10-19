#ifndef VECCORE_MATH_H
#define VECCORE_MATH_H

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include <cmath>

#if defined(__APPLE__) && !defined(__NVCC__)
VECCORE_FORCE_INLINE
void sincosf(const float &x, float *s, float *c)
{
  __sincosf(x, s, c);
}

VECCORE_FORCE_INLINE
void sincos(const double &x, double *s, double *c)
{
  __sincos(x, s, c);
}
#elif defined(_MSC_VER)
VECCORE_FORCE_INLINE
void sincosf(const float &x, float *s, float *c)
{
  *s = sinf(x);
  *c = cosf(x);
}

VECCORE_FORCE_INLINE
void sincos(const double &x, double *s, double *c)
{
  *s = sin(x);
  *c = cos(x);
}
#endif

namespace vecCore {
inline namespace math {

#define VECCORE_MATH_UNARY_FUNCTION(F, f)     \
template <typename T>                         \
VECCORE_FORCE_INLINE VECCORE_ATT_HOST_DEVICE  \
T F(const T &x)                               \
{                                             \
  T ret(x);                                   \
  for(size_t i = 0; i < VectorSize<T>(); ++i) \
    Set(ret, i, std::f(Get(x,i)));            \
  return ret;                                 \
}                                             \

#define VECCORE_MATH_BINARY_FUNCTION(F, f)    \
template <typename T>                         \
VECCORE_FORCE_INLINE VECCORE_ATT_HOST_DEVICE  \
T F(const T &x, const T &y)                   \
{                                             \
  T ret(x);                                   \
  for(size_t i = 0; i < VectorSize<T>(); ++i) \
    Set(ret, i, std::f(Get(x,i), Get(y,i)));  \
  return ret;                                 \
}                                             \

// Abs, Min, Max, Sign

VECCORE_MATH_UNARY_FUNCTION(Abs, abs)

template <class T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Min(const T &a, const T &b)
{
  return Blend(a < b, a, b);
}

template <class T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Max(const T &a, const T &b)
{
  return Blend(a > b, a, b);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Min(const T &a, const T &b, const T &c)
{
  return Min(a, Min(b, c));
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Max(const T &a, const T &b, const T &c)
{
  return Max(a, Max(b, c));
}

template <typename T, template <typename> class Wrapper>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
Wrapper<T> Min(const Wrapper<T> &a, const Wrapper<T> &b)
{
  return Blend(a < b, a, b);
}

template <typename T, template <typename> class Wrapper>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
Wrapper<T> Max(const Wrapper<T> &a, const Wrapper<T> &b)
{
  return Blend(a > b, a, b);
}

VECCORE_MATH_BINARY_FUNCTION(CopySign, copysign)

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Sign(const T &x)
{
  return CopySign(T(1), x);
}

// Trigonometric Functions

VECCORE_MATH_UNARY_FUNCTION(Sin, sin)
VECCORE_MATH_UNARY_FUNCTION(Cos, cos)
VECCORE_MATH_UNARY_FUNCTION(Tan, tan)

VECCORE_MATH_UNARY_FUNCTION(ASin, asin)
VECCORE_MATH_UNARY_FUNCTION(ACos, acos)
VECCORE_MATH_UNARY_FUNCTION(ATan, atan)
VECCORE_MATH_BINARY_FUNCTION(ATan2, atan2)

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void SinCos(const T &x, T *s, T *c)
{
  sincos(x, s, c);
}

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void SinCos(const float &x, float *s, float *c)
{
  ::sincosf(x, s, c);
}

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void SinCos(const double &x, double *s, double *c)
{
  ::sincos(x, s, c);
}

// Hyperbolic Functions

VECCORE_MATH_UNARY_FUNCTION(Sinh, sinh)
VECCORE_MATH_UNARY_FUNCTION(Cosh, cosh)
VECCORE_MATH_UNARY_FUNCTION(Tanh, tanh)

VECCORE_MATH_UNARY_FUNCTION(ASinh, asinh)
VECCORE_MATH_UNARY_FUNCTION(ACosh, acosh)
VECCORE_MATH_UNARY_FUNCTION(ATanh, atanh)

// Exponential and Logarithmic Functions

VECCORE_MATH_UNARY_FUNCTION(Exp, exp)
VECCORE_MATH_UNARY_FUNCTION(Exp2, exp2)
VECCORE_MATH_UNARY_FUNCTION(Expm1, expm1)
VECCORE_MATH_UNARY_FUNCTION(Log, log)
VECCORE_MATH_UNARY_FUNCTION(Log2, log2)
VECCORE_MATH_UNARY_FUNCTION(Logb, logb)
VECCORE_MATH_UNARY_FUNCTION(Log10, log10)
VECCORE_MATH_UNARY_FUNCTION(Log1p, log1p)
VECCORE_MATH_UNARY_FUNCTION(Ilogb, ilogb)

// Gamma Functions
VECCORE_MATH_UNARY_FUNCTION(TGamma, tgamma)

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Frexp(const T &x, int *exp)
{
  T ret;
  for(size_t i = 0; i < VectorSize<T>(); ++i)
    Set(ret, i, std::frexp(Get(x,i), &exp[i]));
  return ret;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Ldexp(const T &x, int exp)
{
  T ret;
  for(size_t i = 0; i < VectorSize<T>(); ++i)
    Set(ret, i, std::ldexp(Get(x,i), exp));
  return ret;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Modf(const T &x, T *intpart)
{
  T ret;
  for(size_t i = 0; i < VectorSize<T>(); ++i)
    Set(ret, i, std::modf(Get(x,i), &intpart[i]));
  return ret;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Rsqrt(const T &x)
{
  T ret(1.0 / x);
  for(size_t i = 0; i < VectorSize<T>(); ++i)
    Set(ret, i, std::sqrt(Get(ret,i)));
  return ret;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Scalbn(const T &x, int n)
{
  T ret;
  for(size_t i = 0; i < VectorSize<T>(); ++i)
    Set(ret, i, std::scalbn(Get(x,i), n));
  return ret;
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Scalbln(const T &x, long int n)
{
  T ret;
  for(size_t i = 0; i < VectorSize<T>(); ++i)
    Set(ret, i, std::scalbln(Get(x,i), n));
  return ret;
}

// Power Functions

VECCORE_MATH_UNARY_FUNCTION(Sqrt, sqrt)
VECCORE_MATH_UNARY_FUNCTION(Cbrt, cbrt)
VECCORE_MATH_BINARY_FUNCTION(Pow, pow)
VECCORE_MATH_BINARY_FUNCTION(Hypot, hypot)

// Rounding and Remainder Functions

VECCORE_MATH_UNARY_FUNCTION(Ceil, ceil)
VECCORE_MATH_UNARY_FUNCTION(Floor, floor)
VECCORE_MATH_BINARY_FUNCTION(Fmod, fmod)
VECCORE_MATH_UNARY_FUNCTION(Round, round)
VECCORE_MATH_UNARY_FUNCTION(Trunc, trunc)

// Miscellaneous Utilities

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
Mask<T> IsInf(const T &x)
{
#ifndef VECCORE_CUDA
  using std::isinf;
#endif
  return isinf(x);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
Mask<T> IsNegative(const T &x)
{
  return x < T(0);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
constexpr T Deg(const T &x)
{
  return (x * T(180.0 / M_PI));
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
constexpr T Rad(const T &x)
{
  return (x * T(M_PI / 180.0));
}

#undef VECCORE_MATH_UNARY_FUNCTION
#undef VECCORE_MATH_BINARY_FUNCTION

} // namespace math
} // namespace vecCore

#endif
