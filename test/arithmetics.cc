#include "test.h"

template <class T> class ArithmeticsTest : public VectorTypeTest<T> {};

TYPED_TEST_SUITE_P(ArithmeticsTest);

TYPED_TEST_P(ArithmeticsTest, Addition) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(1));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs + rhs)));
}

TYPED_TEST_P(ArithmeticsTest, AdditionAssign) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(1));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  lhs += rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

TYPED_TEST_P(ArithmeticsTest, Subtraction) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(3));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs - rhs)));
}

TYPED_TEST_P(ArithmeticsTest, SubtractionAssign) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(3));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  lhs -= rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

TYPED_TEST_P(ArithmeticsTest, Multiplication) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(2));
  Vector_t rhs(static_cast<Scalar_t>(2));
  Vector_t res(static_cast<Scalar_t>(4));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs * rhs)));
}

TYPED_TEST_P(ArithmeticsTest, MultiplicationAssign) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(2));
  Vector_t rhs(static_cast<Scalar_t>(2));
  Vector_t res(static_cast<Scalar_t>(4));

  lhs *= rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

TYPED_TEST_P(ArithmeticsTest, Division) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(10));
  Vector_t rhs(static_cast<Scalar_t>(5));
  Vector_t res(static_cast<Scalar_t>(2));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs / rhs)));
}

TYPED_TEST_P(ArithmeticsTest, DivisionAssign) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(10));
  Vector_t rhs(static_cast<Scalar_t>(5));
  Vector_t res(static_cast<Scalar_t>(2));

  lhs /= rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

REGISTER_TYPED_TEST_SUITE_P(ArithmeticsTest, Addition, Subtraction,
                            Multiplication, Division, AdditionAssign,
                            SubtractionAssign, MultiplicationAssign,
                            DivisionAssign);

#define TEST_BACKEND_P(name, x)                                                \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, ArithmeticsTest,                        \
                                 VectorTypes<vecCore::backend::x>);

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

#include "backends.h"
