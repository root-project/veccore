#undef NDEBUG
#include <cassert>
#include <cstdio>
#include <algorithm>

#include <VecCore>

using namespace vecCore;

template <class Backend>
VECCORE_FORCE_NOINLINE
typename Backend::Real_t StdMin(typename Backend::Real_t a, typename Backend::Real_t b)
{
  return std::min(a, b);
}

template <class Backend>
VECCORE_FORCE_NOINLINE
typename Backend::Real_v StdMinV(typename Backend::Real_v a, typename Backend::Real_v b)
{
  return std::min(a, b);
}

template <class Backend>
VECCORE_FORCE_NOINLINE
typename Backend::Real_v NewMin(typename Backend::Real_v a, typename Backend::Real_v b)
{
  return math::Min(a, b);
}

int main(int argc, char *argv[])
{
  using Backend = backend::Basic<float>;

  typename Backend::Real_v x = atof(argv[1]);
  typename Backend::Real_v y = atof(argv[2]);

  // make assembly version of this function appear in binary
  x = StdMin<Backend>(1.0, 2.0);

  printf("x = %f\n", x[0]);

  x = StdMinV<Backend>(x, y);

  printf("x = %f\n", x[0]);

  x = NewMin<Backend>(x, y);

  printf("x = %f\n", x[0]);

  return 0;
}
