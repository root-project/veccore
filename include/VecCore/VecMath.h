#ifndef VECCORE_MATH_H
#define VECCORE_MATH_H

#include <cmath>

namespace vecCore {
namespace math {

// Min, Max, Sign

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

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Sign(const T &x)
{
  return CopySign(T(1), x);
}

#ifdef VECCORE_ENABLE_LIBMVEC

#define VECMATH_REAL_FUNC2(f, name)                    \
  template <typename T>                                \
  VECCORE_FORCE_INLINE                                 \
  VECCORE_ATT_HOST_DEVICE                              \
  T f(const T &x, const T &y)                          \
  {                                                    \
    T output;                                          \
    int v_size = VectorSize(x);                        \
                                                       \
    _Pragma("omp simd")                                \
    for(int i = 0; i < v_size; i++)                    \
    {                                                  \
      Set(output, i, std::name(Get(x, i), Get(y, i))); \
    }                                                  \
    return output;                                     \
  }

#else

# define VECMATH_REAL_FUNC2(f, name)     \
  template <typename T>                  \
  VECCORE_FORCE_INLINE                   \
  VECCORE_ATT_HOST_DEVICE                \
  T f(const T &x, const T &y)            \
  {                                      \
      return std::name(x, y);            \
  }

#endif

VECMATH_REAL_FUNC2(CopySign, copysign)
VECMATH_REAL_FUNC2(ATan2, atan2)
VECMATH_REAL_FUNC2(Pow, pow)
VECMATH_REAL_FUNC2(Hypot, hypot)
VECMATH_REAL_FUNC2(Fmod, fmod)

#undef VECMATH_REAL_FUNC2


#ifdef VECCORE_ENABLE_LIBMVEC

#define VECMATH_REAL_FUNC(f, name)                                 \
  template <typename T>                                            \
  VECCORE_FORCE_INLINE VECCORE_ATT_HOST_DEVICE T f(const T &input) \
  {                                                                \
    T output;                                                      \
    int v_size = VectorSize(input);                                \
                                                                   \
    _Pragma("omp simd")                                            \
    for(int i = 0; i < v_size; i++)                                \
    {                                                              \
      Set(output, i, std::name(Get(input, i)));                    \
    }                                                              \
    return output;                                                 \
  }

#else

#define VECMATH_REAL_FUNC(f, name)  \
  template <typename T>             \
  VECCORE_FORCE_INLINE              \
  VECCORE_ATT_HOST_DEVICE           \
  T f(const T &input)               \
  {                                 \
    return std::name(input);        \
  }                                 \

#endif

VECMATH_REAL_FUNC(Abs, abs)
VECMATH_REAL_FUNC(Sin, sin)
VECMATH_REAL_FUNC(Cos, cos)
VECMATH_REAL_FUNC(Tan, tan)
VECMATH_REAL_FUNC(ASin, asin)
VECMATH_REAL_FUNC(ACos, acos)
VECMATH_REAL_FUNC(ATan, atan)
VECMATH_REAL_FUNC(Sinh, sinh)
VECMATH_REAL_FUNC(Cosh, cosh)
VECMATH_REAL_FUNC(Tanh, tanh)
VECMATH_REAL_FUNC(ASinh, asinh)
VECMATH_REAL_FUNC(ACosh, acosh)
VECMATH_REAL_FUNC(ATanh, atanh)
VECMATH_REAL_FUNC(Sqrt, sqrt)
VECMATH_REAL_FUNC(Cbrt, cbrt)
VECMATH_REAL_FUNC(Exp, exp)
VECMATH_REAL_FUNC(Exp2, exp2)
VECMATH_REAL_FUNC(Expm1, expm1)
VECMATH_REAL_FUNC(Ldexp, ldexp)
VECMATH_REAL_FUNC(Log, log)
VECMATH_REAL_FUNC(ILogb, ilogb)
VECMATH_REAL_FUNC(Log1p, log1p)
VECMATH_REAL_FUNC(Log10, log10)
VECMATH_REAL_FUNC(Log2, log2)
VECMATH_REAL_FUNC(Logb, logb)
VECMATH_REAL_FUNC(Scalbn, scalbn)
VECMATH_REAL_FUNC(Scalbln, scalbln)
VECMATH_REAL_FUNC(Ceil, ceil)
VECMATH_REAL_FUNC(Floor, floor)
VECMATH_REAL_FUNC(Fmod, fmod)
VECMATH_REAL_FUNC(Trunc, trunc)
VECMATH_REAL_FUNC(Round, round)

#undef VECMATH_REAL_FUNC

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void SinCos(const T &x, T *s, T *c)
{
  sincos(x, s, c);
}

#if defined(__APPLE__) && !defined(NVCC)
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
#endif

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void SinCos(const Float_s &x, Float_s *s, Float_s *c)
{
  sincosf(x, s, c);
}

template <>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void SinCos(const Double_s &x, Double_s *s, Double_s *c)
{
  sincos(x, s, c);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Frexp(const T &x, int *exp)
{
  return std::frexp(x, exp);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Ldexp(const T &x, int exp)
{
  return std::ldexp(x, exp);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Modf(const T &x, T *intpart)
{
  return std::modf(x, intpart);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Scalbn(const T &x, int n)
{
  return std::scalbn(x, n);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T Scalbln(const T &x, long int n)
{
  return std::scalbln(x, n);
}

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

} // namespace math
} // namespace vecCore

#endif
