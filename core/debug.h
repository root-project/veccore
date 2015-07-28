#ifndef VECCORE_DEBUG_H
#define VECCORE_DEBUG_H

#ifdef NDEBUG
#   define VECCORE_DEBUG(...)  /* do nothing */
#   define VECCORE_ASSERT(...) /* do nothing */
#else
#   define VECCORE_DEBUG_ERROR   0
#   define VECCORE_DEBUG_WARN    1
#   define VECCORE_DEBUG_NOTICE  2
#   define VECCORE_DEBUG_INFO    3
#   define VECCORE_DEBUG_DEBUG   4

#   ifndef VECCORE_DEBUG_LEVEL
#       define VECCORE_DEBUG_LEVEL VECCORE_DEBUG_ERROR
#   endif

#   include <cstdio>
#   include <cstdarg>

#   define VECCORE_ASSERT(expr, fmt, ...)                                      \
        do {                                                                   \
            if (!(expr)) {                                                     \
                fprintf(stderr, "Assertion failed: '%s', ", #expr);            \
                fprintf(stderr, "file: '%s', line: %d\n", __FILE__, __LINE__); \
                fprintf(stderr, fmt, __VA_ARGS__);                             \
            }                                                                  \
        } while (0)

#   define DPRINTF(level, fmt, ...)                                            \
        do {                                                                   \
            if (VECCORE_DEBUG_LEVEL >= level) {                                \
                fprintf(stderr, "%s:%d ", __FILE__, __LINE__);                 \
                fprintf(stderr, fmt, __VA_ARGS__ );                            \
            }                                                                  \
        } while (0)

#   define VECCORE_ERROR(fmt, ...)  DPRINTF(VECCORE_DEBUG_ERROR,  fmt, __VA_ARGS__)
#   define VECCORE_WARN(fmt, ...)   DPRINTF(VECCORE_DEBUG_WARN,   fmt, __VA_ARGS__)
#   define VECCORE_NOTICE(fmt, ...) DPRINTF(VECCORE_DEBUG_NOTICE, fmt, __VA_ARGS__)
#   define VECCORE_INFO(fmt, ...)   DPRINTF(VECCORE_DEBUG_INFO,   fmt, __VA_ARGS__)
#   define VECCORE_DEBUG(fmt, ...)  DPRINTF(VECCORE_DEBUG_DEBUG,  fmt, __VA_ARGS__)

#undef DPRINTF

#endif /* NDEBUG */

#endif
