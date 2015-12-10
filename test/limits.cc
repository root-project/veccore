#undef NDEBUG
#include <cassert>
#include <cstdio>
#include <type_traits>

#include <VecCore>

using namespace vecCore;

template <typename T>
void print(const T& x)
{
  const size_t N = T::Size;
  printf("[ ");
  for (size_t i = 0; i < N-1; i++)
    printf("%g, ", x[i]);
  printf("%g ]", x[N-1]);
}

template <class Backend>
void test(const char* name)
{
  typedef typename Backend::Real_t Real_t;
  typedef typename Backend::Real_v Real_v;

  static_assert(std::is_same<typename ScalarType<Real_v>::Type, Real_t>::value == true,
    "conversion of vector type to scalar type failed");

  printf("Backend: %s\n\n", name);

  printf("NumericLimits::Min()\n Real_t: [ %g ]\n Real_v: ",
    NumericLimits<Real_t>::Min());
  print<Real_v>(NumericLimits<Real_v>::Min());
  printf("\n\n");

  printf("NumericLimits::Max()\n Real_t: [ %g ]\n Real_v: ",
    NumericLimits<Real_t>::Max());
  print<Real_v>(NumericLimits<Real_v>::Max());
  printf("\n\n");

  printf("NumericLimits::Lowest()\n Real_t: [ %g ]\n Real_v: ",
    NumericLimits<Real_t>::Lowest());
  print<Real_v>(NumericLimits<Real_v>::Lowest());
  printf("\n\n");

  printf("NumericLimits::Highest()\n Real_t: [ %g ]\n Real_v: ",
    NumericLimits<Real_t>::Highest());
  print<Real_v>(NumericLimits<Real_v>::Highest());
  printf("\n\n");

  printf("NumericLimits::Epsilon()\n Real_t: [ %g ]\n Real_v: ",
    NumericLimits<Real_t>::Epsilon());
  print<Real_v>(NumericLimits<Real_v>::Epsilon());
  printf("\n\n");

  printf("NumericLimits::Infinity()\n Real_t: [ %g ]\n Real_v: ",
    NumericLimits<Real_t>::Infinity());
  print<Real_v>(NumericLimits<Real_v>::Infinity());
  printf("\n\n");
}

int main(int argc, char *argv[])
{
  test<backend::Basic<float> >("Basic (single precision)");
  test<backend::Basic<double> >("Basic (double precision)");

#ifdef VECCORE_ENABLE_VC
  test<backend::VcScalar<float> >("VcScalar (single precision)");
  test<backend::VcScalar<double> >("VcScalar (double precision)");

  test<backend::VcVector<float> >("VcVector (single precision)");
  test<backend::VcVector<double> >("VcVector (double precision)");

  test<backend::VcSimdArray<float> >("VcSimdArray (single precision)");
  test<backend::VcSimdArray<double> >("VcSimdArray (double precision)");
#endif

  return 0;
}
