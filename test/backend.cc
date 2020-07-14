
#include <VecCore/VecCore>

#include <type_traits>
#include <gtest/gtest.h>

using namespace testing;

#if defined(GTEST_HAS_TYPED_TEST) && defined(GTEST_HAS_TYPED_TEST_P)

template <class Backend>
using FloatTypes = Types<typename Backend::Float_v, typename Backend::Double_v>;

template <class Backend>
using IntTypes =
    Types<typename Backend::Int16_v, typename Backend::UInt16_v, typename Backend::Int32_v, typename Backend::UInt32_v>;

template <class Backend>
using VectorTypes = Types<typename Backend::Float_v, typename Backend::Double_v, typename Backend::Int16_v,
                          typename Backend::UInt16_v, typename Backend::Int32_v, typename Backend::UInt32_v>;

///////////////////////////////////////////////////////////////////////////////

template <class T>
class VectorTypeTest : public Test {
public:
  using Scalar_t = typename vecCore::Scalar<T>;
  using Vector_t = T;
};

///////////////////////////////////////////////////////////////////////////////

template <class T>
class ConstructorTest : public VectorTypeTest<T> {
};

TYPED_TEST_CASE_P(ConstructorTest);

TYPED_TEST_P(ConstructorTest, Default)
{
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_constructible<Vector_t>::value));
}

TYPED_TEST_P(ConstructorTest, Copy)
{
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_copy_constructible<Vector_t>::value));
}

TYPED_TEST_P(ConstructorTest, Move)
{
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_move_constructible<Vector_t>::value));
}

TYPED_TEST_P(ConstructorTest, FromScalar)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_constructible<Vector_t, Scalar_t>::value));
}

TYPED_TEST_P(ConstructorTest, FromRefToScalar)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE((std::is_constructible<Vector_t, const Scalar_t &>::value));
}

TYPED_TEST_P(ConstructorTest, FromPtrToScalar)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Scalar_t tmp[vecCore::VectorSize<Vector_t>()];
  Scalar_t *addr = &tmp[0];

  for (vecCore::UInt_s i = 0; i < vecCore::VectorSize<Vector_t>(); i++) {
    tmp[i] = static_cast<Scalar_t>(i);
  }

  Vector_t x = vecCore::FromPtr<Vector_t>(addr);

  for (vecCore::UInt_s i = 0; i < vecCore::VectorSize<Vector_t>(); i++)
    EXPECT_TRUE(!vecCore::MaskEmpty(x == Vector_t(Scalar_t(i))));
}

REGISTER_TYPED_TEST_CASE_P(ConstructorTest, Default, Copy, Move, FromScalar, FromRefToScalar, FromPtrToScalar);

///////////////////////////////////////////////////////////////////////////////

template <class T>
class ArithmeticsTest : public VectorTypeTest<T> {
};

TYPED_TEST_CASE_P(ArithmeticsTest);

TYPED_TEST_P(ArithmeticsTest, Addition)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(1));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs + rhs)));
}

TYPED_TEST_P(ArithmeticsTest, AdditionAssign)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(1));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  lhs += rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

TYPED_TEST_P(ArithmeticsTest, Subtraction)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(3));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs - rhs)));
}

TYPED_TEST_P(ArithmeticsTest, SubtractionAssign)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(3));
  Vector_t rhs(static_cast<Scalar_t>(1));
  Vector_t res(static_cast<Scalar_t>(2));

  lhs -= rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

TYPED_TEST_P(ArithmeticsTest, Multiplication)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(2));
  Vector_t rhs(static_cast<Scalar_t>(2));
  Vector_t res(static_cast<Scalar_t>(4));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs * rhs)));
}

TYPED_TEST_P(ArithmeticsTest, MultiplicationAssign)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(2));
  Vector_t rhs(static_cast<Scalar_t>(2));
  Vector_t res(static_cast<Scalar_t>(4));

  lhs *= rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

TYPED_TEST_P(ArithmeticsTest, Division)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(10));
  Vector_t rhs(static_cast<Scalar_t>(5));
  Vector_t res(static_cast<Scalar_t>(2));

  EXPECT_TRUE(vecCore::MaskFull(res == (lhs / rhs)));
}

