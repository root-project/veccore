#undef NDEBUG
#include <cstdio>
#include <type_traits>

#include <VecCore/VecCore>

using namespace vecCore;

template <class Backend>
void Print(const typename Backend::Real_v &x)
{
  const size_t N = VectorSize(x);
  printf("[ ");
  for (size_t i = 0; i < N - 1; i++)
    printf("%g, ", x[i]);
  printf("%g ]", x[N - 1]);
}

template <>
void Print<backend::Scalar>(const Real_s &x)
{
  printf("[ %g ]", x);
}

template <class Backend>
void Test(const char *name)
{
  using Real_v = typename Backend::Real_v;

  static_assert(std::is_same<typename ScalarType<Real_v>::Type, Real_s>::value,
                "conversion of vector type to scalar type failed");

  printf("Backend: %s\n\n", name);

  printf("NumericLimits::Min()\n Real_s: [ %g ]\n Real_v: ", NumericLimits<Real_s>::Min());
  Print<Backend>(NumericLimits<Real_v>::Min());
  printf("\n\n");

  printf("NumericLimits::Max()\n Real_s: [ %g ]\n Real_v: ", NumericLimits<Real_s>::Max());
  Print<Backend>(NumericLimits<Real_v>::Max());
  printf("\n\n");

  printf("NumericLimits::Lowest()\n Real_s: [ %g ]\n Real_v: ", NumericLimits<Real_s>::Lowest());
  Print<Backend>(NumericLimits<Real_v>::Lowest());
  printf("\n\n");

  printf("NumericLimits::Highest()\n Real_s: [ %g ]\n Real_v: ", NumericLimits<Real_s>::Highest());
  Print<Backend>(NumericLimits<Real_v>::Highest());
  printf("\n\n");

  printf("NumericLimits::Epsilon()\n Real_s: [ %g ]\n Real_v: ", NumericLimits<Real_s>::Epsilon());
  Print<Backend>(NumericLimits<Real_v>::Epsilon());
  printf("\n\n");

  printf("NumericLimits::Infinity()\n Real_s: [ %g ]\n Real_v: ", NumericLimits<Real_s>::Infinity());
  Print<Backend>(NumericLimits<Real_v>::Infinity());
  printf("\n\n");
}

int main()
{
  Test<backend::Scalar>("Scalar");
  Test<backend::ScalarWrapper>("ScalarWrapper");

#ifdef VECCORE_ENABLE_VC
  Test<backend::VcScalar>("VcScalar");
  Test<backend::VcVector>("VcVector");
  Test<backend::VcSimdArray<16>>("VcSimdArray");
#endif

  return 0;
}
