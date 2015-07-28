#ifndef VECCORE_MACROS_H
#define VECCORE_MACROS_H

#ifndef VECCORE_NO_FORCED_INLINING
#    define VECCORE_FORCE_NOINLINE __attribute__((noinline))
#    define VECCORE_FORCE_INLINE   __attribute__((always_inline))
#endif

#endif