TYPED_TEST_P(ArithmeticsTest, DivisionAssign)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t lhs(static_cast<Scalar_t>(10));
  Vector_t rhs(static_cast<Scalar_t>(5));
  Vector_t res(static_cast<Scalar_t>(2));

  lhs /= rhs;
  EXPECT_TRUE(vecCore::MaskFull(res == lhs));
}

REGISTER_TYPED_TEST_CASE_P(ArithmeticsTest, Addition, Subtraction, Multiplication, Division, AdditionAssign,
                           SubtractionAssign, MultiplicationAssign, DivisionAssign);

///////////////////////////////////////////////////////////////////////////////

template <class T>
class MaskArithmeticsTest : public VectorTypeTest<T> {
};

TYPED_TEST_CASE_P(MaskArithmeticsTest);

TYPED_TEST_P(MaskArithmeticsTest, AND)
{
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) &  Mask_t(true)));
  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) && Mask_t(true)));

  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(true) &  Mask_t(false)));
  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(true) && Mask_t(false)));
}

TYPED_TEST_P(MaskArithmeticsTest, OR)
{
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) |  Mask_t(true)));
  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) || Mask_t(true)));

  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) |  Mask_t(false)));
  EXPECT_TRUE(vecCore::MaskFull(Mask_t(true) || Mask_t(false)));

  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(false) |  Mask_t(false)));
  EXPECT_TRUE(vecCore::MaskEmpty(Mask_t(false) || Mask_t(false)));
}

TYPED_TEST_P(MaskArithmeticsTest, NEG)
{
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  EXPECT_TRUE(vecCore::MaskEmpty(!Mask_t(true)));
  EXPECT_TRUE(vecCore::MaskFull(!Mask_t(false)));
}

TYPED_TEST_P(MaskArithmeticsTest, ANDAssign)
{
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  Mask_t mask(true);

  mask &= Mask_t(false);

  EXPECT_TRUE(vecCore::MaskEmpty(mask));
}

TYPED_TEST_P(MaskArithmeticsTest, ORAssign)
{
  using Mask_t = typename vecCore::Mask<typename TestFixture::Vector_t>;

  Mask_t mask(false);
  mask |= Mask_t(true);

  EXPECT_TRUE(vecCore::MaskFull(mask));
}

REGISTER_TYPED_TEST_CASE_P(MaskArithmeticsTest, AND, OR, NEG, ANDAssign, ORAssign);

///////////////////////////////////////////////////////////////////////////////

template <class T>
class VectorInterfaceTest : public VectorTypeTest<T> {
};

TYPED_TEST_CASE_P(VectorInterfaceTest);

TYPED_TEST_P(VectorInterfaceTest, VectorSize)
{
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t v;
  Vector_t &vref = v;

  EXPECT_TRUE(vecCore::VectorSize(v) > 0);
  EXPECT_TRUE(vecCore::VectorSize(v) == vecCore::VectorSize(vref));

  EXPECT_TRUE(vecCore::VectorSize<Vector_t>() > 0);
  EXPECT_TRUE(vecCore::VectorSize<Vector_t&>() == vecCore::VectorSize<Vector_t>());
}

TYPED_TEST_P(VectorInterfaceTest, VectorSizeVariable)
{
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t x;

  EXPECT_TRUE(vecCore::VectorSize(x) > 0);
}

TYPED_TEST_P(VectorInterfaceTest, EarlyReturnMaxLength)
{
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t x;

  // short vector, should return early if it is allowed
  EXPECT_EQ(vecCore::EarlyReturnAllowed(),
   vecCore::EarlyReturnMaxLength(x, vecCore::VectorSize<Vector_t>()));

  // long vector, should not return early, even if it is allowed
  EXPECT_FALSE(vecCore::EarlyReturnMaxLength(x, vecCore::VectorSize<Vector_t>()/2));
}

