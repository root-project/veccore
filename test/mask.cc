#include "test.h"

template <class T> class MaskTest : public VectorTypeTest<T> {};

TYPED_TEST_SUITE_P(MaskTest);

TYPED_TEST_P(MaskTest, Constructor) {
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE(std::is_constructible<vecCore::Mask<Vector_t>>::value);
  EXPECT_TRUE(std::is_copy_constructible<vecCore::Mask<Vector_t>>::value);
  EXPECT_TRUE(std::is_move_constructible<vecCore::Mask<Vector_t>>::value);
}

TYPED_TEST_P(MaskTest, MaskFull) {
  using Vector_t = typename TestFixture::Vector_t;

  vecCore::Mask<Vector_t> tmask(true), fmask(false);

  EXPECT_TRUE(vecCore::MaskFull(tmask));
  EXPECT_TRUE(vecCore::MaskFull(!fmask));
}

TYPED_TEST_P(MaskTest, MaskEmpty) {
  using Vector_t = typename TestFixture::Vector_t;

  vecCore::Mask<Vector_t> tmask(true), fmask(false);

  EXPECT_TRUE(vecCore::MaskEmpty(fmask));
  EXPECT_TRUE(vecCore::MaskEmpty(!tmask));
}

TYPED_TEST_P(MaskTest, MaskAssign) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  // casting 0 with Scalar_t is necessary here
  // as 0 can be interpreted as the null-pointer
  // which leads to ambiguities with some vector types constructors
  // (For example the float instantiation of UME::SIMD vector has 'only'
  //  constructor taking a float and another taking a float* both requiring
  //  one conversion from an integer '0'.)
  Vector_t a(Scalar_t(0)), b(1);

  vecCore::MaskedAssign(a, a > b, b);
  EXPECT_TRUE(vecCore::MaskFull(a == Vector_t(Scalar_t(0))));

  vecCore::MaskedAssign(a, b > a, b);
  EXPECT_TRUE(vecCore::MaskFull(a == b));
}

TYPED_TEST_P(MaskTest, MaskAssign2) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  Scalar_t input[kVS];
  Scalar_t output[kVS];

  for (size_t i = 0; i < kVS; ++i) {
    input[i] = (i % 2 == 0) ? Scalar_t(1) : Scalar_t(0);
    output[i] = (input[i] > 0) ? input[i] : 0;
  }

  Vector_t c(vecCore::Load<Vector_t>(&input[0])), dest(Scalar_t(0));
  vecCore::MaskedAssign(dest, c > Vector_t(Scalar_t(0)), c);

  for (size_t i = 0; i < kVS; ++i)
    EXPECT_EQ(vecCore::Get<Vector_t>(dest, i), output[i]);
}

TYPED_TEST_P(MaskTest, Blend) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t a(Scalar_t(0)), b(1);

  a = vecCore::Blend(a > b, a, b);
}

TYPED_TEST_P(MaskTest, AND) {
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) & Mask_t(true)));
  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) && Mask_t(true)));

  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(true) & Mask_t(false)));
  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(true) && Mask_t(false)));
}

TYPED_TEST_P(MaskTest, OR) {
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) | Mask_t(true)));
  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) || Mask_t(true)));

  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) | Mask_t(false)));
  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) || Mask_t(false)));

  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(false) | Mask_t(false)));
  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(false) || Mask_t(false)));
}

TYPED_TEST_P(MaskTest, NEG) {
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  EXPECT_TRUE(vecCore::MaskEmpty(!Mask_t(true)));
  EXPECT_TRUE(vecCore::MaskFull(!Mask_t(false)));
}

TYPED_TEST_P(MaskTest, ANDAssign) {
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  Mask_t mask(true);

  mask &= Mask_t(false);

  EXPECT_TRUE(vecCore::MaskEmpty(mask));
}

TYPED_TEST_P(MaskTest, ORAssign) {
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  Mask_t mask(false);
  mask |= Mask_t(true);

  EXPECT_TRUE(vecCore::MaskFull(mask));
}

REGISTER_TYPED_TEST_SUITE_P(MaskTest, Constructor, MaskFull, MaskEmpty,
                            MaskAssign, MaskAssign2, Blend, AND, OR, NEG,
                            ANDAssign, ORAssign);

#define TEST_BACKEND_P(name, x)                                                \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, MaskTest,                               \
                                 VectorTypes<vecCore::backend::x>);

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

#include "backends.h"
