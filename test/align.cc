#include "test.h"

#include <array>
#include <memory>

using namespace vecCore;

template <class T> class AlignmentTest : public VectorTypeTest<T> {};

TYPED_TEST_SUITE_P(AlignmentTest);

inline bool is_aligned(const void *ptr, size_t align) {
  return (uintptr_t)ptr % align == 0;
}

TYPED_TEST_P(AlignmentTest, Stack) {
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t v;

  EXPECT_TRUE(is_aligned(&v, alignof(Vector_t)));
}

TYPED_TEST_P(AlignmentTest, Heap) {
  using Vector_t = typename TestFixture::Vector_t;

  Vector_t *v = new Vector_t();

  EXPECT_TRUE(is_aligned(v, alignof(Vector_t)));

  delete v;
}

TYPED_TEST_P(AlignmentTest, StdArray) {
  using Vector_t = typename TestFixture::Vector_t;

  std::array<Vector_t, 8> v;

  EXPECT_TRUE(is_aligned(std::addressof(v), alignof(Vector_t)));
}

REGISTER_TYPED_TEST_SUITE_P(AlignmentTest, Stack, Heap, StdArray);

#define TEST_BACKEND_P(name, x)                                                \
  INSTANTIATE_TYPED_TEST_SUITE_P(name, AlignmentTest, VectorTypes<backend::x>)

/* Avoid errors when only available backend is scalar, which has no alignment
 * requirements */
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(AlignmentTest);

#if defined(VECCORE_ENABLE_VC)
TEST_BACKEND_P(VcVector, VcVector);
#ifdef __SSE__
TEST_BACKEND_P(VcSimdArraySSE, VcSimdArray<2>);
#endif
#ifdef __AVX__
TEST_BACKEND_P(VcSimdArrayAVX, VcSimdArray<4>);
#endif
#endif

/* Note: UME::SIMD does not guarantee alignment of its types */

#ifdef VECCORE_ENABLE_STD_SIMD
TEST_BACKEND_P(SIMDScalar, SIMDScalar);
TEST_BACKEND_P(SIMDNative, SIMDNative);
TEST_BACKEND_P(SIMDVector4, SIMDVector<4>);
TEST_BACKEND_P(SIMDVector8, SIMDVector<8>);
#endif
