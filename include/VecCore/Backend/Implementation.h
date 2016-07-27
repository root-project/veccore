#ifndef VECCORE_BACKEND_IMPLEMENTATION_H
#define VECCORE_BACKEND_IMPLEMENTATION_H

#include "Interface.h"

namespace vecCore {

template <typename T>
VECCORE_BACKEND_FUNCTION
constexpr Size_s VectorSize()
{
  return sizeof(T) / sizeof(Scalar<T>);
}

// Iterators

template <typename T>
struct IteratorImplementation {
  VECCORE_BACKEND_FUNCTION
  static Scalar<T>* Begin(T &v)
  {
    Scalar<T> *addr = (Scalar<T>*)(&v);
    return addr;
  }

  VECCORE_BACKEND_FUNCTION
  static Scalar<T>* End(T &v)
  {
    Scalar<T> *addr = (Scalar<T>*)(&v);
    return addr + sizeof(v);
  }

  VECCORE_BACKEND_FUNCTION
  static Scalar<T> const* Begin(const T &v)
  {
    Scalar<T> const *addr = (Scalar<T>*)(&v);
    return addr;
  }

  VECCORE_BACKEND_FUNCTION
  static Scalar<T> const* End(const T &v)
  {
    Scalar<T> const *addr = (Scalar<T>*)(&v);
    return addr + sizeof(v);
  }
};

template <typename T>
VECCORE_BACKEND_FUNCTION
Scalar<T>* Begin(T &v)
{
  return IteratorImplementation<T>::Begin(v);
}

template <typename T>
VECCORE_BACKEND_FUNCTION
Scalar<T>* End(T &v)
{
  return IteratorImplementation<T>::End(v);
}

template <typename T>
VECCORE_BACKEND_FUNCTION
Scalar<T> const* Begin(T const &v)
{
  return IteratorImplementation<T>::Begin(v);
}

template <typename T>
VECCORE_BACKEND_FUNCTION
Scalar<T> const* End(T const &v)
{
  return IteratorImplementation<T>::End(v);
}

// Get/Set

template <typename T>
struct IndexingImplementation {
  VECCORE_BACKEND_FUNCTION
  static Scalar<T> Get(const T &v, size_t i)
  {
    return *(Begin(v) + i);
  }

  VECCORE_BACKEND_FUNCTION
  static void Set(T &v, size_t i, Scalar<T> const val)
  {
    *(Begin(v) + i) = val;
  }
};

template <typename T>
VECCORE_BACKEND_FUNCTION
Scalar<T> Get(const T &v, size_t i)
{
  return IndexingImplementation<T>::Get(v, i);
}

template <typename T>
VECCORE_BACKEND_FUNCTION
void Set(T &v, size_t i, Scalar<T> const val)
{
  IndexingImplementation<T>::Set(v, i, val);
}

// Load/Store

template <typename T>
struct LoadStoreImplementation {
  template <typename S = Scalar<T>>
  VECCORE_BACKEND_FUNCTION
  static void Load(T& v, S const *ptr)
  {
    for (size_t i = 0; i < VectorSize<T>(); ++i)
      Set(v, i, ptr[i]);
  }

  template <typename S = Scalar<T>>
  VECCORE_BACKEND_FUNCTION
  static void Store(T const &v, S *ptr)
  {
    for (size_t i = 0; i < VectorSize<T>(); ++i)
      ptr[i] = static_cast<S>(Get(v, i));
  }
};

template <typename T>
VECCORE_BACKEND_FUNCTION
void Load(T& v, Scalar<T> const *ptr)
{
  LoadStoreImplementation<T>::template Load(v, ptr);
}

template <typename T>
VECCORE_BACKEND_FUNCTION
void Store(T const &v, Scalar<T> *ptr)
{
  LoadStoreImplementation<T>::template Store(v, ptr);
}

// Gather/Scatter

template <typename T>
struct GatherScatterImplementation {
  template <typename S = Scalar<T>>
  VECCORE_BACKEND_FUNCTION
  static void Gather(T& v, S const *ptr, Index<T> const &idx)
  {
    for (size_t i = 0; i < VectorSize<T>(); ++i)
      Set(v, i, ptr[Get(idx, i)]);
  }

  template <typename S = Scalar<T>>
  VECCORE_BACKEND_FUNCTION
  static void Scatter(T const &v, S *ptr, Index<T> const &idx)
  {
    for (size_t i = 0; i < VectorSize<T>(); ++i)
      ptr[Get(idx, i)] = Get(v, i);
  }
};

template <typename T, typename S>
VECCORE_BACKEND_FUNCTION
T Gather(S const *ptr, Index<T> const &idx)
{
  T v;
  GatherScatterImplementation<T>::template Gather<S>(v, ptr, idx);
  return v;
}

template <typename T, typename S>
VECCORE_BACKEND_FUNCTION
void Scatter(T const &v, S *ptr, Index<T> const &idx)
{
  GatherScatterImplementation<T>::template Scatter<S>(v, ptr, idx);
}

// Masking

// functions commented below must be provided by each backend

// template <typename M> bool MaskFull(const M &mask);
// template <typename M> bool MaskEmpty(const M &mask);

template <typename T>
struct MaskingImplementation {
  static inline void Assign(T& dst, Mask<T> const& mask, T const &src)
  {
    for (size_t i = 0; i < VectorSize<T>(); i++)
      if (Get(mask, i)) Set(dst, i, Get(src, i));
  }

  static inline void Blend(T& dst, Mask<T> const& mask, T const &src1, T const &src2)
  {
    for (size_t i = 0; i < VectorSize<T>(); i++)
      Set(dst, i, Get(mask, i) ? Get(src1, i) : Get(src2, i));
  }
};

template <typename T>
void MaskedAssign(T &dst, const Mask<T> &mask, const T &src)
{
  MaskingImplementation<T>::Assign(dst, mask, src);
}

template <typename T>
T Blend(const Mask<T> &mask, const T &src1, const T &src2)
{
  T v;
  MaskingImplementation<T>::Blend(v, mask, src1, src2);
  return v;
}

// Miscellaneous

VECCORE_BACKEND_FUNCTION
constexpr bool EarlyReturnAllowed()
{
#ifdef VECCORE_NVCC_DEVICE
  return false;
#else
  return true;
#endif
}

} // namespace vecCore

#endif