TYPED_TEST_P(VectorInterfaceTest, LoadStore)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();
  constexpr auto N   = 2 * kVS;

  alignas(64) Scalar_t input[N];
  alignas(64) Scalar_t output[N];

  // init input; output
  for (vecCore::UInt_s i = 0; i < N; ++i) {
    input[i]  = i;
    output[i] = 0;
  }

  // transfer to output via Load/Store sequence
  for (vecCore::UInt_s i = 0; i < 2; ++i) {
    Vector_t tmp;
    vecCore::Load<Vector_t>(tmp, &input[i * kVS]);
    tmp = Scalar_t(2) * tmp;
    vecCore::Store<Vector_t>(tmp, &output[i * kVS]);
  }

  // assert input == output
  for (vecCore::UInt_s i = 0; i < N; ++i)
    EXPECT_EQ(2 * input[i], output[i]);
}

TYPED_TEST_P(VectorInterfaceTest, StoreToPtr)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();
  constexpr auto N   = 2 * kVS;

  Scalar_t input[N];
  Scalar_t output[N];

  // init input; output
  for (vecCore::UInt_s i = 0; i < N; ++i) {
    input[i]  = 2 * i;
    output[i] = 0;
  }

  // transfer to output via FromPtr; Store sequence
  for (vecCore::UInt_s i = 0; i < 2; ++i) {
    Vector_t tmp(vecCore::FromPtr<Vector_t>(&input[i * kVS]));
    vecCore::Store<Vector_t>(tmp, &output[i * kVS]);
  }

  // assert input == output
  for (vecCore::UInt_s i = 0; i < N; ++i)
    EXPECT_EQ(input[i], output[i]);
}

TYPED_TEST_P(VectorInterfaceTest, StoreMaskToPtr)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;
  using Mask_t   = typename vecCore::Mask<Vector_t>;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();
  constexpr auto N   = 2 * kVS;

  Scalar_t input[N];
  bool output[N];

  // init input; output
  for (vecCore::UInt_s i = 0; i < N; ++i) {
    input[i]  = 2 * i;
    output[i] = false;
  }

  for (vecCore::UInt_s i = 0; i < 2; ++i) {
    Vector_t tmp(vecCore::FromPtr<Vector_t>(&input[i * kVS]));
    Mask_t m = tmp > Vector_t(1);
    vecCore::StoreMask(m, &output[i * kVS]);
  }

  for (vecCore::UInt_s i = 0; i < N; ++i)
    EXPECT_EQ(output[i], input[i] > Scalar_t(1));
}

TYPED_TEST_P(VectorInterfaceTest, VectorLaneRead)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  Scalar_t input[kVS];

  for (vecCore::UInt_s i = 0; i < kVS; ++i) {
    input[i] = i;
  }

  Vector_t tmp(vecCore::FromPtr<Vector_t>(&input[0]));

  for (vecCore::UInt_s i = 0; i < kVS; ++i)
    EXPECT_EQ(input[i], vecCore::Get<Vector_t>(tmp, i));
}

TYPED_TEST_P(VectorInterfaceTest, VectorLaneWrite)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  Scalar_t input[kVS];

  for (vecCore::UInt_s i = 0; i < kVS; ++i) {
    input[i] = i;
  }

  Vector_t tmp(vecCore::FromPtr<Vector_t>(&input[0]));

  for (vecCore::UInt_s i = 0; i < kVS; ++i)
    vecCore::Set(tmp, i, Scalar_t(10));

  for (vecCore::UInt_s i = 0; i < kVS; ++i)
    EXPECT_EQ(Scalar_t(10), vecCore::Get<Vector_t>(tmp, i));
}

TYPED_TEST_P(VectorInterfaceTest, MaskLaneRead)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  vecCore::Mask<Vector_t> fmask(false), tmask(true), mmask;

  constexpr size_t kVS = vecCore::VectorSize<Vector_t>();

  Vector_t input;

  for (size_t i = 0; i < kVS; ++i)
    vecCore::Set(input, i, (i % 2 == 0) ? Scalar_t(1) : Scalar_t(0));

  mmask = input > Vector_t(Scalar_t(0));

  for (size_t i = 0; i < kVS; ++i) {
    EXPECT_EQ(vecCore::Get(tmask, i), true);
    EXPECT_EQ(vecCore::Get(fmask, i), false);
    EXPECT_EQ(vecCore::Get(mmask, i), i % 2 == 0);
  }
}

