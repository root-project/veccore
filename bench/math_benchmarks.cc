#include <cmath>
#include <iostream>
#include <VecCore/VecCore>
#include <benchmark/benchmark.h>

using namespace vecCore;

double uniform_random(double a, double b)
{
  return a + (b - a) * drand48();
}

#define BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, a, b)                    \
  void BM_##type##_##mathfunc(benchmark::State &state)                         \
  {                                                                            \
                                                                               \
    using Vector_t = backend::type::Float_v;                                   \
    using Scalar_t = ScalarType<Vector_t>::Type;                               \
                                                                               \
    auto kVS = VectorSize<Vector_t>();                                         \
    size_t N = 64 * kVS;                                                       \
    Scalar_t input[N]      __attribute__((aligned(64)));                       \
    Scalar_t reference[N]  __attribute__((aligned(64)));                       \
    Scalar_t output[N]     __attribute__((aligned(64)));                       \
                                                                               \
    for(size_t i = 0; i < N; i++) {                                            \
      input[i] = static_cast<Scalar_t>(uniform_random(a, b));                  \
      reference[i] = math::mathfunc(input[i]);                                 \
    }                                                                          \
                                                                               \
    while (state.KeepRunning()) {                                              \
      Vector_t x;                                                              \
      for (size_t j = 0; j < N; j += kVS) {                                    \
        Load<Vector_t>(x, &input[j]);                                          \
        Store<Vector_t>(math::mathfunc(x), &output[j]);                        \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (size_t i = 0; i < N; i++) {                                           \
      Scalar_t diff = std::abs((output[i]-reference[i])/reference[i]);         \
      if (diff > 1.0e-02)                                                      \
        std::cout << "Warning: values differ:"                                 \
                  << " input:     " << input[i]                                \
                  << " reference: " << reference[i]                            \
                  << " output:    " << output[i]                               \
                  << std::endl;                                                \
    }                                                                          \
  }                                                                            \


#define BENCHMARK_MATH_FUNCTION_HIGH(type, mathfunc) \
  BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, -38.0, 38.0)

#define BENCHMARK_MATH_FUNCTION_NORM(type, mathfunc) \
  BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, -1.0 * FLT_MAX, FLT_MAX)

#define BENCHMARK_MATH_FUNCTION_TRIG(type, mathfunc) \
  BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, 0.0, 2.0 * M_PI)

#define BENCHMARK_MATH_FUNCTION_POS(type, mathfunc) \
  BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, 0.0, 1.0)

#define BENCHMARK_MATH_FUNCTION_NEG(type, mathfunc) \
  BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, -1.0, 0.0)

#define BENCHMARK_MATH_FUNCTION_ONE(type, mathfunc) \
  BENCHMARK_MATH_FUNCTION_RANGE(type, mathfunc, -1.0, 1.0)

BENCHMARK_MATH_FUNCTION_NORM(Scalar, Abs);
BENCHMARK_MATH_FUNCTION_NORM(ScalarWrapper, Abs);
BENCHMARK_MATH_FUNCTION_NORM(VcScalar, Abs);
BENCHMARK_MATH_FUNCTION_NORM(VcVector, Abs);
BENCHMARK_MATH_FUNCTION_NORM(UMESimd, Abs);

BENCHMARK_MATH_FUNCTION_NORM(Scalar, Floor);
BENCHMARK_MATH_FUNCTION_NORM(ScalarWrapper, Floor);
BENCHMARK_MATH_FUNCTION_NORM(VcScalar, Floor);
BENCHMARK_MATH_FUNCTION_NORM(VcVector, Floor);
BENCHMARK_MATH_FUNCTION_NORM(UMESimd, Floor);

BENCHMARK_MATH_FUNCTION_NORM(Scalar, Ceil);
BENCHMARK_MATH_FUNCTION_NORM(ScalarWrapper, Ceil);
BENCHMARK_MATH_FUNCTION_NORM(VcScalar, Ceil);
BENCHMARK_MATH_FUNCTION_NORM(VcVector, Ceil);
BENCHMARK_MATH_FUNCTION_NORM(UMESimd, Ceil);

