#include <cstdio>
#include <cstdlib>
#include <string>

#include "timer.h"
#include <VecCore/VecCore>

using namespace vecCore;

extern void write_png(const char *filename, unsigned char *data, size_t nx, size_t ny);

template<typename T>
void mandelbrot(T xmin, T xmax, size_t nx,
                T ymin, T ymax, size_t ny,
                size_t max_iter, unsigned char *image)
{
    T dx = (xmax - xmin) / T(nx);
    T dy = (ymax - ymin) / T(ny);

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; ++j) {
            size_t k = 0;
            T x = xmin + T(i) * dx, cr = x, zr = x;
            T y = ymin + T(j) * dy, ci = y, zi = y;

            do {
                x  = zr*zr - zi*zi + cr;
                y  = T(2.0) * zr*zi + ci;
                zr = x;
                zi = y;
            } while (++k < max_iter && (zr*zr + zi*zi < T(4.0)));

            image[ny*i + j] = k;
        }
    }
}

#ifdef __AVX2__
void mandelbrot_avx2(float xmin, float xmax, size_t nx,
                     float ymin, float ymax, size_t ny,
                     size_t max_iter, unsigned char *image)
{
    __m256 dx  = _mm256_set1_ps((xmax - xmin) / float(nx));
    __m256 dy  = _mm256_set1_ps((ymax - ymin) / float(ny));
    __m256 dyv = _mm256_mul_ps(dy, _mm256_set_ps(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f));

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; j += 8) {
            size_t k = 0;
#ifdef __FMA__
            __m256 x = _mm256_fmadd_ps(_mm256_set1_ps(i), dx, _mm256_set1_ps(xmin));
            __m256 y = _mm256_fmadd_ps(_mm256_set1_ps(j), dy, _mm256_add_ps(_mm256_set1_ps(ymin), dyv));
#else
            __m256 x = _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(i), dx), _mm256_set1_ps(xmin));
            __m256 y = _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(j), dy), _mm256_add_ps(_mm256_set1_ps(ymin), dyv));
#endif
            __m256 cr = x, zr = x, ci = y, zi = y;

            __m256i kv = _mm256_setzero_si256();
            __m256 mask = _mm256_andnot_ps(_mm256_setzero_ps(), _mm256_setzero_ps());

            do {
                __m256 zr2 = _mm256_mul_ps(zr, zr);
                __m256 zi2 = _mm256_mul_ps(zi, zi);
                __m256 zri = _mm256_mul_ps(zr, zi);
                x = _mm256_add_ps(_mm256_sub_ps(zr2,zi2), cr);
#ifdef __FMA__
                y = _mm256_fmadd_ps(_mm256_set1_ps(2.0f), zri, ci);
#else
                y = _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(2.0f), zri), ci);
#endif
                zr = _mm256_blendv_ps(zr, x, mask);
                zi = _mm256_blendv_ps(zi, y, mask);
                kv = _mm256_blendv_epi8(kv, _mm256_set1_epi32(++k), _mm256_castps_si256(mask));
                mask = _mm256_cmp_ps(_mm256_add_ps(_mm256_mul_ps(zr, zr), _mm256_mul_ps(zi, zi)),
                  _mm256_set1_ps(4.0f), _CMP_NGE_UQ);
            } while (k < max_iter && !_mm256_testz_ps(mask, mask));

            for (k = 0; k < 8; ++k)
                image[ny*i + j + k] = (unsigned char) *((int*)(&kv)+k);
        }
    }
}
#endif

