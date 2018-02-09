#include <cstdio>
#include <cstdlib>
#include <string>

#include <VecCore/Timer.h>
#include <VecCore/VecCore>

using namespace vecCore;

extern void write_png(const char *filename, unsigned char *data, size_t nx, size_t ny);

template<typename T>
void julia(T xmin, T xmax, size_t nx,
           T ymin, T ymax, size_t ny,
           size_t max_iter, unsigned char *image, T real, T im)
{
    T dx = (xmax - xmin) / T(nx);
    T dy = (ymax - ymin) / T(ny);

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; ++j) {
            size_t k = 0;
            T x = xmin + T(i) * dx, cr = real, zr = x;
            T y = ymin + T(j) * dy, ci = im, zi = y;

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
void julia_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
             Scalar<T> ymin, Scalar<T> ymax, size_t ny,
             Scalar<Index<T>> max_iter, unsigned char *image, Scalar<T> real, Scalar<T> im)
{
    T iota;
    for (size_t i = 0; i < VectorSize<T>(); ++i)
        Set<T>(iota, i, i);

    T dx = T(xmax - xmin) / T(nx);
    T dy = T(ymax - ymin) / T(ny), dyv = iota * dy;

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; j += VectorSize<T>()) {
            Scalar<Index<T>> k{0};
            T x = xmin + T(i) * dx,       cr = real, zr = x;
            T y = ymin + T(j) * dy + dyv, ci = im, zi = y;

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
void bench_julia(T xmin, T xmax, size_t nx, T ymin, T ymax, size_t ny,
                 int max_iter, unsigned char *image, const char *backend, T cr, T ci)
{
    std::string filename = "julia_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    julia<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image, cr, ci);
    printf("%15s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

template<typename T>
void bench_julia_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                   Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                   int max_iter, unsigned char *image, const char *backend, Scalar<T> cr, Scalar<T> ci)
{
    std::string filename = "julia_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    julia_v<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image, cr, ci);
    printf("%15s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

int main(int argc, char *argv[])
{
    double xmin = -2, xmax = 2;
    double ymin = -2, ymax = 2;

    size_t nx = 2400, ny = 2025, max_iter = 200;
    double cr = 0.285, ci = 0.01;
    unsigned char *image = new unsigned char[nx*ny];

    if (argc > 1) {
        if (argc != 3) {
           fprintf(stderr, "%s: incorrect number of parameters\n\n"
                           "Usage: julia [<Re(c)> <Im(c)>]\n\n\twhere f(z) = z*z + c\n",
                           argv[0]);
           return 1;
        } else {
            cr = atof(argv[1]);
            ci = atof(argv[2]);
        }
    }

    /* single precision */

    bench_julia<float>(xmin, xmax, nx, ymin, ymax, ny,
                       max_iter, image, "float", cr, ci);

    bench_julia_v<float>(xmin, xmax, nx, ymin, ymax, ny,
                         max_iter, image, "float_v", cr, ci);

#ifdef VECCORE_ENABLE_VC
    bench_julia_v<backend::VcVector::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                              max_iter, image, "float_vc", cr, ci);
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_julia_v<backend::UMESimd::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                             max_iter, image, "float_umesimd", cr, ci);
#endif

    /* double precision */

    bench_julia<double>(xmin, xmax, nx, ymin, ymax, ny,
                             max_iter, image, "double", cr, ci);

    bench_julia_v<backend::Scalar::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                  max_iter, image, "double_v", cr, ci);

#ifdef VECCORE_ENABLE_VC
    bench_julia_v<backend::VcVector::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                    max_iter, image, "double_vc", cr, ci);
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_julia_v<backend::UMESimd::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                   max_iter, image, "double_umesimd", cr, ci);
#endif
    return 0;
}
