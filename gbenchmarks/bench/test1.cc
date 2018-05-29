#include <benchmark/benchmark.h>
#include <cmath>
#include <VecCore/VecCore>

using namespace vecCore;

static void LIBMVEC_Exp(benchmark::State &state)
{
  float *input;
  input = (float *)AlignedAlloc(VECCORE_SIMD_ALIGN, 8192 * sizeof(float));
  using Float_v = backend::Scalar::Float_v;

  for(int i = 0; i < 8192; i++)
  {
    input[i] = 0.123f;
  }

  while (state.KeepRunning())
  {
      for(size_t i = 0; i < 8192; i += VectorSize<Float_v>())
      {
        math::Exp((Float_v &)(input[i]));
      }
  }
}

// Register the function as a benchmark
BENCHMARK(LIBMVEC_Exp)->Range(1, 1);

BENCHMARK_MAIN();
