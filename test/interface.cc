#include "test.h"

template <class T> class VectorInterfaceTest : public VectorTypeTest<T> {};

TYPED_TEST_SUITE_P(VectorInterfaceTest);

TYPED_TEST_P(VectorInterfaceTest, VectorSize) {
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t v;
  Vector_t &vref = v;

  EXPECT_TRUE(vecCore::VectorSize(v) > 0);
  EXPECT_TRUE(vecCore::VectorSize(v) == vecCore::VectorSize(vref));

  EXPECT_TRUE(vecCore::VectorSize<Vector_t>() > 0);
  EXPECT_TRUE(vecCore::VectorSize<Vector_t &>() ==
              vecCore::VectorSize<Vector_t>());
}

TYPED_TEST_P(VectorInterfaceTest, VectorSizeVariable) {
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t x;

  EXPECT_TRUE(vecCore::VectorSize(x) > 0);
}

TYPED_TEST_P(VectorInterfaceTest, EarlyReturnMaxLength) {
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t x;

  // short vector, should return early if it is allowed
  EXPECT_EQ(vecCore::EarlyReturnAllowed(),
            vecCore::EarlyReturnMaxLength(x, vecCore::VectorSize<Vector_t>()));

  // long vector, should not return early, even if it is allowed
  EXPECT_FALSE(
      vecCore::EarlyReturnMaxLength(x, vecCore::VectorSize<Vector_t>() / 2));
}

TYPED_TEST_P(VectorInterfaceTest, LoadStore) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();
  constexpr auto N = 2 * kVS;

  alignas(64) Scalar_t input[N];
  alignas(64) Scalar_t output[N];

  // init input; output
  for (size_t i = 0; i < N; ++i) {
    input[i] = i;
    output[i] = 0;
  }

  // transfer to output via Load/Store sequence
  for (size_t i = 0; i < 2; ++i) {
    Vector_t tmp;
    vecCore::Load<Vector_t>(tmp, &input[i * kVS]);
    tmp = Scalar_t(2) * tmp;
    vecCore::Store<Vector_t>(tmp, &output[i * kVS]);
  }

  // assert input == output
  for (size_t i = 0; i < N; ++i)
    EXPECT_EQ(2 * input[i], output[i]);
}

TYPED_TEST_P(VectorInterfaceTest, StoreToPtr) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();
  constexpr auto N = 2 * kVS;

  Scalar_t input[N];
  Scalar_t output[N];

  // init input; output
  for (size_t i = 0; i < N; ++i) {
    input[i] = 2 * i;
    output[i] = 0;
  }

  // transfer to output via FromPtr; Store sequence
  for (size_t i = 0; i < 2; ++i) {
    Vector_t tmp(vecCore::Load<Vector_t>(&input[i * kVS]));
    vecCore::Store<Vector_t>(tmp, &output[i * kVS]);
  }

  // assert input == output
  for (size_t i = 0; i < N; ++i)
    EXPECT_EQ(input[i], output[i]);
}

TYPED_TEST_P(VectorInterfaceTest, StoreMaskToPtr) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;
  using Mask_t = typename vecCore::Mask<Vector_t>;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();
  constexpr auto N = 2 * kVS;

  Scalar_t input[N];
  bool output[N];

  // init input; output
  for (size_t i = 0; i < N; ++i) {
    input[i] = 2 * i;
    output[i] = false;
  }

  for (size_t i = 0; i < 2; ++i) {
    Vector_t tmp(vecCore::Load<Vector_t>(&input[i * kVS]));
    Mask_t m = tmp > Vector_t(1);
    vecCore::Store(m, &output[i * kVS]);
  }

  for (size_t i = 0; i < N; ++i)
    EXPECT_EQ(output[i], input[i] > Scalar_t(1));
}

TYPED_TEST_P(VectorInterfaceTest, VectorLaneRead) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  Scalar_t input[kVS];

  for (size_t i = 0; i < kVS; ++i) {
    input[i] = i;
  }

  Vector_t tmp(vecCore::Load<Vector_t>(&input[0]));

  for (size_t i = 0; i < kVS; ++i)
    EXPECT_EQ(input[i], vecCore::Get<Vector_t>(tmp, i));
}