template<typename T>
void mandelbrot_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                  Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                  Scalar<Index<T>> max_iter, unsigned char *image)
{
    T iota;
    for (size_t i = 0; i < VectorSize<T>(); ++i)
        Set<T>(iota, i, i);

    T dx = T(xmax - xmin) / T((Scalar<T>)nx);
    T dy = T(ymax - ymin) / T((Scalar<T>)ny), dyv = iota * dy;

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; j += VectorSize<T>()) {
            Scalar<Index<T>> k(0);
            T x = xmin + T((Scalar<T>)i) * dx,       cr = x, zr = x;
            T y = ymin + T((Scalar<T>)j) * dy + dyv, ci = y, zi = y;

            Index<T> kv(0);
            Mask<T> m(true);

            do {
                x = zr*zr - zi*zi + cr;
                y = T((Scalar<T>)2.0) * zr*zi + ci;
                MaskedAssign<T>(zr, m, x);
                MaskedAssign<T>(zi, m, y);
                MaskedAssign<Index<T>>(kv, m, ++k);
                m = zr*zr + zi*zi < T((Scalar<T>)4.0);
            } while (k < max_iter && !MaskEmpty(m));

            for (size_t k = 0; k < VectorSize<T>(); ++k)
                image[ny*i + j + k] = (unsigned char) Get(kv, k);
        }
    }
}

template<typename T>
void bench_mandelbrot(T xmin, T xmax, size_t nx, T ymin, T ymax, size_t ny,
                      int max_iter, unsigned char *image, const char *backend)
{
    std::string filename = "mandelbrot_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    mandelbrot<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image);
    printf("%25s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

#ifdef __AVX2__
void bench_mandelbrot_avx2(float xmin, float xmax, size_t nx,
                           float ymin, float ymax, size_t ny,
                           int max_iter, unsigned char *image,
                           const char *backend)
{
    std::string filename = "mandelbrot_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    mandelbrot_avx2(xmin, xmax, nx, ymin, ymax, ny, max_iter, image);
    printf("%25s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}
#endif

template<typename T>
void bench_mandelbrot_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                        Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                        int max_iter, unsigned char *image, const char *backend)
{
    std::string filename = "mandelbrot_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    mandelbrot_v<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image);
    printf("%25s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

int main()
{
    double xmin = -2.1, xmax = 1.1;
    double ymin = -1.35, ymax = 1.35;

    size_t nx = 1024, ny = 864, max_iter = 500;
    unsigned char *image = new unsigned char[nx*ny];

    /* single precision */

    bench_mandelbrot<float>(xmin, xmax, nx, ymin, ymax, ny,
                            max_iter, image, "float");

    bench_mandelbrot_v<float>(xmin, xmax, nx, ymin, ymax, ny,
                              max_iter, image, "float_v");

#ifdef __AVX2__
    bench_mandelbrot_avx2(xmin, xmax, nx, ymin, ymax, ny, max_iter, image, "float_avx2");
#endif

#ifdef VECCORE_ENABLE_VC
    bench_mandelbrot_v<backend::VcVector::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "float_vc");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_mandelbrot_v<backend::UMESimd::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                  max_iter, image, "float_umesimd");
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
    bench_mandelbrot_v<backend::SIMDScalar::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_scalar_float");

    bench_mandelbrot_v<backend::SIMDNative::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_native_float");

    bench_mandelbrot_v<backend::SIMDVector<4>::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_vector4_float");

    bench_mandelbrot_v<backend::SIMDVector<8>::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_vector8_float");
#endif

    /* double precision */

    bench_mandelbrot<double>(xmin, xmax, nx, ymin, ymax, ny,
                             max_iter, image, "double");

    bench_mandelbrot_v<backend::Scalar::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                  max_iter, image, "double_v");

#ifdef VECCORE_ENABLE_VC
    bench_mandelbrot_v<backend::VcVector::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                    max_iter, image, "double_vc");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_mandelbrot_v<backend::UMESimd::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "double_umesimd");
#endif

#ifdef VECCORE_ENABLE_STD_SIMD
    bench_mandelbrot_v<backend::SIMDScalar::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_scalar_double");

    bench_mandelbrot_v<backend::SIMDNative::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_native_double");

    bench_mandelbrot_v<backend::SIMDVector<4>::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_vector4_double");

    bench_mandelbrot_v<backend::SIMDVector<8>::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "std_simd_vector8_double");
#endif

    delete[] image;
    return 0;
}
