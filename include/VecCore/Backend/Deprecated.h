#ifndef VECCORE_BACKEND_DEPRECATED_H
#define VECCORE_BACKEND_DEPRECATED_H

#include "Interface.h"

// This file contains old interface functions implemented in terms
// of the new ones. This is necessary not to break user code. Once
// we decide if the names for the new interface are good and migrate
// code to the new names, then these can be safely removed after a
// version bump of the library.

namespace vecCore {

// old interface, scalar was added first, so is different than others...

template <typename T>
using Mask_v = typename TypeTraits<T>::MaskType;
template <typename T>
using Index_v = typename TypeTraits<T>::IndexType;

template <typename T>
struct ScalarType {
  using Type = typename TypeTraits<T>::ScalarType;
};

// Get/Set

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
Scalar<T> LaneAt(const T &v, size_t i)
{
  return Get(v, i);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void AssignLane(T &v, size_t i, Scalar<T> const val)
{
  Set(v, i, val);
}

// Load/Store

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
T FromPtr(Scalar<T> const *ptr)
{
  T v;
  LoadStoreImplementation<T>::template Load(v, ptr);
  return v;
}

template <typename M>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void StoreMask(M const &mask, bool *ptr)
{
  LoadStoreImplementation<M>::template Store<bool>(mask, ptr);
}

// Masking

template <typename M>
bool MaskFull(const M &mask);
template <typename M>
bool MaskEmpty(const M &mask);

template <typename M>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
bool MaskLaneAt(const M &mask, size_t i)
{
  return Get(mask, i);
}

template <typename T>
VECCORE_FORCE_INLINE
VECCORE_ATT_HOST_DEVICE
void AssignMaskLane(T &mask, size_t i, bool val)
{
  Set(mask, i, val);
}

} // namespace vecCore

#endif
