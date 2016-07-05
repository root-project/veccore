#include <VecCore/VecCore>

#include <type_traits>
#include <gtest/gtest.h>

// a compile time test - testing the type traits for backends
// test passes, if compilation passes
using namespace vecCore;
using namespace vecCore::backend;

// testing the ScalarType property of TypeTraits
#define TEST_SCALAR_TRAIT(x, vector, scalar) \
  static_assert(std::is_same<TypeTraits<x::vector>::ScalarType, scalar>::value, "Type trait assertion failed");

#define TEST_TRAIT(x)                      \
  TEST_SCALAR_TRAIT(x, Real_v, Real_s)     \
  TEST_SCALAR_TRAIT(x, Float_v, Float_s)   \
  TEST_SCALAR_TRAIT(x, Double_v, Double_s) \
  TEST_SCALAR_TRAIT(x, Int32_v, Int32_s)   \
  TEST_SCALAR_TRAIT(x, UInt32_v, UInt32_s) \
  TEST_SCALAR_TRAIT(x, Int16_v, Int16_s)   \
  TEST_SCALAR_TRAIT(x, UInt16_v, UInt16_s)

TEST_TRAIT(Scalar);
TEST_TRAIT(ScalarWrapper);

#ifdef VECCORE_ENABLE_VC
TEST_TRAIT(VcScalar);
TEST_TRAIT(VcVector);
TEST_TRAIT(VcSimdArray<8>);
#endif

#ifdef VECCORE_ENABLE_UMESIMD
TEST_TRAIT(UMESimd)
TEST_TRAIT(UMESimdArray<16>)
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
