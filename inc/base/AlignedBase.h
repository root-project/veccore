/// \file AlignedBase.h
/// \author Johannes de Fine Licht (johannes.definelicht@cern.ch)

#ifndef VECGEOM_BASE_ALIGNEDBASE_H_
#define VECGEOM_BASE_ALIGNEDBASE_H_

#include "base/Global.h"
#ifdef VECGEOM_VC
#include <Vc/Vc>
#endif

namespace vecgeom {

VECGEOM_DEVICE_FORWARD_DECLARE( class AlignedBase; )
VECGEOM_DEVICE_DECLARE_CONV( AlignedBase )

inline namespace VECGEOM_IMPL_NAMESPACE {

#ifdef VECGEOM_VC
class AlignedBase : public Vc::VectorAlignedBase {
public:
      virtual ~AlignedBase() {}
};
#elif !defined(VECGEOM_NVCC)
class AlignedBase {

public:

  VECGEOM_INLINE
  void *operator new(size_t size) {
    return _mm_malloc(size, kAlignmentBoundary);
  }

  VECGEOM_INLINE
  void *operator new(size_t, void *p) {
    return p;
  }

  VECGEOM_INLINE
  void *operator new[](size_t size) {
    return _mm_malloc(size, kAlignmentBoundary);
  }

  VECGEOM_INLINE
  void *operator new[](size_t , void *p) {
    return p;
  }
  
  VECGEOM_INLINE
  void operator delete(void *ptr, size_t) {
    _mm_free(ptr);
  }

  VECGEOM_INLINE
  void operator delete(void *, void *) {}

  VECGEOM_INLINE
  void operator delete[](void *ptr, size_t) {
    _mm_free(ptr);
  }

  VECGEOM_INLINE
  void operator delete[](void *, void *) {}

};
#else
class AlignedBase {};
#endif

} } // End global namespace

#endif // VECGEOM_BASE_ALIGNEDBASE_H_
