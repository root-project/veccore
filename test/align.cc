#undef NDEBUG
#include <gtest/gtest.h>
#include <VecCore/VecCore>

#include <array>
#include <memory>
#include <vector>

using namespace testing;
using namespace vecCore;

template<class Backend>
using VcTypes = Types
  <
    typename Backend::Float_v,
    typename Backend::Double_v,
    typename Backend::Int16_v,
    typename Backend::UInt16_v,
    typename Backend::Int32_v,
    typename Backend::UInt32_v
  >;

template<class Backend>
using UMESimdTypes = Types
  <
    typename Backend::Float_v,
    typename Backend::Double_v,
    typename Backend::Int16_v,
    typename Backend::UInt16_v,
    typename Backend::Int32_v,
    typename Backend::UInt32_v,
    typename Backend::Int64_v,
    typename Backend::UInt64_v
  >;

template<class T>
class AlignmentTest : public Test {
public:
  using Scalar_t = typename vecCore::Scalar<T>;
  using Vector_t = T;
};

TYPED_TEST_SUITE_P(AlignmentTest);

inline bool
is_aligned(const void *ptr, size_t align)
{
  return (uintptr_t)ptr % align == 0;
}

TYPED_TEST_P(AlignmentTest, Stack)
{
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t v;

  EXPECT_TRUE(is_aligned(&v, alignof(Vector_t)));
}

TYPED_TEST_P(AlignmentTest, Heap)
{
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t *v = new Vector_t();

  EXPECT_TRUE(is_aligned(v, alignof(Vector_t)));

  delete v;
}

TYPED_TEST_P(AlignmentTest, StdArray)
{
  using Vector_t = typename TestFixture::Vector_t;

  std::array<Vector_t, 8> v;

  EXPECT_TRUE(is_aligned(std::addressof(v), alignof(Vector_t)));
}

#if 0
TYPED_TEST_P(AlignmentTest, StdVector)
{
  using Vector_t = typename TestFixture::Vector_t;

  std::vector<Vector_t> v(8);

  EXPECT_TRUE(is_aligned(std::addressof(v), alignof(Vector_t)));
}

TYPED_TEST_P(AlignmentTest, Collection)
{
  using Vector_t = typename TestFixture::Vector_t;

  std::vector<std::array<Vector_t, 8>> v(8);

  EXPECT_TRUE(is_aligned(std::addressof(v), alignof(Vector_t)));
}

REGISTER_TYPED_TEST_SUITE_P(AlignmentTest, Stack, Heap, StdArray, StdVector, Collection);
#endif
REGISTER_TYPED_TEST_SUITE_P(AlignmentTest, Stack, Heap, StdArray);

#define TEST_BACKEND_P(name, types, x) \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, AlignmentTest, types<vecCore::backend::x>)

/* Avoid errors when only available backend is scalar, which has no alignment requirements */
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(AlignmentTest);

/*
 * Notes:
 *  - UME::SIMD does not promise its types are aligned
 */

#if defined(VECCORE_ENABLE_VC)
TEST_BACKEND_P(VcVector, VcTypes, VcVector);
#ifdef __SSE__
TEST_BACKEND_P(VcSimdArraySSE, VcTypes, VcSimdArray<2>);
#endif
#ifdef __AVX__
TEST_BACKEND_P(VcSimdArrayAVX, VcTypes, VcSimdArray<4>);
#endif
#endif

/* To be turned on when UME::SIMD has aligned types */

#if 0 && defined(VECCORE_ENABLE_UMESIMD)
TEST_BACKEND_P(UMESimd, UMESimdTypes, UMESimd);
TEST_BACKEND_P(UMESimdArray, UMESimdTypes, UMESimdArray<16>);
#endif

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
