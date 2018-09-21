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

template<typename T>
void mandelbrot_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                  Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                  Scalar<Index<T>> max_iter, unsigned char *image)
{
    T iota;
    for (size_t i = 0; i < VectorSize<T>(); ++i)
        Set<T>(iota, i, i);

    T dx = T(xmax - xmin) / T(nx);
    T dy = T(ymax - ymin) / T(ny), dyv = iota * dy;

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; j += VectorSize<T>()) {
            Scalar<Index<T>> k{0};
            T x = xmin + T(i) * dx,       cr = x, zr = x;
            T y = ymin + T(j) * dy + dyv, ci = y, zi = y;

            Index<T> kv{0};
            Mask<T> m{true};

            do {
                x = zr*zr - zi*zi + cr;
                y = T(2.0) * zr*zi + ci;
                MaskedAssign<T>(zr, m, x);
                MaskedAssign<T>(zi, m, y);
                MaskedAssign<Index<T>>(kv, m, ++k);
                m = zr*zr + zi*zi < T(4.0);
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
    printf("%15s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

template<typename T>
void bench_mandelbrot_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                        Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                        int max_iter, unsigned char *image, const char *backend)
{
    std::string filename = "mandelbrot_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    mandelbrot_v<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image);
    printf("%15s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

int main()
{
    double xmin = -2.1, xmax = 1.1;
    double ymin = -1.35, ymax = 1.35;

    size_t nx = 2400, ny = 2025, max_iter = 200;
    unsigned char *image = new unsigned char[nx*ny];

    /* single precision */

    bench_mandelbrot<float>(xmin, xmax, nx, ymin, ymax, ny,
                            max_iter, image, "float");

    bench_mandelbrot_v<float>(xmin, xmax, nx, ymin, ymax, ny,
                              max_iter, image, "float_v");

#ifdef VECCORE_ENABLE_VC
    bench_mandelbrot_v<backend::VcVector::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "float_vc");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_mandelbrot_v<backend::UMESimd::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                  max_iter, image, "float_umesimd");
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
    return 0;
}
