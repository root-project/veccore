#include "test.h"

template <class T> class NumericLimitsTest : public VectorTypeTest<T> {};

TYPED_TEST_SUITE_P(NumericLimitsTest);

TYPED_TEST_P(NumericLimitsTest, Limits) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  using vecCore::Get;
  using vecCore::NumericLimits;
  using vecCore::VectorSize;

  size_t N = VectorSize<Vector_t>();

  EXPECT_TRUE(NumericLimits<Scalar_t>::Min() ==
              Get(NumericLimits<Vector_t>::Min(), 0));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Max() ==
              Get(NumericLimits<Vector_t>::Max(), 0));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Lowest() ==
              Get(NumericLimits<Vector_t>::Lowest(), 0));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Highest() ==
              Get(NumericLimits<Vector_t>::Highest(), 0));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Epsilon() ==
              Get(NumericLimits<Vector_t>::Epsilon(), 0));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Infinity() ==
              Get(NumericLimits<Vector_t>::Infinity(), 0));

  EXPECT_TRUE(NumericLimits<Scalar_t>::Min() ==
              Get(NumericLimits<Vector_t>::Min(), N - 1));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Max() ==
              Get(NumericLimits<Vector_t>::Max(), N - 1));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Lowest() ==
              Get(NumericLimits<Vector_t>::Lowest(), N - 1));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Highest() ==
              Get(NumericLimits<Vector_t>::Highest(), N - 1));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Epsilon() ==
              Get(NumericLimits<Vector_t>::Epsilon(), N - 1));
  EXPECT_TRUE(NumericLimits<Scalar_t>::Infinity() ==
              Get(NumericLimits<Vector_t>::Infinity(), N - 1));
}

REGISTER_TYPED_TEST_SUITE_P(NumericLimitsTest, Limits);

#define TEST_BACKEND_P(name, x)                                                \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, NumericLimitsTest,                      \
                                 FloatTypes<vecCore::backend::x>);

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

TEST_BACKEND(Scalar);
TEST_BACKEND(ScalarWrapper);

#ifdef VECCORE_ENABLE_VC
TEST_BACKEND(VcScalar);
TEST_BACKEND(VcVector);
TEST_BACKEND_P(VcSimdArray, VcSimdArray<4>);
#endif

#ifdef VECCORE_ENABLE_UMESIMD
TEST_BACKEND(UMESimd);
TEST_BACKEND_P(UMESimdArray, UMESimdArray<4>);
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
TEST_BACKEND_P(SIMDScalar, SIMDScalar);
TEST_BACKEND_P(SIMDVector4, SIMDVector<4>);
TEST_BACKEND_P(SIMDVector8, SIMDVector<8>);
TEST_BACKEND_P(SIMDNative, SIMDNative);
#endif
