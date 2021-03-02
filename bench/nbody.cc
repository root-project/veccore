#include <cmath>
#include <cstdio>
#include <random>
#include <memory>
#include <numeric>
#include <random>

#include "timer.h"
#include <VecCore/VecCore>

using namespace vecCore;

constexpr auto kNruns = 1;
constexpr auto kN = 1024;

#ifdef VECCORE_TIMER_CYCLES
using time_unit = cycles;
constexpr auto time_unit_name = "cycles";
#else
using time_unit = nanoseconds;
constexpr auto time_unit_name = "ns";
#endif

constexpr float eps2 = 0.01f;
constexpr float timeStep = 0.0001f;

inline void pPInteraction(float p1posx, float p1posy, float p1posz, float& p1velx, float& p1vely, float& p1velz,
                          float p2posx, float p2posy, float p2posz, float p2mass) {
    const float xdistance = p1posx - p2posx;
    const float ydistance = p1posy - p2posy;
    const float zdistance = p1posz - p2posz;
    const float xdistanceSqr = xdistance * xdistance;
    const float ydistanceSqr = ydistance * ydistance;
    const float zdistanceSqr = zdistance * zdistance;
    const float distSqr = eps2 + xdistanceSqr + ydistanceSqr + zdistanceSqr;
    const float distSixth = distSqr * distSqr * distSqr;
    const float invDistCube = 1.0f / std::sqrt(distSixth);
    const float sts = p2mass * invDistCube * timeStep;
    p1velx += xdistanceSqr * sts;
    p1vely += ydistanceSqr * sts;
    p1velz += zdistanceSqr * sts;
}

VECCORE_FORCE_NOINLINE
void TestNBody(float* __restrict__ posx, float* __restrict__ posy, float* __restrict__ posz,
               float* __restrict__ velx, float* __restrict__ vely, float* __restrict__ velz,
               float* __restrict__ mass, size_t kN)
{
    Timer<time_unit> timer;
    unsigned long long t[kNruns];
    for (size_t n = 0; n < kNruns; n++) {
        timer.Start();

        for (std::size_t i = 0; i < kN; i++)
            for (std::size_t j = 0; j < kN; j++)
                pPInteraction(posx[j], posy[j], posz[j], velx[j], vely[j], velz[j],
                    posx[i], posy[i], posz[i], mass[i]);
        for (std::size_t i = 0; i < kN; i++) {
            posx[i] += velx[i] * timeStep;
            posy[i] += vely[i] * timeStep;
            posz[i] += velz[i] * timeStep;
        }

      t[n] = timer.Elapsed();
    }

    const unsigned long long mean = std::accumulate(std::begin(t), std::end(t), 0ull) / (kN * kNruns);
    printf("%20s %6llu\n", "Scalar", mean);
}

#ifdef __AVX2__
const __m256 vEps2 = _mm256_set1_ps(eps2);
const __m256 vTimestep = _mm256_broadcast_ss(&timeStep);

inline void pPInteractionAVX2(
    __m256 p1posx,
    __m256 p1posy,
    __m256 p1posz,
    __m256& p1velx,
    __m256& p1vely,
    __m256& p1velz,
    __m256 p2posx,
    __m256 p2posy,
    __m256 p2posz,
    __m256 p2mass)
{
    const __m256 xdistance = _mm256_sub_ps(p1posx, p2posx);
    const __m256 ydistance = _mm256_sub_ps(p1posy, p2posy);
    const __m256 zdistance = _mm256_sub_ps(p1posz, p2posz);
    const __m256 xdistanceSqr = _mm256_mul_ps(xdistance, xdistance);
    const __m256 ydistanceSqr = _mm256_mul_ps(ydistance, ydistance);
    const __m256 zdistanceSqr = _mm256_mul_ps(zdistance, zdistance);
    const __m256 distSqr = _mm256_add_ps(_mm256_add_ps(_mm256_add_ps(vEps2, xdistanceSqr), ydistanceSqr), zdistanceSqr);
    const __m256 distSixth = _mm256_mul_ps(_mm256_mul_ps(distSqr, distSqr), distSqr);
    const __m256 invDistCube = _mm256_div_ps(_mm256_set1_ps(1.0f), _mm256_sqrt_ps(distSixth));
    const __m256 sts = _mm256_mul_ps(_mm256_mul_ps(p2mass, invDistCube), vTimestep);
    p1velx = _mm256_fmadd_ps(xdistanceSqr, sts, p1velx);
    p1vely = _mm256_fmadd_ps(ydistanceSqr, sts, p1vely);
    p1velz = _mm256_fmadd_ps(zdistanceSqr, sts, p1velz);
}

