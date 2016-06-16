#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>

#ifndef __APPLE__
#include <malloc.h>
#endif

#include "VecCore/Timer.h"
#include <VecCore/VecCore>

using namespace vecCore;

static const int N = (8 * 1024 * 1024);

// solve ax2 + bx + c = 0

// naive scalar code

template <typename T>
int QuadSolveNaive(T a, T b, T c, T &x1, T &x2)
{
  T delta = b * b - 4.0 * a * c;

  if (delta < 0.0)
    return 0;

  if (delta < NumericLimits<T>::Epsilon()) {
    x1 = x2 = -0.5 * b / a;
    return 1;
  }

  if (b >= 0.0) {
    x1 = -0.5 * (b + math::Sqrt(delta)) / a;
    x2 = c / (a * x1);
  }
  else {
    x2 = -0.5 * (b - math::Sqrt(delta)) / a;
    x1 = c / (a * x2);
  }

  return 2;
}

// optimized scalar code

template <typename T>
void QuadSolveOptimized(const T &a, const T &b, const T &c, T &x1, T &x2, int &roots)
{
  T a_inv = T(1.0) / a;
  T delta = b * b - T(4.0) * a * c;
  T s = (b >= 0) ? T(1.0) : T(-1.0);

  roots = delta > NumericLimits<T>::Epsilon() ? 2 : delta < T(0.0) ? 0 : 1;

  switch (roots) {
  case 2:
    x1 = T(-0.5) * (b + s * math::Sqrt(delta));
    x2 = c / x1;
    x1 *= a_inv;
    return;

  case 0:
    return;

  case 1:
    x1 = x2 = T(-0.5) * b * a_inv;
    return;

  default:
    return;
  }
}

#if !defined(VECCORE_DISABLE_SIMD) && defined(__AVX2__)

// explicit AVX2 code using intrinsics

VECCORE_FORCE_NOINLINE
void QuadSolveAVX(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                  float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots)
{
  __m256 one = _mm256_set1_ps(1.0f);
  __m256 va = _mm256_load_ps(a);
  __m256 vb = _mm256_load_ps(b);
  __m256 zero = _mm256_set1_ps(0.0f);
  __m256 a_inv = _mm256_div_ps(one, va);
  __m256 b2 = _mm256_mul_ps(vb, vb);
  __m256 eps = _mm256_set1_ps(NumericLimits<float>::Epsilon());
  __m256 vc = _mm256_load_ps(c);
  __m256 negone = _mm256_set1_ps(-1.0f);
  __m256 ac = _mm256_mul_ps(va, vc);
  __m256 sign = _mm256_blendv_ps(negone, one, _mm256_cmp_ps(vb, zero, _CMP_GE_OS));
#if defined(__FMA__)
  __m256 delta = _mm256_fmadd_ps(_mm256_set1_ps(-4.0f), ac, b2);
  __m256 r1 = _mm256_fmadd_ps(sign, _mm256_sqrt_ps(delta), vb);
#else
  __m256 delta = _mm256_sub_ps(b2, __256_mul_ps(_mm256_set1_ps(-4.0f), ac));
  __m256 r1 = _mm256_add_ps(vb, _mm256_mul_ps(sign, _mm256_sqrt_ps(delta)));
#endif
  __m256 mask0 = _mm256_cmp_ps(delta, zero, _CMP_LT_OS);
  __m256 mask2 = _mm256_cmp_ps(delta, eps, _CMP_GE_OS);
  r1 = _mm256_mul_ps(_mm256_set1_ps(-0.5f), r1);
  __m256 r2 = _mm256_div_ps(vc, r1);
  r1 = _mm256_mul_ps(a_inv, r1);
  __m256 r3 = _mm256_mul_ps(_mm256_set1_ps(-0.5f), _mm256_mul_ps(vb, a_inv));
  __m256 nr = _mm256_blendv_ps(one, _mm256_set1_ps(2), mask2);
  nr = _mm256_blendv_ps(nr, _mm256_set1_ps(0), mask0);
  r3 = _mm256_blendv_ps(r3, zero, mask0);
  r1 = _mm256_blendv_ps(r3, r1, mask2);
  r2 = _mm256_blendv_ps(r3, r2, mask2);
  _mm256_store_si256((__m256i *)roots, _mm256_cvtps_epi32(nr));
  _mm256_store_ps(x1, r1);
  _mm256_store_ps(x2, r2);
}
#endif

// explicit SIMD code

