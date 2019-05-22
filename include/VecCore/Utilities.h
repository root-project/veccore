#ifndef VECCORE_UTILITIES_H
#define VECCORE_UTILITIES_H

#ifndef _MSC_VER

#include <cstdlib>

namespace vecCore {

inline void *AlignedAlloc(size_t alignment, size_t size)
{
  void *ptr = nullptr;

  if (posix_memalign(&ptr, alignment, size) == 0)
    return ptr;

  return nullptr;
}

inline void AlignedFree(void *ptr)
{
  free(ptr);
}

}

#else // _MSC_VER

#include <malloc.h>

namespace vecCore {

inline void *AlignedAlloc(size_t alignment, size_t size)
{
  return _aligned_malloc(size, alignment);
}

inline void AlignedFree(void *ptr)
{
  _aligned_free(ptr);
}

}

#endif // _MSC_VER

#endif // VECCORE_UTILITIES_H
