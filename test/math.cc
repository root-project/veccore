#include <VecCore/VecCore>

#include <type_traits>
#include <gtest/gtest.h>

using namespace testing;

template <class Backend>
using FloatTypes = Types<typename Backend::Float_v, typename Backend::Double_v>;

///////////////////////////////////////////////////////////////////////////////

template <class T>
class VectorTypeTest : public Test {
public:
  using Scalar_t = typename vecCore::ScalarType<T>::Type;
  using Vector_t = T;
};

///////////////////////////////////////////////////////////////////////////////

template <class T>
class MathFunctions : public VectorTypeTest<T> {
};

TYPED_TEST_CASE_P(MathFunctions);

double uniform_random(double a, double b)
{
  return a + (b - a) * drand48();
}

#define TEST_MATH_FUNCTION_RANGE(func, stdfunc, a, b)                 \
  TYPED_TEST_P(MathFunctions, func)                                   \
  {                                                                   \
    using Scalar_t = typename TestFixture::Scalar_t;                  \
    using Vector_t = typename TestFixture::Vector_t;                  \
                                                                      \
    auto kVS = vecCore::VectorSize<Vector_t>();                       \
    size_t N = 16384;                                                 \
    Scalar_t input[N]  __attribute__((aligned(VECCORE_SIMD_ALIGN)));  \
    Scalar_t output[N] __attribute__((aligned(VECCORE_SIMD_ALIGN)));  \
                                                                      \
    for (size_t i = 0; i < N; ++i)                                    \
      input[i] = static_cast<Scalar_t>(uniform_random(a, b));         \
                                                                      \
    for (size_t j = 0; j < N; j += kVS) {                             \
      Vector_t x;                                                     \
      vecCore::Load(x, &input[j]);                                    \
      vecCore::Store<Vector_t>(vecCore::math::func(x), &output[j]);   \
    }                                                                 \
                                                                      \
    for (size_t i = 0; i < N; ++i) {                                  \
      EXPECT_FLOAT_EQ(std::stdfunc(input[i]), output[i]);             \
    }                                                                 \
  }

#define TEST_MATH_FUNCTION_RANGE_2(func, stdfunc, a, b, c, d)         \
  TYPED_TEST_P(MathFunctions, func)                                   \
  {                                                                   \
    using Scalar_t = typename TestFixture::Scalar_t;                  \
    using Vector_t = typename TestFixture::Vector_t;                  \
                                                                      \
    auto kVS = vecCore::VectorSize<Vector_t>();                       \
    size_t N = 16384;                                                 \
    Scalar_t input1[N] __attribute__((aligned(VECCORE_SIMD_ALIGN)));  \
    Scalar_t input2[N] __attribute__((aligned(VECCORE_SIMD_ALIGN)));  \
    Scalar_t output[N] __attribute__((aligned(VECCORE_SIMD_ALIGN)));  \
                                                                      \
    for (size_t i = 0; i < N; ++i) {                                  \
      input1[i] = static_cast<Scalar_t>(uniform_random(a, b));        \
      input2[i] = static_cast<Scalar_t>(uniform_random(c, d));        \
    }                                                                 \
                                                                      \
    for (size_t j = 0; j < N; j += kVS) {                             \
      Vector_t x, y;                                                  \
      vecCore::Load(x, &input1[j]);                                   \
      vecCore::Load(y, &input2[j]);                                   \
      vecCore::Store<Vector_t>(vecCore::math::func(x,y), &output[j]); \
    }                                                                 \
                                                                      \
    for (size_t i = 0; i < N; ++i) {                                  \
      EXPECT_FLOAT_EQ(output[i], std::stdfunc(input1[i], input2[i])); \
    }                                                                 \
  }


#define TEST_MATH_FUNCTION(F, f) TEST_MATH_FUNCTION_RANGE(F, f, -FLT_MAX, FLT_MAX)
#define TEST_MATH_FUNCTION_ONE(F, f) TEST_MATH_FUNCTION_RANGE(F, f, -1.0, 1.0)
#define TEST_MATH_FUNCTION_NEG(F, f) TEST_MATH_FUNCTION_RANGE(F, f, -1.0, 0.0)
#define TEST_MATH_FUNCTION_POS(F, f) TEST_MATH_FUNCTION_RANGE(F, f, FLT_MIN, FLT_MAX)
#define TEST_MATH_FUNCTION_TRIG(F, f) TEST_MATH_FUNCTION_RANGE(F, f, 0.0, 2.0 * M_PI)

#define TEST_MATH_FUNCTION_2(F, f) TEST_MATH_FUNCTION_RANGE_2(F, f, -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX)

TEST_MATH_FUNCTION(Abs, abs);
TEST_MATH_FUNCTION(Floor, floor);
TEST_MATH_FUNCTION(Ceil, ceil);

TEST_MATH_FUNCTION_TRIG(Sin, sin);
TEST_MATH_FUNCTION_TRIG(Cos, cos);
TEST_MATH_FUNCTION_TRIG(Tan, tan);
TEST_MATH_FUNCTION_ONE(ASin, asin);
TEST_MATH_FUNCTION_ONE(ACos, acos);
TEST_MATH_FUNCTION(ATan, atan);

TEST_MATH_FUNCTION(Sinh, sinh);
TEST_MATH_FUNCTION(Cosh, cosh);
TEST_MATH_FUNCTION(Tanh, tanh);
TEST_MATH_FUNCTION(ASinh, asinh);
TEST_MATH_FUNCTION_RANGE(ACosh, acosh,  1., 100.);
TEST_MATH_FUNCTION_ONE(ATanh, atanh);

TEST_MATH_FUNCTION(Exp, exp);
TEST_MATH_FUNCTION_POS(Log, log);
TEST_MATH_FUNCTION_POS(Sqrt, sqrt);
TEST_MATH_FUNCTION_POS(TGamma, tgamma);
TEST_MATH_FUNCTION(Cbrt, cbrt);
TEST_MATH_FUNCTION(Trunc, trunc);

TEST_MATH_FUNCTION_2(ATan2, atan2);
TEST_MATH_FUNCTION_2(CopySign, copysign);

TEST_MATH_FUNCTION_RANGE_2(Pow, pow, FLT_MIN, FLT_MAX, 1.0, 10.0);

REGISTER_TYPED_TEST_CASE_P(MathFunctions,
   Abs, CopySign, Floor, Ceil,
   Sin, Cos, Tan, ASin, ACos, ATan, ATan2,
   Sinh, Cosh, Tanh, ASinh, ACosh, ATanh,
   Exp, Log, TGamma, Sqrt, Cbrt, Pow, Trunc);

#define TEST_BACKEND_P(name, x) \
  INSTANTIATE_TYPED_TEST_CASE_P(name, MathFunctions, FloatTypes<vecCore::backend::x>);

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

///////////////////////////////////////////////////////////////////////////////

TEST_BACKEND(Scalar);
TEST_BACKEND(ScalarWrapper);

#ifdef VECCORE_ENABLE_UMESIMD
TEST_BACKEND(UMESimd);
TEST_BACKEND_P(UMESimdArray, UMESimdArray<16>);
#endif

#ifdef VECCORE_ENABLE_VC
TEST_BACKEND(VcScalar);
TEST_BACKEND(VcVector);
TEST_BACKEND_P(VcSimdArray, VcSimdArray<16>);
#endif

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