template <class Backend>
void QuadSolveSIMD(typename Backend::Float_v const &a, typename Backend::Float_v const &b,
                   typename Backend::Float_v const &c, typename Backend::Float_v &x1, typename Backend::Float_v &x2,
                   typename Backend::Int32_v &roots)
{
  using Float_v = typename Backend::Float_v;
  using Int32_v = typename Backend::Int32_v;
  using FMask = Mask_v<Float_v>;
  using IMask = Mask_v<Int32_v>;

  Float_v a_inv = Float_v(1.0f) / a;
  Float_v delta = b * b - Float_v(4.0f) * a * c;
  Float_v sign = Blend(FMask(b >= Float_v(0.0f)), Float_v(1.0f), Float_v(-1.0f));

  FMask mask0(delta < Float_v(0.0f));
  FMask mask2(delta >= NumericLimits<Float_v>::Epsilon());

  Float_v root1 = Float_v(-0.5f) * (b + sign * math::Sqrt(delta));
  Float_v root2 = c / root1;
  root1 = root1 * a_inv;

  FMask mask1 = !(mask2 || mask0);

  MaskedAssign(x1, mask2, root1);
  MaskedAssign(x2, mask2, root2);
  roots = Blend((IMask)mask2, Int32_v(2), Int32_v(0));

  if (MaskEmpty(mask1))
    return;

  root1 = Float_v(-0.5f) * b * a_inv;
  MaskedAssign(roots, (IMask)mask1, Int32_v(1));
  MaskedAssign(x1, mask1, root1);
  MaskedAssign(x2, mask1, root1);
}

template <class Backend>
void TestQuadSolve(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                   float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots, size_t N, const char *name)
{
  using Float_v = typename Backend::Float_v;
  using Int32_v = typename Backend::Int32_v;

  Timer<milliseconds> timer;
  for (size_t i = 0; i < N; i += VectorSize<Float_v>())
    QuadSolveSIMD<Backend>((Float_v &)(a[i]), (Float_v &)(b[i]), (Float_v &)(c[i]), (Float_v &)(x1[i]),
                           (Float_v &)(x2[i]), (Int32_v &)(roots[i]));
  double t = timer.Elapsed();

#ifdef VERBOSE
  int index = (int)((N - 100) * drand48());
  for (int i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%32s: %7.3lfms\n", name, t);
}

int main(int argc, char *argv[])
{
  float *a, *b, *c, *x1, *x2;
  int *roots;
  posix_memalign((void**) &a, VECCORE_SIMD_ALIGN, N * sizeof(float));
  posix_memalign((void**) &b, VECCORE_SIMD_ALIGN, N * sizeof(float));
  posix_memalign((void**) &c, VECCORE_SIMD_ALIGN, N * sizeof(float));

   
  posix_memalign((void**) &roots, VECCORE_SIMD_ALIGN, N * sizeof(int));
  posix_memalign((void**) &x1, VECCORE_SIMD_ALIGN, N * sizeof(float));
  posix_memalign((void**) &x2,VECCORE_SIMD_ALIGN, N * sizeof(float));

  srand48(time(NULL));

  for (int i = 0; i < N; i++) {
    a[i] = 10.0 * (drand48() - 0.5);
    b[i] = 10.0 * (drand48() - 0.5);
    c[i] = 50.0 * (drand48() - 0.5);
    x1[i] = 0.0;
    x2[i] = 0.0;
    roots[i] = 0;
  }

  Timer<milliseconds> timer;
  for (int i = 0; i < N; i++)
    roots[i] = QuadSolveNaive(a[i], b[i], c[i], x1[i], x2[i]);
  double t = timer.Elapsed();

#ifdef VERBOSE
  int index = (int)((N - 100) * drand48());
  // print random values to check results across backends
  for (int i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%32s: %7.3lfms\n", "naive scalar", t);

  timer.Start();
  for (int i = 0; i < N; i++)
    QuadSolveOptimized<float>(a[i], b[i], c[i], x1[i], x2[i], roots[i]);
  t = timer.Elapsed();

#ifdef VERBOSE
  for (int i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%32s: %7.3lfms\n", "optimized scalar", t);

#if !defined(VECCORE_DISABLE_SIMD) && defined(__AVX2__)
  timer.Start();
  for (int i = 0; i < N; i += 8)
    QuadSolveAVX(&a[i], &b[i], &c[i], &x1[i], &x2[i], &roots[i]);
  t = timer.Elapsed();

#ifdef VERBOSE
  for (int i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], x1[i], x2[i]);
#endif
  printf("%32s: %7.3lfms\n", "AVX2 intrinsics", t);
#endif

  TestQuadSolve<backend::Scalar>(a, b, c, x1, x2, roots, N, "plain scalar backend");
  TestQuadSolve<backend::ScalarWrapper>(a, b, c, x1, x2, roots, N, "scalarwrapper backend");

#ifdef VECCORE_ENABLE_VC
  TestQuadSolve<backend::VcScalar>(a, b, c, x1, x2, roots, N, "Vc scalar backend");
  TestQuadSolve<backend::VcVector>(a, b, c, x1, x2, roots, N, "Vc vector backend");
  TestQuadSolve<backend::VcSimdArray<8>>(a, b, c, x1, x2, roots, N, "VcSimdArray<8> backend");
  TestQuadSolve<backend::VcSimdArray<16>>(a, b, c, x1, x2, roots, N, "VcSimdArray<16> backend");
  TestQuadSolve<backend::VcSimdArray<32>>(a, b, c, x1, x2, roots, N, "VcSimdArray<32> backend");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
  TestQuadSolve<backend::UMESimd>(a, b, c, x1, x2, roots, N, "UME::SIMD backend");
#endif

  return 0;
}
