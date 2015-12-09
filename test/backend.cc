#undef NDEBUG
#include <cassert>
#include <cstdio>

#include <VecCore>

using namespace vecCore;

template <class T>
void TestMask() {
  using Mask = typename T::Mask;

  Mask mtrue(true), mfalse(false);

  assert(IsFull(mtrue));
  assert(IsEmpty(mfalse));

  assert(IsFull(mtrue || mfalse));
  assert(IsEmpty(mtrue && mfalse));
}

template <class T>
void TestReal() {
  using Mask = typename T::Mask;

  TestMask<T>();

  printf("T::Size = %d\n\n", (int)T::Size);

  T x(0.0), y(1.0), z(2.0);

  x = y + z;
  x = y - z;
  x = y * z;
  x = y / z;

  for (size_t i = 0; i < T::Size; i++)
    x[i] = i + 1;

  Mask mask = (x > T(T::Size) / T(2));

  for (size_t i = 0; i < T::Size; i++)
    printf("x[%lu] = %g, x[%lu] > %lu == %s\n", i, (double)x[i], i,
           (size_t)T::Size / 2, mask[i] ? "true" : "false");

  printf("\n");
}

template <class T>
void TestInteger() {
  using Mask = typename T::Mask;

  TestMask<T>();

  printf("T::Size = %d\n\n", (int)T::Size);

  T x(0), y(1), z(2);

  x = y + z;
  x = y - z;
  x = y * z;
  x = y / z;

  for (size_t i = 0; i < T::Size; i++)
    x[i] = i + 1;

  Mask mask = (x > T(T::Size) / T(2));

  for (size_t i = 0; i < T::Size; i++)
    printf("x[%lu] = %d, x[%lu] > %d == %s\n", i, (int)x[i], i,
           (int)T::Size / 2, mask[i] ? "true" : "false");

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

  printf("Backend Test: %s\n\n", "Basic");
  TestBackend<backend::Basic<float> >();

#ifdef VECCORE_ENABLE_VC
  printf("Backend Test: %s\n\n", "Vc (Scalar)");
  TestBackend<backend::VcScalar<float> >();

  printf("Backend Test: %s\n\n", "Vc (Vector)");
  TestBackend<backend::VcVector<float> >();

  printf("Backend Test: %s\n\n", "Vc (SimdArray, float)");
  TestBackend<backend::VcSimdArray<float, 8> >();

  printf("Backend Test: %s\n\n", "Vc (SimdArray, double)");
  TestBackend<backend::VcSimdArray<double, 8> >();
#endif

  return 0;
}