BENCHMARK_MATH_FUNCTION_TRIG(Scalar, Sin);
BENCHMARK_MATH_FUNCTION_TRIG(ScalarWrapper, Sin);
BENCHMARK_MATH_FUNCTION_TRIG(VcScalar, Sin);
BENCHMARK_MATH_FUNCTION_TRIG(VcVector, Sin);
BENCHMARK_MATH_FUNCTION_TRIG(UMESimd, Sin);

BENCHMARK_MATH_FUNCTION_ONE(Scalar, ASin);
BENCHMARK_MATH_FUNCTION_ONE(ScalarWrapper, ASin);
BENCHMARK_MATH_FUNCTION_ONE(VcScalar, ASin);
BENCHMARK_MATH_FUNCTION_ONE(VcVector, ASin);
// BENCHMARK_MATH_FUNCTION_ONE(UMESimd, ASin);

BENCHMARK_MATH_FUNCTION_TRIG(Scalar, Cos);
BENCHMARK_MATH_FUNCTION_TRIG(ScalarWrapper, Cos);
BENCHMARK_MATH_FUNCTION_TRIG(VcScalar, Cos);
BENCHMARK_MATH_FUNCTION_TRIG(VcVector, Cos);
BENCHMARK_MATH_FUNCTION_TRIG(UMESimd, Cos);

BENCHMARK_MATH_FUNCTION_TRIG(Scalar, Tan);
BENCHMARK_MATH_FUNCTION_TRIG(ScalarWrapper, Tan);
BENCHMARK_MATH_FUNCTION_TRIG(VcScalar, Tan);
BENCHMARK_MATH_FUNCTION_TRIG(VcVector, Tan);
BENCHMARK_MATH_FUNCTION_TRIG(UMESimd, Tan);

BENCHMARK_MATH_FUNCTION_NORM(Scalar, ATan);
BENCHMARK_MATH_FUNCTION_NORM(ScalarWrapper, ATan);
BENCHMARK_MATH_FUNCTION_NORM(VcScalar, ATan);
BENCHMARK_MATH_FUNCTION_NORM(VcVector, ATan);
BENCHMARK_MATH_FUNCTION_NORM(UMESimd, ATan);

BENCHMARK_MATH_FUNCTION_HIGH(Scalar, Exp);
BENCHMARK_MATH_FUNCTION_HIGH(ScalarWrapper, Exp);
BENCHMARK_MATH_FUNCTION_HIGH(VcScalar, Exp);
BENCHMARK_MATH_FUNCTION_HIGH(VcVector, Exp);
BENCHMARK_MATH_FUNCTION_HIGH(UMESimd, Exp);

BENCHMARK_MATH_FUNCTION_POS(Scalar, Log);
BENCHMARK_MATH_FUNCTION_POS(ScalarWrapper, Log);
BENCHMARK_MATH_FUNCTION_POS(VcScalar, Log);
BENCHMARK_MATH_FUNCTION_POS(VcVector, Log);
BENCHMARK_MATH_FUNCTION_POS(UMESimd, Log);

BENCHMARK_MATH_FUNCTION_POS(Scalar, Sqrt);
BENCHMARK_MATH_FUNCTION_POS(ScalarWrapper, Sqrt);
BENCHMARK_MATH_FUNCTION_POS(VcScalar, Sqrt);
BENCHMARK_MATH_FUNCTION_POS(VcVector, Sqrt);
BENCHMARK_MATH_FUNCTION_POS(UMESimd, Sqrt);

BENCHMARK_MATH_FUNCTION_NORM(Scalar, Cbrt);
BENCHMARK_MATH_FUNCTION_NORM(ScalarWrapper, Cbrt);
BENCHMARK_MATH_FUNCTION_NORM(VcScalar, Cbrt);
BENCHMARK_MATH_FUNCTION_NORM(VcVector, Cbrt);
BENCHMARK_MATH_FUNCTION_NORM(UMESimd, Cbrt);