TYPED_TEST_P(VectorInterfaceTest, MaskLaneWrite)
{
  using Vector_t = typename TestFixture::Vector_t;

  vecCore::Mask<Vector_t> mask(false);

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  // check for all false

  for (size_t i = 0; i < kVS; ++i)
    EXPECT_EQ(vecCore::Get(mask, i), false);

  // check for all true

  for (size_t i = 0; i < kVS; ++i)
    vecCore::Set(mask, i, true);

  for (size_t i = 0; i < kVS; ++i)
    EXPECT_EQ(vecCore::Get(mask, i), true);

  // check for interleaving true/false

  for (size_t i = 0; i < kVS; ++i)
    vecCore::Set(mask, i, i % 2 == 0);

  for (size_t i = 0; i < kVS; ++i)
    EXPECT_EQ(vecCore::Get(mask, i), i % 2 == 0);
}

TYPED_TEST_P(VectorInterfaceTest, ReduceAdd)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t x(1);

  EXPECT_EQ(Scalar_t(vecCore::VectorSize<Vector_t>()), vecCore::ReduceAdd(x));
}

TYPED_TEST_P(VectorInterfaceTest, ReduceMinMax)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t v;

  for (size_t i = 0; i < vecCore::VectorSize<Vector_t>(); ++i)
     vecCore::Set(v, i, Scalar_t(i+1));

  EXPECT_EQ(Scalar_t(1), vecCore::ReduceMin(v));
  EXPECT_EQ(Scalar_t(vecCore::VectorSize<Vector_t>()), vecCore::ReduceMax(v));
}

TYPED_TEST_P(VectorInterfaceTest, Convert)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;
  using Index_v  = typename vecCore::Index<Vector_t>;
  using Index_t  = typename vecCore::Scalar<Index_v>;

  Index_v idx;
  Vector_t v;

  for (size_t i = 0; i < vecCore::VectorSize<Vector_t>(); ++i) {
     vecCore::Set(v, i, Scalar_t(i));
     vecCore::Set(idx, i, Index_t(i));
   }

  idx = vecCore::Convert<Index_v, Vector_t>(v);

  EXPECT_TRUE(vecCore::MaskFull(vecCore::Convert<Vector_t, Index_v>(idx) == v));
}

TYPED_TEST_P(VectorInterfaceTest, Gather)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;
  using Index_v  = typename vecCore::Index<Vector_t>;
  using Index_t  = typename vecCore::Scalar<Index_v>;

  constexpr size_t N = vecCore::VectorSize<Vector_t>();

  Scalar_t input[N * N];
  for (vecCore::UInt_s i = 0; i < N * N; ++i) {
    input[i] = i;
  }

  Index_v idx;
  for (vecCore::UInt_s i = 0; i < N; ++i)
    vecCore::Set(idx, i, Index_t(i));

  for (vecCore::UInt_s i = 1; i <= N; ++i) {
    Index_v iidx = Index_v(i) * idx;

    Vector_t x = vecCore::Gather<Vector_t>(input, iidx);

    for (vecCore::UInt_s j = 0; j < N; ++j)
      EXPECT_TRUE(vecCore::Get(x, j) == input[i * j]);
  }
}

TYPED_TEST_P(VectorInterfaceTest, Scatter)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;
  using Index_v  = typename vecCore::Index<Vector_t>;
  using Index_t  = typename vecCore::Scalar<Index_v>;

  constexpr size_t N = vecCore::VectorSize<Vector_t>();

  Index_t index[N];
  Scalar_t input[N], output[N];

  for (vecCore::UInt_s i = 0; i < N; ++i) {
    input[i] = i;
    index[i] = i;
  }

  Index_v idx(vecCore::FromPtr<Index_v>(&index[0]));

  Vector_t x = vecCore::FromPtr<Vector_t>(&input[0]);

  vecCore::Scatter<Vector_t>(x, &output[0], idx);

  Vector_t y = vecCore::FromPtr<Vector_t>(&output[0]);

  EXPECT_TRUE(vecCore::MaskFull(x == y));
}

REGISTER_TYPED_TEST_CASE_P(VectorInterfaceTest,
                           EarlyReturnMaxLength,
                           VectorSize, VectorSizeVariable,
                           VectorLaneRead, VectorLaneWrite,
                           MaskLaneRead, MaskLaneWrite,
                           LoadStore, StoreToPtr, StoreMaskToPtr,
                           ReduceAdd, ReduceMinMax,
                           Convert, Gather, Scatter);

