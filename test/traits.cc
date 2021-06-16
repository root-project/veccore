#include <VecCore/VecCore>

#include <type_traits>
#include <gtest/gtest.h>

// a compile time test - testing the type traits for backends
// test passes, if compilation passes
using namespace vecCore;

// testing the ScalarType property of TypeTraits
#define TEST_SCALAR_TRAIT(x, vector, scalar) \
  static_assert(std::is_same<TypeTraits<x::vector>::ScalarType, scalar>::value, "Type trait assertion failed");

#define TEST_TRAIT(x)                      \
  TEST_SCALAR_TRAIT(x, Real_v,   Real_s)   \
  TEST_SCALAR_TRAIT(x, Float_v,  float)    \
  TEST_SCALAR_TRAIT(x, Double_v, double)   \
  TEST_SCALAR_TRAIT(x, Int32_v,  int32_t)  \
  TEST_SCALAR_TRAIT(x, UInt32_v, uint32_t) \
  TEST_SCALAR_TRAIT(x, Int16_v,  int16_t)  \
  TEST_SCALAR_TRAIT(x, UInt16_v, uint16_t)

TEST_TRAIT(backend::Scalar);
TEST_TRAIT(backend::ScalarWrapper);

#ifdef VECCORE_ENABLE_VC
TEST_TRAIT(backend::VcScalar);
TEST_TRAIT(backend::VcVector);
TEST_TRAIT(backend::VcSimdArray<8>);
#endif

#ifdef VECCORE_ENABLE_UMESIMD
TEST_TRAIT(backend::UMESimd)
TEST_TRAIT(backend::UMESimdArray<16>)
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
TEST_TRAIT(backend::SIMDScalar)
TEST_TRAIT(backend::SIMDVector<4>)
TEST_TRAIT(backend::SIMDVector<8>)
TEST_TRAIT(backend::SIMDVector<16>)
TEST_TRAIT(backend::SIMDNative)
#endif

TEST(TraitTest, TraitTest)
{
  // if this runs; it passes trivially
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