TYPED_TEST_P(VectorInterfaceTest, VectorLaneWrite) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;

  constexpr auto kVS = vecCore::VectorSize<Vector_t>();

  Scalar_t input[kVS];

  for (size_t i = 0; i < kVS; ++i) {
    input[i] = i;
  }

  Vector_t tmp(vecCore::Load<Vector_t>(&input[0]));

  for (size_t i = 0; i < kVS; ++i)
    vecCore::Set(tmp, i, Scalar_t(10));

  for (size_t i = 0; i < kVS; ++i)
    EXPECT_EQ(Scalar_t(10), vecCore::Get<Vector_t>(tmp, i));
}

TYPED_TEST_P(VectorInterfaceTest, MaskLaneRead) {
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

TYPED_TEST_P(VectorInterfaceTest, MaskLaneWrite) {
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

TYPED_TEST_P(VectorInterfaceTest, ReduceAdd) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t x(1);

  EXPECT_EQ(Scalar_t(vecCore::VectorSize<Vector_t>()), vecCore::ReduceAdd(x));
}

TYPED_TEST_P(VectorInterfaceTest, ReduceMinMax) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t v;

  for (size_t i = 0; i < vecCore::VectorSize<Vector_t>(); ++i)
    vecCore::Set(v, i, Scalar_t(i + 1));

  EXPECT_EQ(Scalar_t(1), vecCore::ReduceMin(v));
  EXPECT_EQ(Scalar_t(vecCore::VectorSize<Vector_t>()), vecCore::ReduceMax(v));
}

TYPED_TEST_P(VectorInterfaceTest, Convert) {
  using Scalar_t = typename TestFixture::Scalar_t;
  using Vector_t = typename TestFixture::Vector_t;
  using Index_v = typename vecCore::Index<Vector_t>;
  using Index_t = typename vecCore::Scalar<Index_v>;

  Index_v idx;
  Vector_t v;

  for (size_t i = 0; i < vecCore::VectorSize<Vector_t>(); ++i) {
    vecCore::Set(v, i, Scalar_t(i));
    vecCore::Set(idx, i, Index_t(i));
  }

  idx = vecCore::Convert<Index_v, Vector_t>(v);

  EXPECT_TRUE(vecCore::MaskFull(vecCore::Convert<Vector_t, Index_v>(idx) == v));
}

TYPED_TEST_P(VectorInterfaceTest, Gather) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;
  using Index_v = typename vecCore::Index<Vector_t>;
  using Index_t = typename vecCore::Scalar<Index_v>;

  constexpr size_t N = vecCore::VectorSize<Vector_t>();

  Scalar_t input[N * N];
  for (size_t i = 0; i < N * N; ++i) {
    input[i] = i;
  }

  Index_v idx;
  for (size_t i = 0; i < N; ++i)
    vecCore::Set(idx, i, Index_t(i));

  for (size_t i = 1; i <= N; ++i) {
    Index_v iidx = Index_v((Index_t)i) * idx;

    Vector_t x = vecCore::Gather<Vector_t>(input, iidx);

    for (size_t j = 0; j < N; ++j)
      EXPECT_TRUE(vecCore::Get(x, j) == input[i * j]);
  }
}

TYPED_TEST_P(VectorInterfaceTest, Scatter) {
  using Vector_t = typename TestFixture::Vector_t;
  using Scalar_t = typename TestFixture::Scalar_t;
  using Index_v = typename vecCore::Index<Vector_t>;
  using Index_t = typename vecCore::Scalar<Index_v>;

  constexpr size_t N = vecCore::VectorSize<Vector_t>();

  Index_t index[N];
  Scalar_t input[N], output[N];

  for (size_t i = 0; i < N; ++i) {
    input[i] = i;
    index[i] = i;
  }

  Index_v idx(vecCore::Load<Index_v>(&index[0]));

  Vector_t x = vecCore::Load<Vector_t>(&input[0]);

  vecCore::Scatter<Vector_t>(x, &output[0], idx);

  Vector_t y = vecCore::Load<Vector_t>(&output[0]);

  EXPECT_TRUE(vecCore::MaskFull(x == y));
}

REGISTER_TYPED_TEST_SUITE_P(VectorInterfaceTest, EarlyReturnMaxLength,
                            VectorSize, VectorSizeVariable, VectorLaneRead,
                            VectorLaneWrite, MaskLaneRead, MaskLaneWrite,
                            LoadStore, StoreToPtr, StoreMaskToPtr, ReduceAdd,
                            ReduceMinMax, Convert, Gather, Scatter);

#define TEST_BACKEND_P(name, x)                                                \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, VectorInterfaceTest,                    \
                                 VectorTypes<vecCore::backend::x>)

#define TEST_BACKEND(x) TEST_BACKEND_P(x, x)

#include "backends.h"
