#ifndef VECCORE_TIMER_H
#define VECCORE_TIMER_H

#include <chrono>

using namespace std::chrono;

template <class unit = nanoseconds>
class Timer {
  using clock = high_resolution_clock;

public:
  Timer() : fStart(), fStop() { Start(); }

  void Reset() { Start(); }

  void Start() { fStart = clock::now(); }

  double Elapsed()
  {
    fStop = clock::now();
    return duration_cast<unit>(fStop - fStart).count();
  }

private:
  high_resolution_clock::time_point fStart, fStop;
};

#if !defined(__CUDA_ARCH__) && !defined(__NVCOMPILER) \
 && (defined(__x86_64__) || defined(_M_X64) \
    || defined(__i386__) || defined(_M_IX86))

#define VECCORE_TIMER_CYCLES

#ifdef _MSC_VER
#  include <intrin.h>
#else
#  include <x86intrin.h>
#endif

class cycles {
};

template <>
class Timer<cycles> {
public:
  Timer() { Start(); }

  void Reset() { Start(); }

  void Start() { fStart = GetCycleCount(); }

  double Elapsed() { return static_cast<double>(GetCycleCount() - fStart); }

private:
  unsigned long long int fStart;

  inline unsigned long long int GetCycleCount()
  {
     return __rdtsc();
  }
};
#endif

#endif