BENCHMARK_MATH_FUNCTION_NORM(Scalar, Trunc);
BENCHMARK_MATH_FUNCTION_NORM(ScalarWrapper, Trunc);
BENCHMARK_MATH_FUNCTION_NORM(VcScalar, Trunc);
BENCHMARK_MATH_FUNCTION_NORM(VcVector, Trunc);
BENCHMARK_MATH_FUNCTION_NORM(UMESimd, Trunc);

// Register functions as benchmarks
BENCHMARK(BM_Scalar_Abs);
BENCHMARK(BM_ScalarWrapper_Abs);
BENCHMARK(BM_VcScalar_Abs);
BENCHMARK(BM_VcVector_Abs);
BENCHMARK(BM_UMESimd_Abs);

BENCHMARK(BM_Scalar_Floor);
BENCHMARK(BM_ScalarWrapper_Floor);
BENCHMARK(BM_VcScalar_Floor);
BENCHMARK(BM_VcVector_Floor);
BENCHMARK(BM_UMESimd_Floor);

BENCHMARK(BM_Scalar_Ceil);
BENCHMARK(BM_ScalarWrapper_Ceil);
BENCHMARK(BM_VcScalar_Ceil);
BENCHMARK(BM_VcVector_Ceil);
BENCHMARK(BM_UMESimd_Ceil);

BENCHMARK(BM_Scalar_Sin);
BENCHMARK(BM_ScalarWrapper_Sin);
BENCHMARK(BM_VcScalar_Sin);
BENCHMARK(BM_VcVector_Sin);
BENCHMARK(BM_UMESimd_Sin);

BENCHMARK(BM_Scalar_ASin);
BENCHMARK(BM_ScalarWrapper_ASin);
BENCHMARK(BM_VcScalar_ASin);
BENCHMARK(BM_VcVector_ASin);
// BENCHMARK(BM_UMESimd_ASin);

BENCHMARK(BM_Scalar_Cos);
BENCHMARK(BM_ScalarWrapper_Cos);
BENCHMARK(BM_VcScalar_Cos);
BENCHMARK(BM_VcVector_Cos);
BENCHMARK(BM_UMESimd_Cos);

BENCHMARK(BM_Scalar_Tan);
BENCHMARK(BM_ScalarWrapper_Tan);
BENCHMARK(BM_VcScalar_Tan);
BENCHMARK(BM_VcVector_Tan);
BENCHMARK(BM_UMESimd_Tan);

BENCHMARK(BM_Scalar_ATan);
BENCHMARK(BM_ScalarWrapper_ATan);
BENCHMARK(BM_VcScalar_ATan);
BENCHMARK(BM_VcVector_ATan);
BENCHMARK(BM_UMESimd_ATan);

BENCHMARK(BM_Scalar_Exp);
BENCHMARK(BM_ScalarWrapper_Exp);
BENCHMARK(BM_VcScalar_Exp);
BENCHMARK(BM_VcVector_Exp);
BENCHMARK(BM_UMESimd_Exp);

BENCHMARK(BM_Scalar_Log);
BENCHMARK(BM_ScalarWrapper_Log);
BENCHMARK(BM_VcScalar_Log);
BENCHMARK(BM_VcVector_Log);
BENCHMARK(BM_UMESimd_Log);

BENCHMARK(BM_Scalar_Sqrt);
BENCHMARK(BM_ScalarWrapper_Sqrt);
BENCHMARK(BM_VcScalar_Sqrt);
BENCHMARK(BM_VcVector_Sqrt);
BENCHMARK(BM_UMESimd_Sqrt);

BENCHMARK(BM_Scalar_Cbrt);
BENCHMARK(BM_ScalarWrapper_Cbrt);
BENCHMARK(BM_VcScalar_Cbrt);
BENCHMARK(BM_VcVector_Cbrt);
BENCHMARK(BM_UMESimd_Cbrt);

BENCHMARK(BM_Scalar_Trunc);
BENCHMARK(BM_ScalarWrapper_Trunc);
BENCHMARK(BM_VcScalar_Trunc);
BENCHMARK(BM_VcVector_Trunc);
BENCHMARK(BM_UMESimd_Trunc);

BENCHMARK_MAIN();
