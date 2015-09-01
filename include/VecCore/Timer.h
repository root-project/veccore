#include <chrono>

class TimerInterface {
  public:
	virtual ~TimerInterface() {}
    virtual void Start() = 0;
    virtual void Reset() = 0;
    virtual double Elapsed() = 0;
};

template <class duration_unit = std::chrono::nanoseconds>
class Timer : public TimerInterface {
  public:
    Timer() : fStart(), fStop() { Start(); }
	~Timer() { }

    inline void Reset() { Start(); }

    inline void Start()
    {
        fStart = std::chrono::high_resolution_clock::now();
    }

    inline double Elapsed()
    {
        fStop = std::chrono::high_resolution_clock::now();
        duration_unit t_elapsed =
        std::chrono::duration_cast<duration_unit>(fStop - fStart);
        return t_elapsed.count();
    }

  private:
    std::chrono::high_resolution_clock::time_point fStart;
    std::chrono::high_resolution_clock::time_point fStop;
};

#if !defined(VECCORE_CUDA_DEVICE)
class CPUCycleTimer {
  public:
    CPUCycleTimer() { Start(); }
	~CPUCycleTimer() { }

    inline void Reset() { Start(); }

    inline void Start() { fStart = CurrentCycleCount(); }

    inline double Elapsed()
    {
        return static_cast<double>(CurrentCycleCount() - fStart);
    }

  private:
    unsigned long long int fStart;

    inline __attribute__((always_inline))
    unsigned long long int CurrentCycleCount()
    {
        unsigned int hi, lo;
        asm volatile("cpuid\n\t" "rdtsc" : "=a"(lo), "=d"(hi));
        return ((unsigned long long)lo)|(((unsigned long long)hi) << 32);
    }
};
#endif
