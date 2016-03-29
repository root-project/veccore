#undef NDEBUG
#include <cassert>
#include <cstdio>

#include <VecCore/VecCore>

using namespace vecCore;

template <class T>
void TestMask() {
  using Mask = typename T::Mask;

  Mask mtrue(true), mfalse(false);

  assert(MaskFull(mtrue));
  assert(MaskEmpty(mfalse));

  assert(MaskFull(mtrue || mfalse));
  assert(MaskEmpty(mtrue && mfalse));
}

template <class T>
void TestReal() {
  using Mask = typename T::Mask;

  TestMask<T>();

  printf("T::size() = %d\n\n", (int)T::size());

  T x(0.0), y(1.0), z(2.0);

  x = y + z;
  x = y - z;
  x = y * z;
  x = y / z;

  for (size_t i = 0; i < T::size(); i++)
    x[i] = i + 1;

  Mask mask = (x > T(T::size()) / T(2));

  for (size_t i = 0; i < T::size(); i++)
    printf("x[%lu] = %g, x[%lu] > %lu == %s\n", i, (double)x[i], i,
           (size_t)T::size() / 2, mask[i] ? "true" : "false");

  printf("\n");
}

template <class T>
void TestInteger() {
  using Mask = typename T::Mask;

  TestMask<T>();

  printf("T::size() = %d\n\n", (int)T::size());

  T x(0), y(1), z(2);

  x = y + z;
  x = y - z;
  x = y * z;
  x = y / z;

  for (size_t i = 0; i < T::size(); i++)
    x[i] = i + 1;

  Mask mask = (x > T(T::size()) / T(2));

  for (size_t i = 0; i < T::size(); i++)
    printf("x[%lu] = %d, x[%lu] > %d == %s\n", i, (int)x[i], i,
           (int)T::size() / 2, mask[i] ? "true" : "false");

  printf("\n");
}

template <class Backend>
void TestBackend() {
  TestReal<typename Backend::Float_v>();
  TestReal<typename Backend::Double_v>();
  TestInteger<typename Backend::Int32_v>();
  TestInteger<typename Backend::UInt32_v>();
}

int main(int argc, char *argv[]) {

  printf("Backend Test: %s\n\n", "ScalarWrapper");
  TestBackend<backend::ScalarWrapper>();

#ifdef VECCORE_ENABLE_VC
  printf("Backend Test: %s\n\n", "Vc (Scalar)");
  TestBackend<backend::VcScalar>();

  printf("Backend Test: %s\n\n", "Vc (Vector)");
  TestBackend<backend::VcVector>();

  printf("Backend Test: %s\n\n", "Vc (SimdArray<16>)");
  TestBackend<backend::VcSimdArray<16> >();
#endif

  return 0;
}