///////////////////////////////////////////////////////////////////////////////

template <class T>
class VectorMaskTest : public VectorTypeTest<T> {
};

TYPED_TEST_CASE_P(VectorMaskTest);

TYPED_TEST_P(VectorMaskTest, Constructor)
{
  using Vector_t = typename TestFixture::Vector_t;

  EXPECT_TRUE(std::is_constructible<vecCore::Mask<Vector_t>>::value);
  EXPECT_TRUE(std::is_copy_constructible<vecCore::Mask<Vector_t>>::value);
  EXPECT_TRUE(std::is_move_constructible<vecCore::Mask<Vector_t>>::value);
}

TYPED_TEST_P(VectorMaskTest, MaskFull)
{
  using Vector_t = typename TestFixture::Vector_t;

  vecCore::Mask<Vector_t> tmask(true), fmask(false);

  EXPECT_TRUE(vecCore::MaskFull(tmask));
  EXPECT_TRUE(vecCore::MaskFull(!fmask));
}

TYPED_TEST_P(VectorMaskTest, MaskEmpty)
{
  using Vector_t = typename TestFixture::Vector_t;

  vecCore::Mask<Vector_t> tmask(true), fmask(false);

  EXPECT_TRUE(vecCore::MaskEmpty(fmask));
  EXPECT_TRUE(vecCore::MaskEmpty(!tmask));
}

TYPED_TEST_P(VectorMaskTest, MaskAssign)
{
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

TYPED_TEST_P(VectorMaskTest, MaskAssign2)
{
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  Scalar_t input[kVS];
  Scalar_t output[kVS];

  for (vecCore::UInt_s i = 0; i < kVS; ++i) {
    input[i]  = (i % 2 == 0) ? i : -i;
    output[i] = (input[i] > 0) ? input[i] : 0;
  }

  Vector_t c(vecCore::FromPtr<Vector_t>(&input[0])), dest(Scalar_t(0));
  vecCore::MaskedAssign(dest, c > Vector_t(Scalar_t(0)), c);

  for (vecCore::UInt_s i = 0; i < kVS; ++i)
    EXPECT_EQ(vecCore::Get<Vector_t>(dest, i), output[i]);
}

TYPED_TEST_P(VectorMaskTest, Blend)
{
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t a(Scalar_t(0)), b(1);

  a = vecCore::Blend(a > b, a, b);
}

REGISTER_TYPED_TEST_CASE_P(VectorMaskTest, Constructor, MaskFull, MaskEmpty, MaskAssign, MaskAssign2, Blend);

#define TEST_BACKEND_P(name, x)                                                               \
  INSTANTIATE_TYPED_TEST_CASE_P(name, ConstructorTest, VectorTypes<vecCore::backend::x>);     \
  INSTANTIATE_TYPED_TEST_CASE_P(name, ArithmeticsTest, VectorTypes<vecCore::backend::x>);     \
  INSTANTIATE_TYPED_TEST_CASE_P(name, MaskArithmeticsTest, VectorTypes<vecCore::backend::x>); \
  INSTANTIATE_TYPED_TEST_CASE_P(name, VectorMaskTest, VectorTypes<vecCore::backend::x>);      \
  INSTANTIATE_TYPED_TEST_CASE_P(name, VectorInterfaceTest, VectorTypes<vecCore::backend::x>)

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

///////////////////////////////////////////////////////////////////////////////

TEST_BACKEND(Scalar);
TEST_BACKEND(ScalarWrapper);

#ifdef VECCORE_ENABLE_VC
TEST_BACKEND(VcScalar);
TEST_BACKEND(VcVector);
TEST_BACKEND_P(VcSimdArray, VcSimdArray<16>);
#endif

#ifdef VECCORE_ENABLE_UMESIMD
TEST_BACKEND(UMESimd);
TEST_BACKEND_P(UMESimdArray, UMESimdArray<16>);
#endif

#else // if !GTEST_HAS_TYPED_TEST
TEST(DummyTest, TypedTestsAreNotSupportedOnThisPlatform)
{
}
#endif

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
