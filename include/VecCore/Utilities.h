#ifndef VECCORE_UTILITIES_H
#define VECCORE_UTILITIES_H

#include <mm_malloc.h>

namespace vecCore {

inline void *AlignedAlloc(size_t alignment, size_t size)
{
  return _mm_malloc(size, alignment);
}

inline void AlignedFree(void *ptr)
{
  _mm_free(ptr);
}
}

#endif