VECCORE_FORCE_NOINLINE
void TestNBodyAVX2(float* __restrict__ posx, float* __restrict__ posy, float* __restrict__ posz,
                   float* __restrict__ velx, float* __restrict__ vely, float* __restrict__ velz,
                   float* __restrict__ mass, size_t kN)
{
    Timer<time_unit> timer;
    unsigned long long t[kNruns];
    for (size_t n = 0; n < kNruns; n++) {
        timer.Start();

        for (std::size_t i = 0; i < kN; i++) {
            for (std::size_t j = 0; j < kN; j += VectorSize<__m256>()) {
                __m256 p1velx = _mm256_load_ps(&velx[j]);
                __m256 p1vely = _mm256_load_ps(&vely[j]);
                __m256 p1velz = _mm256_load_ps(&velz[j]);
                pPInteractionAVX2(
                    _mm256_load_ps(&posx[j]),
                    _mm256_load_ps(&posy[j]),
                    _mm256_load_ps(&posz[j]),
                    p1velx,
                    p1vely,
                    p1velz,
                    _mm256_broadcast_ss(&posx[i]),
                    _mm256_broadcast_ss(&posy[i]),
                    _mm256_broadcast_ss(&posz[i]),
                    _mm256_broadcast_ss(&mass[i])
                );
                _mm256_store_ps(&velx[j], p1velx);
                _mm256_store_ps(&vely[j], p1vely);
                _mm256_store_ps(&velz[j], p1velz);
            }
        }
        for (std::size_t i = 0; i < kN; i += VectorSize<__m256>()) {
            _mm256_store_ps(&posx[i], _mm256_fmadd_ps(_mm256_load_ps(&velx[i]), vTimestep, _mm256_load_ps(&posx[i])));
            _mm256_store_ps(&posy[i], _mm256_fmadd_ps(_mm256_load_ps(&vely[i]), vTimestep, _mm256_load_ps(&posy[i])));
            _mm256_store_ps(&posz[i], _mm256_fmadd_ps(_mm256_load_ps(&velz[i]), vTimestep, _mm256_load_ps(&posz[i])));
        }

        t[n] = timer.Elapsed();
    }

    const unsigned long long mean = std::accumulate(std::begin(t), std::end(t), 0ull) / (kN * kNruns);
    printf("%20s %6llu\n", "AVX2 Intrinsics", mean);
}
#endif

template <typename Vec>
inline void pPInteractionSIMD(Vec p1posx, Vec p1posy, Vec p1posz, Vec& p1velx, Vec& p1vely, Vec& p1velz,
                              Vec p2posx, Vec p2posy, Vec p2posz, Vec p2mass)
{
    const Vec xdistance = p1posx - p2posx;
    const Vec ydistance = p1posy - p2posy;
    const Vec zdistance = p1posz - p2posz;
    const Vec xdistanceSqr = xdistance * xdistance;
    const Vec ydistanceSqr = ydistance * ydistance;
    const Vec zdistanceSqr = zdistance * zdistance;
    const Vec distSqr = eps2 + xdistanceSqr + ydistanceSqr + zdistanceSqr;
    const Vec distSixth = distSqr * distSqr * distSqr;
    const Vec invDistCube = 1.0f / math::Sqrt(distSixth);
    const Vec sts = p2mass * invDistCube * timeStep;
    p1velx += xdistanceSqr * sts;
    p1vely += ydistanceSqr * sts;
    p1velz += zdistanceSqr * sts;
}

