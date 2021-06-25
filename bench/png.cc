#include "png.h"

#include <cstdio>
#include <cstddef>

#ifndef HAVE_GD

void write_png(const char *filename, Color *data, size_t nx, size_t ny)
{
   /* empty */
}

void write_png(const char *filename, unsigned char *data, size_t nx, size_t ny)
{
   /* empty */
}

#else /* HAVE_GD */

#include <cmath>
#include <gd.h>

void colormap(int c, int& cr, int& cg, int& cb)
{
    double x = 1.0 - 0.7*std::pow(1.0 - c/256.0, 10.0);

    c = x < 0.50 ? (x < 0.25 ? 0 : 1) : (x < 0.75 ? 2 : 3);

    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    switch(c) {
        case 0:
            r = 0.0;
            g = 4.0*x;
            b = 1.0;
            break;
        case 1:
            r = 0.0;
            g = 1.0;
            b = 4.0*(0.25 - x);
            break;
        case 2:
            r = 4.0*(x-0.5);
            g = 1.0;
            b = 0.0;
            break;
        case 3:
            r = 1.0;
            g = 4.0*(1.0-x);
            b = 0.0;
            break;
    }

    cr = 256 * r;
    cg = 256 * g;
    cb = 256 * b;
}

void write_png(const char *filename, unsigned char *data, size_t nx, size_t ny)
{
    FILE *output = fopen(filename, "wb");

    if (!output) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return;
    }

    gdImagePtr image = gdImageCreate(nx, ny);

    if (!image) {
        fprintf(stderr, "Error: cannot create image\n");
        fclose(output);
        return;
    }

    int colors[256];

    for (size_t i = 0; i < 256; ++i) {
        int r = 0, g = 0, b = 0;
        colormap(i, r, g, b);
        colors[i] = gdImageColorAllocate(image, r, g, b);
    }

    for (size_t i = 0; i < nx; ++i)
        for (size_t j = 0; j < ny; ++j)
            gdImageSetPixel(image, i, j, colors[data[ny*(nx - i - 1) + j]]);

    gdImagePng(image, output);
    gdImageDestroy(image);
    fclose(output);
}

void write_png(const char *filename, Color *data, size_t nx, size_t ny)
{
    FILE *output = fopen(filename, "wb");

    if (!output) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return;
    }

    gdImagePtr image = gdImageCreateTrueColor(nx, ny);

    if (!image) {
        fprintf(stderr, "Error: cannot create image\n");
        fclose(output);
        return;
    }

    for (size_t i = 0; i < nx; ++i) {
       for (size_t j = 0; j < ny; ++j) {
          const auto& pixel = data[ny * i + j];
          gdImageSetPixel(image, i, j,
                gdTrueColorAlpha(pixel.red, pixel.green, pixel.blue, pixel.alpha));
       }
    }

    gdImagePng(image, output);
    gdImageDestroy(image);
    fclose(output);
}

#endif
