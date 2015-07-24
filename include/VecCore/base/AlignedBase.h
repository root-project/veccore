/// \file AlignedBase.h
/// \author Johannes de Fine Licht (johannes.definelicht@cern.ch)

#ifndef VECCORE_BASE_ALIGNEDBASE_H_
#define VECCORE_BASE_ALIGNEDBASE_H_

#include "VecCoreGlobal.h"
#ifdef VECGEOM_VC
#include <Vc/Vc>
#endif

namespace veccore{

VECCORE_DEVICE_FORWARD_DECLARE( class AlignedBase; )
VECCORE_DEVICE_DECLARE_CONV( AlignedBase )

inline namespace VECGEOM_IMPL_NAMESPACE {

#ifdef VECGEOM_VC
class AlignedBase : public Vc::VectorAlignedBase {
public:
      virtual ~AlignedBase() {}
};
#elif !defined(VECGEOM_NVCC)
class AlignedBase {

public:

  VECCORE_INLINE
  void *operator new(size_t size) {
    return _mm_malloc(size, kAlignmentBoundary);
  }

  VECCORE_INLINE
  void *operator new(size_t, void *p) {
    return p;
  }

  VECCORE_INLINE
  void *operator new[](size_t size) {
    return _mm_malloc(size, kAlignmentBoundary);
  }

  VECCORE_INLINE
  void *operator new[](size_t , void *p) {
    return p;
  }
  
  VECCORE_INLINE
  void operator delete(void *ptr, size_t) {
    _mm_free(ptr);
  }

  VECCORE_INLINE
  void operator delete(void *, void *) {}

  VECCORE_INLINE
  void operator delete[](void *ptr, size_t) {
    _mm_free(ptr);
  }

  VECCORE_INLINE
  void operator delete[](void *, void *) {}

};
#else
class AlignedBase {};
#endif

} } // End global namespace

#endif // VECCORE_BASE_ALIGNEDBASE_H_