template <typename Vec>
VECCORE_FORCE_NOINLINE
void TestNBodySIMD(float* __restrict__ posx, float* __restrict__ posy, float* __restrict__ posz,
                   float* __restrict__ velx, float* __restrict__ vely, float* __restrict__ velz,
                   float* __restrict__ mass, size_t kN, const char* name)
{
    Timer<time_unit> timer;
    unsigned long long t[kNruns];
    for (size_t n = 0; n < kNruns; n++) {
        timer.Start();

        for (std::size_t i = 0; i < kN; i++) {
            for (std::size_t j = 0; j < kN; j += VectorSize<Vec>()) {
                pPInteractionSIMD((Vec&)(posx[j]), (Vec&)(posy[j]), (Vec&)(posz[j]),
                                  (Vec&)(velx[j]), (Vec&)(vely[j]), (Vec&)(velz[j]),
                                  Vec(posx[i]), Vec(posy[i]), Vec(posz[i]), Vec(mass[i]));
            }
        }
        for (std::size_t i = 0; i < kN; i += VectorSize<Vec>()) {
             (Vec&)(posx[i]) += (const Vec&)(velx[i]) * timeStep;
             (Vec&)(posy[i]) += (const Vec&)(vely[i]) * timeStep;
             (Vec&)(posz[i]) += (const Vec&)(velz[i]) * timeStep;
        }

        t[n] = timer.Elapsed();
    }

    const unsigned long long mean = std::accumulate(std::begin(t), std::end(t), 0ull) / (kN * kNruns);
    printf("%20s %6llu\n", name, mean);
}

int main()
{
    auto alloc = [] {
        return std::unique_ptr<float[], decltype(&AlignedFree)>{
            static_cast<float*>(AlignedAlloc(VECCORE_SIMD_ALIGN, kN * sizeof(float))), &AlignedFree
        };
    };
    auto posx = alloc();
    auto posy = alloc();
    auto posz = alloc();
    auto velx = alloc();
    auto vely = alloc();
    auto velz = alloc();
    auto mass = alloc();

    std::random_device rng;
    std::default_random_engine engine{rng()};
    std::normal_distribution<float> dist{0.0f, 1.0f};

    for (std::size_t i = 0; i < kN; ++i) {
        posx[i] = dist(engine);
        posy[i] = dist(engine);
        posz[i] = dist(engine);
        velx[i] = dist(engine) / 10.0f;
        vely[i] = dist(engine) / 10.0f;
        velz[i] = dist(engine) / 10.0f;
        mass[i] = dist(engine) / 100.0f;
    }

    printf("             Backend / Mean (%s)\n", time_unit_name);

    TestNBody(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN);

#ifdef __AVX2__
    TestNBodyAVX2(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN);
#endif

    TestNBodySIMD<backend::Scalar::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "Scalar Backend");
    TestNBodySIMD<backend::ScalarWrapper::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "ScalarWrapper");

    #ifdef VECCORE_ENABLE_VC
    TestNBodySIMD<backend::VcScalar::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "VcScalar");
    TestNBodySIMD<backend::VcVector::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "VcVector");
    TestNBodySIMD<backend::VcSimdArray<8>::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "VcSimdArray<8>");
    TestNBodySIMD<backend::VcSimdArray<16>::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "VcSimdArray<16>");
    TestNBodySIMD<backend::VcSimdArray<32>::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "VcSimdArray<32>");
    #endif

    #ifdef VECCORE_ENABLE_UMESIMD
    TestNBodySIMD<backend::UMESimd::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "UME::SIMD");
    TestNBodySIMD<backend::UMESimdArray<8>::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "UME::SIMD<8>");
    TestNBodySIMD<backend::UMESimdArray<16>::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "UME::SIMD<16>");
    TestNBodySIMD<backend::UMESimdArray<32>::Float_v>(posx.get(), posy.get(), posz.get(), velx.get(), vely.get(), velz.get(), mass.get(), kN, "UME::SIMD<32>");
    #endif

    return 0;
}
