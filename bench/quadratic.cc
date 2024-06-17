#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <random>

#include "timer.h"
#include <VecCore/VecCore>

using namespace vecCore;

static constexpr size_t kNruns = 10;
static constexpr size_t kN = (1024 * 1024);

#ifdef VECCORE_TIMER_CYCLES
using time_unit = cycles;
static const char *time_unit_name = "cycles";
#else
using time_unit = nanoseconds;
static const char *time_unit_name = "ns";
#endif

// solve ax2 + bx + c = 0

// naive scalar code

template <typename T>
int QuadSolve(T a, T b, T c, T &x1, T &x2)
{
  T delta = b * b - 4.0 * a * c;

  if (delta < 0.0) return 0;

  if (delta < NumericLimits<T>::Epsilon()) {
    x1 = x2 = -0.5 * b / a;
    return 1;
  }

  if (b >= 0.0) {
    x1 = -0.5 * (b + Sqrt(delta)) / a;
    x2 = c / (a * x1);
  } else {
    x2 = -0.5 * (b - Sqrt(delta)) / a;
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
  T s     = (b >= 0) ? T(1.0) : T(-1.0);

  roots = delta > NumericLimits<T>::Epsilon() ? 2 : delta < T(0.0) ? 0 : 1;

  switch (roots) {
  case 2:
    x1 = T(-0.5) * (b + s * Sqrt(delta));
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

#if defined(__AVX2__)

// explicit AVX2 code using intrinsics

void QuadSolveAVX(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                  float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots)
{
  __m256 one    = _mm256_set1_ps(1.0f);
  __m256 va     = _mm256_load_ps(a);
  __m256 vb     = _mm256_load_ps(b);
  __m256 zero   = _mm256_set1_ps(0.0f);
  __m256 a_inv  = _mm256_div_ps(one, va);
  __m256 b2     = _mm256_mul_ps(vb, vb);
  __m256 eps    = _mm256_set1_ps(NumericLimits<float>::Epsilon());
  __m256 vc     = _mm256_load_ps(c);
  __m256 negone = _mm256_set1_ps(-1.0f);
  __m256 ac     = _mm256_mul_ps(va, vc);
  __m256 sign   = _mm256_blendv_ps(negone, one, _mm256_cmp_ps(vb, zero, _CMP_GE_OS));
#if defined(__FMA__)
  __m256 delta = _mm256_fmadd_ps(_mm256_set1_ps(-4.0f), ac, b2);
  __m256 r1    = _mm256_fmadd_ps(sign, _mm256_sqrt_ps(delta), vb);
#else
  __m256 delta = _mm256_sub_ps(b2, _mm256_mul_ps(_mm256_set1_ps(-4.0f), ac));
  __m256 r1    = _mm256_add_ps(vb, _mm256_mul_ps(sign, _mm256_sqrt_ps(delta)));
#endif
  __m256 mask0 = _mm256_cmp_ps(delta, zero, _CMP_LT_OS);
  __m256 mask2 = _mm256_cmp_ps(delta, eps, _CMP_GE_OS);
  r1           = _mm256_mul_ps(_mm256_set1_ps(-0.5f), r1);
  __m256 r2    = _mm256_div_ps(vc, r1);
  r1           = _mm256_mul_ps(a_inv, r1);
  __m256 r3    = _mm256_mul_ps(_mm256_set1_ps(-0.5f), _mm256_mul_ps(vb, a_inv));
  __m256 nr    = _mm256_blendv_ps(one, _mm256_set1_ps(2), mask2);
  nr           = _mm256_blendv_ps(nr, _mm256_set1_ps(0), mask0);
  r3           = _mm256_blendv_ps(r3, zero, mask0);
  r1           = _mm256_blendv_ps(r3, r1, mask2);
  r2           = _mm256_blendv_ps(r3, r2, mask2);
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
  using FMask   = Mask<Float_v>;
  using IMask   = Mask<Int32_v>;

  Float_v a_inv = Float_v(1.0f) / a;
  Float_v delta = b * b - Float_v(4.0f) * a * c;
  Float_v sign  = Blend(FMask(b >= Float_v(0.0f)), Float_v(1.0f), Float_v(-1.0f));

  FMask mask0(delta < Float_v(0.0f));
  FMask mask2(delta >= NumericLimits<Float_v>::Epsilon());

  Float_v root1 = Float_v(-0.5f) * (b + sign * Sqrt(delta));
  Float_v root2 = c / root1;
  root1         = root1 * a_inv;

  FMask mask1 = !(mask2 || mask0);

  MaskedAssign(x1, mask2, root1);
  MaskedAssign(x2, mask2, root2);
  roots = Blend(reinterpret_cast<IMask&>(mask2), Int32_v(2), Int32_v(0));

  if (MaskEmpty(mask1)) return;

  root1 = Float_v(-0.5f) * b * a_inv;
  MaskedAssign(roots, reinterpret_cast<IMask&>(mask1), Int32_v(1));
  MaskedAssign(x1, mask1, root1);
  MaskedAssign(x2, mask1, root1);
}

VECCORE_FORCE_NOINLINE
void TestQuadSolve(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                   float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots, size_t kN)
{
  Timer<time_unit> timer;
  unsigned long long t[kNruns], mean = 0;
  for (size_t n = 0; n < kNruns; n++) {
    timer.Start();
    for (size_t i = 0; i < kN; i ++)
      roots[i] = QuadSolve(a[i], b[i], c[i], x1[i], x2[i]);
    t[n] = timer.Elapsed();
  }

  for (size_t n = 0; n < kNruns; n++)
    mean += t[n];

  mean = mean / (kN * kNruns);

#ifdef VERBOSE
  size_t index = (size_t)((kN - 100) * rand()/RAND_MAX);
  for (size_t i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%20s %6llu\n", "Scalar", mean);
}

VECCORE_FORCE_NOINLINE
void TestQuadSolveOptimized(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                            float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots, size_t kN)
{
  Timer<time_unit> timer;
  unsigned long long t[kNruns], mean = 0;
  for (size_t n = 0; n < kNruns; n++) {
    timer.Start();
    for (size_t i = 0; i < kN; i ++)
      QuadSolveOptimized(a[i], b[i], c[i], x1[i], x2[i], roots[i]);
    t[n] = timer.Elapsed();
  }

  for (size_t n = 0; n < kNruns; n++)
    mean += t[n];

  mean = mean / (kN * kNruns);

#ifdef VERBOSE
  size_t index = (size_t)((kN - 100) * rand()/RAND_MAX);
  for (size_t i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%20s %6llu\n", "Optimized Scalar", mean);
}

#ifdef __AVX2__
VECCORE_FORCE_NOINLINE
void TestQuadSolveAVX2(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                       float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots, size_t kN)
{
  Timer<time_unit> timer;
  unsigned long long t[kNruns], mean = 0;
  for (size_t n = 0; n < kNruns; n++) {
    timer.Start();
    for (size_t i = 0; i < kN; i += 8)
      QuadSolveAVX(&a[i], &b[i], &c[i], &x1[i], &x2[i], &roots[i]);
    t[n] = timer.Elapsed();
  }

  for (size_t n = 0; n < kNruns; n++)
    mean += t[n];

  mean = mean / (kN * kNruns);

#ifdef VERBOSE
  size_t index = (size_t)((kN - 100) * rand()/RAND_MAX);
  for (size_t i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%20s %6llu\n", "AVX2 Intrinsics", mean);
}
#endif

template <class Backend>
VECCORE_FORCE_NOINLINE
void TestQuadSolve(const float *__restrict__ a, const float *__restrict__ b, const float *__restrict__ c,
                   float *__restrict__ x1, float *__restrict__ x2, int *__restrict__ roots, size_t kN, const char *name)
{
  using Float_v = typename Backend::Float_v;
  using Int32_v = typename Backend::Int32_v;

  Timer<time_unit> timer;
  unsigned long long t[kNruns], mean = 0;
  for (size_t n = 0; n < kNruns; n++) {
    timer.Start();
    for (size_t i = 0; i < kN; i += VectorSize<Float_v>())
      QuadSolveSIMD<Backend>(reinterpret_cast<const Float_v&>(a[i]),
                             reinterpret_cast<const Float_v&>(b[i]),
                             reinterpret_cast<const Float_v&>(c[i]),
                             reinterpret_cast<Float_v&>(x1[i]),
                             reinterpret_cast<Float_v&>(x2[i]),
                             reinterpret_cast<Int32_v&>(roots[i]));
    t[n] = timer.Elapsed();
  }

  for (size_t n = 0; n < kNruns; n++)
    mean += t[n];

  mean = mean / (kN * kNruns);

#ifdef VERBOSE
  size_t index = (size_t)((kN - 100) * rand()/RAND_MAX);
  for (size_t i = index; i < index + 10; i++)
    printf("%d: a = % 8.3f, b = % 8.3f, c = % 8.3f, roots = %d, x1 = % 8.3f, x2 = % 8.3f\n", i, a[i], b[i], c[i],
           roots[i], roots[i] > 0 ? x1[i] : 0, roots[i] > 1 ? x2[i] : 0);
#endif
  printf("%20s %6llu\n", name, mean);
}

int main()
{
  float *a   = (float *)AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(float));
  float *b   = (float *)AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(float));
  float *c   = (float *)AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(float));
  float *x1  = (float *)AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(float));
  float *x2  = (float *)AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(float));
  int *roots = (int *)AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(int));

  srand((unsigned)time(NULL));

  std::random_device rng;
  std::default_random_engine g(rng());
  std::uniform_real_distribution<float> dist(-10.0f, 10.0f);

  for (size_t i = 0; i < kN; i++) {
    a[i]     = dist(g);
    b[i]     = dist(g);
    c[i]     = 5.0f * dist(g);
    x1[i]    = 0.0f;
    x2[i]    = 0.0f;
    roots[i] = 0;
  }

  printf("             Backend / Mean (%s)\n", time_unit_name);

  TestQuadSolve(a, b, c, x1, x2, roots, kN);
  TestQuadSolveOptimized(a, b, c, x1, x2, roots, kN);

#ifdef __AVX2__
  TestQuadSolveAVX2(a, b, c, x1, x2, roots, kN);
#endif

  TestQuadSolve<backend::Scalar>(a, b, c, x1, x2, roots, kN, "Scalar Backend");
  TestQuadSolve<backend::ScalarWrapper>(a, b, c, x1, x2, roots, kN, "ScalarWrapper");

#ifdef VECCORE_ENABLE_VC
  TestQuadSolve<backend::VcScalar>(a, b, c, x1, x2, roots, kN, "VcScalar");
  TestQuadSolve<backend::VcVector>(a, b, c, x1, x2, roots, kN, "VcVector");
  TestQuadSolve<backend::VcSimdArray<8>>(a, b, c, x1, x2, roots, kN, "VcSimdArray<8>");
  TestQuadSolve<backend::VcSimdArray<16>>(a, b, c, x1, x2, roots, kN, "VcSimdArray<16>");
  TestQuadSolve<backend::VcSimdArray<32>>(a, b, c, x1, x2, roots, kN, "VcSimdArray<32>");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
  TestQuadSolve<backend::UMESimd>(a, b, c, x1, x2, roots, kN, "UME::SIMD");
  TestQuadSolve<backend::UMESimdArray<8>>(a, b, c, x1, x2, roots, kN, "UME::SIMD<8>");
  TestQuadSolve<backend::UMESimdArray<16>>(a, b, c, x1, x2, roots, kN, "UME::SIMD<16>");
  TestQuadSolve<backend::UMESimdArray<32>>(a, b, c, x1, x2, roots, kN, "UME::SIMD<32>");
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
  TestQuadSolve<backend::SIMDScalar>(a, b, c, x1, x2, roots, kN, "SIMDScalar");
  TestQuadSolve<backend::SIMDNative>(a, b, c, x1, x2, roots, kN, "SIMDNative");
  TestQuadSolve<backend::SIMDVector<4>>(a, b, c, x1, x2, roots, kN, "SIMDVector4");
  TestQuadSolve<backend::SIMDVector<8>>(a, b, c, x1, x2, roots, kN, "SIMDVector8");
#endif

  AlignedFree(a);
  AlignedFree(b);
  AlignedFree(c);
  AlignedFree(x1);
  AlignedFree(x2);
  AlignedFree(roots);

  return 0;
}
