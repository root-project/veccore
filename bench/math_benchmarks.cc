#include <VecCore/VecCore>
#include <benchmark/benchmark.h>

#include <iomanip>
#include <iostream>
#include <random>

using namespace vecCore;

constexpr size_t N = 128;

#define BENCHMARK_MATH_FUNCTION_RANGE(mathfunc, impl, a, b)                    \
  void mathfunc##_##impl(benchmark::State &state)                              \
  {                                                                            \
    using Vector_t = backend::impl::Double_v;                                  \
    using Scalar_t = Scalar<Vector_t>;                                         \
                                                                               \
    Scalar_t *input     = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
    Scalar_t *reference = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
    Scalar_t *output    = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
                                                                               \
    std::random_device rng;                                                    \
    std::default_random_engine g(rng());                                       \
    std::uniform_real_distribution<Scalar_t> dist(a, b);                       \
                                                                               \
    for(size_t i = 0; i < N; i++) {                                            \
      input[i] = dist(g);                                                      \
      reference[i] = mathfunc(input[i]);                                       \
    }                                                                          \
                                                                               \
    for(auto _ : state) {                                                      \
      for (size_t i = 0; i < N; i += VectorSize<Vector_t>()) {                 \
        Vector_t x;                                                            \
        Load(x, &input[i]);                                                    \
        Store(mathfunc(x), &output[i]);                                        \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (size_t i = 0; i < N; i++) {                                           \
      Scalar_t diff = std::abs((output[i]-reference[i])/reference[i]);         \
      if (diff > 1.0e-02) {                                                    \
        std::cout << std::setprecision(18)                                     \
                  << "Warning: values differ:"                                 \
                  << " input:     " << input[i]                                \
                  << " reference: " << reference[i]                            \
                  << " output:    " << output[i]                               \
                  << std::endl;                                                \
       std::abort();                                                           \
      }                                                                        \
    }                                                                          \
    AlignedFree(input);                                                        \
    AlignedFree(reference);                                                    \
    AlignedFree(output);                                                       \
  }

#define BENCHMARK_MATH_FUNCTION_SCALAR(f, a, b)                                \
   BENCHMARK_MATH_FUNCTION_RANGE(f, Scalar, a, b);                             \
   BENCHMARK_MATH_FUNCTION_RANGE(f, ScalarWrapper, a, b);                      \
   BENCHMARK(f##_Scalar);                                                      \
   BENCHMARK(f##_ScalarWrapper);                                               \

#ifdef VECCORE_ENABLE_VC
  #define BENCHMARK_MATH_FUNCTION_VC(f, a, b)                                  \
     BENCHMARK_MATH_FUNCTION_RANGE(f, VcScalar, a, b);                         \
     BENCHMARK_MATH_FUNCTION_RANGE(f, VcVector, a, b);                         \
     BENCHMARK(f##_VcScalar);                                                  \
     BENCHMARK(f##_VcVector);
#else
  #define BENCHMARK_MATH_FUNCTION_VC(f, a, b)
#endif

#ifdef VECCORE_ENABLE_UMESIMD
  #define BENCHMARK_MATH_FUNCTION_UMESIMD(f, a, b)                             \
     BENCHMARK_MATH_FUNCTION_RANGE(f, UMESimd, a, b);                          \
     BENCHMARK(f##_UMESimd);
#else
  #define BENCHMARK_MATH_FUNCTION_UMESIMD(f, a, b)
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
  #define BENCHMARK_MATH_FUNCTION_SIMD(f, a, b)                                \
     BENCHMARK_MATH_FUNCTION_RANGE(f, SIMDScalar, a, b);                       \
     BENCHMARK_MATH_FUNCTION_RANGE(f, SIMDNative, a, b);                       \
     BENCHMARK(f##_SIMDScalar);                                                \
     BENCHMARK(f##_SIMDNative);
#else
  #define BENCHMARK_MATH_FUNCTION_SIMD(f, a, b)
#endif

#define BENCHMARK_MATH_FUNCTION(f, a, b)                                       \
   BENCHMARK_MATH_FUNCTION_SCALAR(f, a, b);                                    \
   BENCHMARK_MATH_FUNCTION_SIMD(f, a, b);                                      \
   BENCHMARK_MATH_FUNCTION_VC(f, a, b);                                        \
   BENCHMARK_MATH_FUNCTION_UMESIMD(f, a, b);                                   \

BENCHMARK_MATH_FUNCTION(Abs,   -FLT_MAX, FLT_MAX);
BENCHMARK_MATH_FUNCTION(Floor, -FLT_MAX, FLT_MAX);
BENCHMARK_MATH_FUNCTION(Ceil,  -FLT_MAX, FLT_MAX);
// BENCHMARK_MATH_FUNCTION(Trunc, -FLT_MAX, FLT_MAX); // broken in Vc

BENCHMARK_MATH_FUNCTION(Sin, 0, 2*M_PI);
BENCHMARK_MATH_FUNCTION(Cos, 0, 2*M_PI);
BENCHMARK_MATH_FUNCTION(Tan, 0, 2*M_PI);

BENCHMARK_MATH_FUNCTION(ASin, 0, 1);
BENCHMARK_MATH_FUNCTION(ACos, 0, 1);
BENCHMARK_MATH_FUNCTION(ATan, -FLT_MAX, FLT_MAX);

BENCHMARK_MATH_FUNCTION(Exp, -10, 10);
BENCHMARK_MATH_FUNCTION(Log, 1, 1000);
BENCHMARK_MATH_FUNCTION(TGamma, 1, 1000);

BENCHMARK_MATH_FUNCTION(Sqrt, 0, 1000);
BENCHMARK_MATH_FUNCTION(Cbrt, 0, 1000);
BENCHMARK_MATH_FUNCTION(Rsqrt, 1, 1000);

#undef BENCHMARK_MATH_FUNCTION_RANGE
#undef BENCHMARK_MATH_FUNCTION


#define BENCHMARK_MATH_FUNCTION2_RANGE(mathfunc, impl, a, b, c, d)             \
  void mathfunc##_##impl(benchmark::State &state)                              \
  {                                                                            \
    using Vector_t = backend::impl::Float_v;                                   \
    using Scalar_t = Scalar<Vector_t>;                                         \
                                                                               \
    Scalar_t *input1    = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
    Scalar_t *input2    = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
    Scalar_t *reference = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
    Scalar_t *output    = (Scalar_t *)AlignedAlloc(64, N * sizeof(Scalar_t));  \
                                                                               \
    std::random_device rng;                                                    \
    std::default_random_engine g(rng());                                       \
    std::uniform_real_distribution<Scalar_t> dist_ab(a, b);                    \
    std::uniform_real_distribution<Scalar_t> dist_cd(c, d);                    \
                                                                               \
    for(size_t i = 0; i < N; i++) {                                            \
      input1[i] = dist_ab(g);                                                  \
      input2[i] = dist_cd(g);                                                  \
      reference[i] = mathfunc(input1[i], input2[i]);                           \
    }                                                                          \
                                                                               \
    for(auto _ : state) {                                                      \
      for (size_t i = 0; i < N; i += VectorSize<Vector_t>()) {                 \
        Vector_t x, y;                                                         \
        Load(x, &input1[i]);                                                   \
        Load(y, &input2[i]);                                                   \
        Store(mathfunc(x, y), &output[i]);                                     \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (size_t i = 0; i < N; i++) {                                           \
      Scalar_t diff = std::abs((output[i]-reference[i])/reference[i]);         \
      if (diff > 1.0e-02) {                                                    \
        std::cout << std::setprecision(18)                                     \
                  << "Warning: values differ:"                                 \
                  << " input 1:     " << input1[i]                             \
                  << " input 2:     " << input2[i]                             \
                  << " reference: " << reference[i]                            \
                  << " output:    " << output[i]                               \
                  << std::endl;                                                \
       std::abort();                                                           \
      }                                                                        \
    }                                                                          \
    AlignedFree(input1);                                                       \
    AlignedFree(input2);                                                       \
    AlignedFree(reference);                                                    \
    AlignedFree(output);                                                       \
  }

#define BENCHMARK_MATH_FUNCTION2_SCALAR(f, a, b, c, d)                         \
   BENCHMARK_MATH_FUNCTION2_RANGE(f, Scalar, a, b, c, d);                      \
   BENCHMARK_MATH_FUNCTION2_RANGE(f, ScalarWrapper, a, b, c, d);               \
   BENCHMARK(f##_Scalar);                                                      \
   BENCHMARK(f##_ScalarWrapper);                                               \

#ifdef VECCORE_ENABLE_VC
  #define BENCHMARK_MATH_FUNCTION2_VC(f, a, b, c, d)                           \
     BENCHMARK_MATH_FUNCTION2_RANGE(f, VcScalar, a, b, c, d);                  \
     BENCHMARK_MATH_FUNCTION2_RANGE(f, VcVector, a, b, c, d);                  \
     BENCHMARK(f##_VcScalar);                                                  \
     BENCHMARK(f##_VcVector);
#else
  #define BENCHMARK_MATH_FUNCTION2_VC(f, a, b, c, d)
#endif

#ifdef VECCORE_ENABLE_UMESIMD
  #define BENCHMARK_MATH_FUNCTION2_UMESIMD(f, a, b, c, d)                      \
     BENCHMARK_MATH_FUNCTION2_RANGE(f, UMESimd, a, b, c, d);                   \
     BENCHMARK(f##_UMESimd);
#else
  #define BENCHMARK_MATH_FUNCTION2_UMESIMD(f, a, b, c, d)
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
  #define BENCHMARK_MATH_FUNCTION2_SIMD(f, a, b, c, d)                         \
     BENCHMARK_MATH_FUNCTION2_RANGE(f, SIMDScalar, a, b, c, d);                \
     BENCHMARK_MATH_FUNCTION2_RANGE(f, SIMDNative, a, b, c, d);                \
     BENCHMARK(f##_SIMDScalar);                                                \
     BENCHMARK(f##_SIMDNative);
#else
  #define BENCHMARK_MATH_FUNCTION2_SIMD(f, a, b, c, d)
#endif

#define BENCHMARK_MATH_FUNCTION2(f, a, b, c, d)                                \
   BENCHMARK_MATH_FUNCTION2_SCALAR(f, a, b,c , d);                             \
   BENCHMARK_MATH_FUNCTION2_VC(f, a, b, c, d);                                 \
   BENCHMARK_MATH_FUNCTION2_UMESIMD(f, a, b, c, d);                            \

BENCHMARK_MATH_FUNCTION2(CopySign, -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX);
BENCHMARK_MATH_FUNCTION2(Pow, FLT_MIN, FLT_MAX, 1.0, 10.0);
BENCHMARK_MATH_FUNCTION2(ATan2, -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX);

#undef BENCHMARK_MATH_FUNCTION2_RANGE
#undef BENCHMARK_MATH_FUNCTION2

BENCHMARK_MAIN();
