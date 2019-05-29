#include "png.h"

#include <cstdio>
#include <cstdlib>
#include <string>

#include "timer.h"
#include <VecCore/VecCore>

using namespace vecCore;

/*
    calculates Newton fractal for f(z) = z^4 - 1
    https://en.wikipedia.org/wiki/Newton_fractal
*/

Color COLORS[] = {
    {0, 0, 0, 0},     // black
    {0, 255, 255, 0}, // yellow
    {0, 255, 0, 0},   // red
    {0, 0, 255, 0},   // green
    {0, 0, 0, 255}    // blue
};

template <typename T>
bool is_equal(T re1, T im1, T re2, T im2)
{
    const T tol2 = 0.0001;
    return ((re1 - re2) * (re1 - re2) + (im1 - im2) * (im1 - im2)) < tol2;
}

template <typename T>
bool converged(T re, T im, int iters, uint8_t& color_index, uint8_t& alpha)
{
    alpha = std::min(iters * 10, 100);

    if (is_equal(re, im, T(1), T(0))) {
        color_index = 1;
        return true;
    }

    if (is_equal(re, im, T(-1), T(0))) {
        color_index = 2;
        return true;
    }

    if (is_equal(re, im, T(0), T(1))) {
        color_index = 3;
        return true;
    }

    if (is_equal(re, im, T(0), T(-1))) {
        color_index = 4;
        return true;
    }

    return false;
}

template<typename T>
void newton(T xmin, T xmax, size_t nx,
            T ymin, T ymax, size_t ny,
            size_t max_iter, Color *image)
{
    T dx = (xmax - xmin) / T(nx);
    T dy = (ymax - ymin) / T(ny);
    
    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; ++j) {
            uint8_t color_index = 0, alpha = 0;
            T re = xmin + T(i) * dx, x = re;
            T im = ymin + T(j) * dy, y = im;
            bool has_converged = false;

            for (size_t k = 0; !has_converged && (k < max_iter); ) {
                T re2 = re * re, re3 = re2 * re, re4 = re3 * re, re5 = re4 * re, re6 = re5 * re, re7 = re6 * re;
                T im2 = im * im, im3 = im2 * im, im4 = im3 * im, im5 = im4 * im, im6 = im5 * im, im7 = im6 * im;
                T coeff = T(0.25) / ((re2 + im2) * (re2 + im2) * (re2 + im2));

                x = T(3) * re * im2 + re * im6 + re7 + T(3) * re5 * im2 - re3 + T(3) * re3 * im4;
                y = T(3) * re2 * im + im7 + T(3) * re2 * im5 - im3 + re6 * im + T(3) * re4 * im3;

                re -= x * coeff;
                im -= y * coeff;
                has_converged = converged(re, im, k, color_index, alpha);
                ++k;
            }
            Color color = COLORS[color_index];
            color.alpha = alpha;
            image[ny * i + j] = color;
        }
    }
}


template <typename T>
Mask<T> is_equal_v(T re1, T im1, T re2, T im2)
{
    const T tol2 = 0.0001;
    return ((re1 - re2) * (re1 - re2) + (im1 - im2) * (im1 - im2)) < tol2;
}

template <typename T>
Mask<T> converged_v(T re, T im, Index<T> iters, Index<T>& color_index, Index<T>& alphas)
{
    alphas = iters * 10;
    MaskedAssign<Index<T>>(alphas, alphas > 100, 100);

    Mask<T> m0 = is_equal_v(re, im, T(1), T(0));
    MaskedAssign<Index<T>>(color_index, m0, 1);

    Mask<T> m1 = is_equal_v(re, im, T(-1), T(0));
    MaskedAssign<Index<T>>(color_index, m1, 2);

    Mask<T> m2 = is_equal_v(re, im, T(0), T(1));
    MaskedAssign<Index<T>>(color_index, m2, 3);

    Mask<T> m3 = is_equal_v(re, im, T(0), T(-1));
    MaskedAssign<Index<T>>(color_index, m3, 4);

    return m0 || m1 || m2 || m3;
}

