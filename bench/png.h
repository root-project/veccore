#ifndef VECCORE_BENCH_PNG_H
#define VECCORE_BENCH_PNG_H

#include <cstddef>
#include <cstdint>

struct Color {
    uint8_t alpha;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void write_png(const char *filename, Color *data, size_t nx, size_t ny);
void write_png(const char *filename, unsigned char *data, size_t nx, size_t ny);

#endif
