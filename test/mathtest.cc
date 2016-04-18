#include <VecCore/VecCore>

#include <type_traits>
#include <gtest/gtest.h>

using namespace testing;

#if defined(GTEST_HAS_TYPED_TEST) && defined(GTEST_HAS_TYPED_TEST_P)

template <class Backend> using FloatTypes = Types<typename Backend::Float_v, typename Backend::Double_v>;

template <class Backend> using VectorTypes = Types<typename Backend::Float_v, typename Backend::Double_v>;

///////////////////////////////////////////////////////////////////////////////

template <class T> class VectorTypeTest : public Test {
public:
  using Scalar_t = typename vecCore::ScalarType<T>::Type;
  using Vector_t = T;
};

///////////////////////////////////////////////////////////////////////////////

template <class T> class BackendMathTest : public VectorTypeTest<T> {};

TYPED_TEST_CASE_P(BackendMathTest);

TYPED_TEST_P(BackendMathTest, SinTrivial) {
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE(vecCore::MaskFull(vecCore::math::Sin(Vector_t(0.)) == Vector_t(0.)));
}

// a standard test for many functions (without strict input range restriction)
#define TEST_MATHFUNCTIONS_POSITIVEREALS_P(name, func, stdfunc)                            \
  TYPED_TEST_P(name, func) {                                                               \
    using Scalar_t = typename TestFixture::Scalar_t;                                       \
    using Vector_t = typename TestFixture::Vector_t;                                       \
                                                                                           \
    auto kVS = vecCore::VectorSize<Vector_t>();                                            \
    size_t N = 2 * kVS;                                                                    \
    Scalar_t input[N];                                                                     \
    Scalar_t output[N];                                                                    \
                                                                                           \
    for (size_t i = 0; i < N; ++i) {                                                       \
      input[i] = i * i;                                                                    \
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

TEST_MATHFUNCTIONS_POSITIVEREALS_P(BackendMathTest, Exp, exp);
TEST_MATHFUNCTIONS_POSITIVEREALS_P(BackendMathTest, Cos, cos);
TEST_MATHFUNCTIONS_POSITIVEREALS_P(BackendMathTest, Sin, sin);
TEST_MATHFUNCTIONS_POSITIVEREALS_P(BackendMathTest, Sqrt, sqrt);
TEST_MATHFUNCTIONS_POSITIVEREALS_P(BackendMathTest, Log, log);

// Tan not available in Vc:
// TEST_MATHFUNCTIONS_POSITIVEREALS_P(BackendMathTest, Tan, tan);

// TODO: add further tests

REGISTER_TYPED_TEST_CASE_P(BackendMathTest, SinTrivial, Sqrt, Exp, Cos, Sin, Log);

#define TEST_BACKEND_P(name, x)                                                            \
  INSTANTIATE_TYPED_TEST_CASE_P(name, BackendMathTest, VectorTypes<vecCore::backend::x>);  \

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
