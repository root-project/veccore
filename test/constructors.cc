#include "test.h"

template <class T> class ConstructorTest : public VectorTypeTest<T> {};

TYPED_TEST_SUITE_P(ConstructorTest);

TYPED_TEST_P(ConstructorTest, Default) {
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_constructible<Vector_t>::value));
}

TYPED_TEST_P(ConstructorTest, Copy) {
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_copy_constructible<Vector_t>::value));
}

TYPED_TEST_P(ConstructorTest, Move) {
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_move_constructible<Vector_t>::value));
}

TYPED_TEST_P(ConstructorTest, FromScalar) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_constructible<Vector_t, Scalar_t>::value));
}

TYPED_TEST_P(ConstructorTest, FromRefToScalar) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_constructible<Vector_t, const Scalar_t &>::value));
}

TYPED_TEST_P(ConstructorTest, FromPtrToScalar) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Scalar_t tmp[vecCore::VectorSize<Vector_t>()];
  Scalar_t *addr = &tmp[0];

  for (size_t i = 0; i < vecCore::VectorSize<Vector_t>(); i++) {
    tmp[i] = static_cast<Scalar_t>(i);
  }

  Vector_t x = vecCore::Load<Vector_t>(addr);

  for (size_t i = 0; i < vecCore::VectorSize<Vector_t>(); i++)
    EXPECT_TRUE(!vecCore::MaskEmpty(x == Vector_t(Scalar_t(i))));
}

REGISTER_TYPED_TEST_SUITE_P(ConstructorTest, Default, Copy, Move, FromScalar,
                            FromRefToScalar, FromPtrToScalar);

#define TEST_BACKEND_P(name, x)                                                \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, ConstructorTest,                        \
                                 VectorTypes<vecCore::backend::x>)

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

#include "backends.h"
