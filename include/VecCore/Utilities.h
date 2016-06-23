#ifndef VECCORE_UTILITIES_H
#define VECCORE_UTILITIES_H

#include <cstdlib>
#include <system_error>

namespace vecCore {

inline void *AlignedAlloc(size_t alignment, size_t size)
{
  int status;
  void *ptr = NULL;

  if ((status = posix_memalign(&ptr, alignment, size)) != 0) throw std::system_error(status, std::system_category());

  return ptr;
}
}

#endif
