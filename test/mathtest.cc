#include <VecCore/VecCore>

#include <type_traits>
#include <gtest/gtest.h>

using namespace testing;

#if defined(GTEST_HAS_TYPED_TEST) && defined(GTEST_HAS_TYPED_TEST_P)

template <class Backend> using FloatTypes = Types<typename Backend::Float_v, typename Backend::Double_v>;

///////////////////////////////////////////////////////////////////////////////

template <class T> class VectorTypeTest : public Test {
public:
  using Scalar_t = typename vecCore::ScalarType<T>::Type;
  using Vector_t = T;
};

///////////////////////////////////////////////////////////////////////////////

template <class T> class MathFunctions : public VectorTypeTest<T> {};

TYPED_TEST_CASE_P(MathFunctions);

double uniform_random(double a, double b)
{
	return a + (b-a) * drand48();
}

#define TEST_MATH_FUNCTION_RANGE(func, stdfunc, a, b)                                      \
  TYPED_TEST_P(MathFunctions, func) {                                                      \
    using Scalar_t = typename TestFixture::Scalar_t;                                       \
    using Vector_t = typename TestFixture::Vector_t;                                       \
                                                                                           \
    auto kVS = vecCore::VectorSize<Vector_t>();                                            \
    size_t N = 2 * kVS;                                                                    \
    Scalar_t input[N];                                                                     \
    Scalar_t output[N];                                                                    \
                                                                                           \
    for (size_t i = 0; i < N; ++i) {                                                       \
      input[i] = static_cast<Scalar_t>(uniform_random(a, b));                              \
    }                                                                                      \
                                                                                           \
    for (size_t j = 0; j < N; j += kVS) {                                                  \
      Vector_t x(vecCore::FromPtr<Vector_t>(&input[j]));                                   \
      Vector_t y = vecCore::math::func(x);                                                 \
      vecCore::Store<Vector_t>(y, &output[j]);                                             \
    }                                                                                      \
                                                                                           \
    for (size_t i = 0; i < N; ++i) {                                                       \
      EXPECT_FLOAT_EQ(output[i], std::stdfunc(input[i]));                                  \
    }                                                                                      \
  }                                                                                        \

#define TEST_MATH_FUNCTION(F, f) TEST_MATH_FUNCTION_RANGE(F, f, FLT_MIN, FLT_MAX)
#define TEST_MATH_FUNCTION_ONE(F, f) TEST_MATH_FUNCTION_RANGE(F, f, -1.0, 1.0)
#define TEST_MATH_FUNCTION_NEG(F, f) TEST_MATH_FUNCTION_RANGE(F, f, -1.0, 0.0)
#define TEST_MATH_FUNCTION_POS(F, f) TEST_MATH_FUNCTION_RANGE(F, f,  0.0, 1.0)

#define TEST_MATH_FUNCTION_RANGE_2(func, stdfunc, a, b, c, d)                              \
  TYPED_TEST_P(MathFunctions, func) {                                                      \
    using Scalar_t = typename TestFixture::Scalar_t;                                       \
    using Vector_t = typename TestFixture::Vector_t;                                       \
                                                                                           \
    auto kVS = vecCore::VectorSize<Vector_t>();                                            \
    size_t N = 2 * kVS;                                                                    \
    Scalar_t input1[N];                                                                    \
    Scalar_t input2[N];                                                                    \
    Scalar_t output[N];                                                                    \
                                                                                           \
    for (size_t i = 0; i < N; ++i) {                                                       \
      input1[i] = static_cast<Scalar_t>(uniform_random(a, b));                             \
      input2[i] = static_cast<Scalar_t>(uniform_random(c, d));                             \
    }                                                                                      \
                                                                                           \
    for (size_t j = 0; j < N; j += kVS) {                                                  \
      Vector_t x(vecCore::FromPtr<Vector_t>(&input1[j]));                                  \
      Vector_t y(vecCore::FromPtr<Vector_t>(&input2[j]));                                  \
      Vector_t r = vecCore::math::func(x, y);                                              \
      vecCore::Store<Vector_t>(r, &output[j]);                                             \
    }                                                                                      \
                                                                                           \
    for (size_t i = 0; i < N; ++i) {                                                       \
      EXPECT_FLOAT_EQ(output[i], std::stdfunc(input1[i], input2[i]));                      \
    }                                                                                      \
  }                                                                                        \

#define TEST_MATH_FUNCTION_2(F, f) TEST_MATH_FUNCTION_RANGE_2(F, f, FLT_MIN, FLT_MAX, FLT_MIN, FLT_MAX)

// commented functions are not yet implemented in Vc, need to be implemented in VecCore

TEST_MATH_FUNCTION(Abs, abs);
TEST_MATH_FUNCTION(Floor, floor);
TEST_MATH_FUNCTION(Ceil, ceil);
TEST_MATH_FUNCTION(Sin, sin);
TEST_MATH_FUNCTION_ONE(Asin, asin);
// TEST_MATH_FUNCTION(Sinh, sinh);
// TEST_MATH_FUNCTION(Asinh, asinh);
TEST_MATH_FUNCTION(Cos, cos);
// TEST_MATH_FUNCTION(Acos, acos);
// TEST_MATH_FUNCTION(Cosh, cosh);
// TEST_MATH_FUNCTION(Acosh, acosh);
TEST_MATH_FUNCTION(Tan, tan);
TEST_MATH_FUNCTION(Atan, atan);
TEST_MATH_FUNCTION(Exp, exp);
TEST_MATH_FUNCTION_POS(Log, log);
TEST_MATH_FUNCTION_POS(Sqrt, sqrt);
// TEST_MATH_FUNCTION(Cbrt, cbrt);

TEST_MATH_FUNCTION_2(Atan2, atan2);
TEST_MATH_FUNCTION_2(Pow, pow);

REGISTER_TYPED_TEST_CASE_P(MathFunctions,
  Abs, Floor, Ceil, Sin, Asin, Cos, Tan, Atan, Exp, Log, Sqrt, Atan2, Pow);
//  Abs, Floor, Ceil, Sin, Asin, Sinh, Asihn, Cos, Acos, Cosh, Acosh, Tan, Atan, Exp, Log, Sqrt, Cbrt);

#define TEST_BACKEND_P(name, x)                                                            \
  INSTANTIATE_TYPED_TEST_CASE_P(name, MathFunctions, FloatTypes<vecCore::backend::x>);

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

///////////////////////////////////////////////////////////////////////////////

TEST_BACKEND(Scalar);
TEST_BACKEND(ScalarWrapper);

#ifdef VECCORE_ENABLE_VC
TEST_BACKEND(VcScalar);
TEST_BACKEND(VcVector);
TEST_BACKEND_P(VcSimdArray, VcSimdArray<16>);
#endif

#else // if !GTEST_HAS_TYPED_TEST
TEST(DummyTest, TypedTestsAreNotSupportedOnThisPlatform) {}
#endif

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