template<typename T>
void newton_v(T xmin, T xmax, size_t nx,
              T ymin, T ymax, size_t ny,
              size_t max_iter, Color *image)
{
    T iota;
    for (size_t i = 0; i < VectorSize<T>(); ++i) {
        Set<T>(iota, i, i);
    }

    T dx = T(xmax - xmin) / T(nx);
    T dy = T(ymax - ymin) / T(ny), dyv = iota * dy;
    
    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; j += VectorSize<T>()) {
            T re = xmin + T(i) * dx,       x = re;
            T im = ymin + T(j) * dy + dyv, y = im;

            Index<T> kv{0};
            Index<T> color_index{0};
            Index<T> alphas{0};
            Mask<T> m{false};

            for (size_t k = 0; !MaskFull(m) && (k < max_iter); ) {
                T re2 = re * re, re3 = re2 * re, re4 = re3 * re, re5 = re4 * re, re6 = re5 * re, re7 = re6 * re;
                T im2 = im * im, im3 = im2 * im, im4 = im3 * im, im5 = im4 * im, im6 = im5 * im, im7 = im6 * im;
                T coeff = T(0.25) / ((re2 + im2) * (re2 + im2) * (re2 + im2));

                x = T(3) * re * im2 + re * im6 + re7 + T(3) * re5 * im2 - re3 + T(3) * re3 * im4;
                y = T(3) * re2 * im + im7 + T(3) * re2 * im5 - im3 + re6 * im + T(3) * re4 * im3;

                re -= x * coeff;
                im -= y * coeff;
                m = converged_v(re, im, kv, color_index, alphas);
                MaskedAssign<Index<T>>(kv, !m, ++k);
            }

            for (size_t k = 0; k < VectorSize<T>(); ++k) {
                uint8_t ci = Get(color_index, k);
                uint8_t alpha = Get(alphas, k);
                Color color = COLORS[ci];
                color.alpha = alpha;
                image[ny * i + j + k] = color;
            }
        }
    }
}

template<typename T>
void bench_newton(T xmin, T xmax, size_t nx, 
                  T ymin, T ymax, size_t ny,
                  int max_iter, Color *image, const char *backend)
{
    std::string filename = "newton_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    newton<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image);
    printf("%15s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

template<typename T>
void bench_newton_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                    Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                    int max_iter, Color *image, const char *backend)
{
    std::string filename = "newton_" + std::string(backend) + ".png";
    Timer<milliseconds> timer;
    newton_v<T>(xmin, xmax, nx, ymin, ymax, ny, max_iter, image);
    printf("%15s: %7.2lf ms\n", backend, timer.Elapsed());
    write_png(filename.c_str(), image, nx, ny);
}

int main()
{
    double xmin = -2, xmax = 2;
    double ymin = -2, ymax = 2;

    size_t nx = 1024, ny = 1024, max_iter = 1000;
    Color *image = new Color[nx * ny];

    /* single precision */

    bench_newton<float>(xmin, xmax, nx, ymin, ymax, ny, 
                        max_iter, image, "float");

    bench_newton_v<float>(xmin, xmax, nx, ymin, ymax, ny,
                          max_iter, image, "float_v");


#ifdef VECCORE_ENABLE_VC
    bench_newton_v<backend::VcVector::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                               max_iter, image, "float_vc");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_newton_v<backend::UMESimd::Float_v>(xmin, xmax, nx, ymin, ymax, ny,
                                              max_iter, image, "float_umesimd");
#endif

    /* double precision */

    bench_newton<double>(xmin, xmax, nx, ymin, ymax, ny,
                         max_iter, image, "double");

    bench_newton_v<backend::Scalar::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                              max_iter, image, "double_v");

#ifdef VECCORE_ENABLE_VC
    bench_newton_v<backend::VcVector::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                                max_iter, image, "double_vc");
#endif

#ifdef VECCORE_ENABLE_UMESIMD
    bench_newton_v<backend::UMESimd::Double_v>(xmin, xmax, nx, ymin, ymax, ny,
                                               max_iter, image, "double_umesimd");
#endif

    return 0;
}
