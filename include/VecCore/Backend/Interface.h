#ifndef VECCORE_BACKEND_INTERFACE_H
#define VECCORE_BACKEND_INTERFACE_H

#define VECCORE_BACKEND_FUNCTION \
  VECCORE_FORCE_INLINE VECCORE_ATT_HOST_DEVICE

namespace vecCore {

template <typename T> struct TypeTraits;

template <typename T> using Mask   = typename TypeTraits<T>::MaskType;
template <typename T> using Index  = typename TypeTraits<T>::IndexType;
template <typename T> using Scalar = typename TypeTraits<T>::ScalarType;

// Iterators

template <typename T> VECCORE_BACKEND_FUNCTION Scalar<T>* Begin(T &v);
template <typename T> VECCORE_BACKEND_FUNCTION Scalar<T>* End(T &v);
template <typename T> VECCORE_BACKEND_FUNCTION Scalar<T> const* Begin(T const &v);
template <typename T> VECCORE_BACKEND_FUNCTION Scalar<T> const* End(T const &v);

// Vector Size

template <typename T> VECCORE_BACKEND_FUNCTION constexpr Size_s VectorSize();

// Get/Set

template <typename T> VECCORE_BACKEND_FUNCTION Scalar<T> Get(const T &v, size_t i);
template <typename T> VECCORE_BACKEND_FUNCTION void Set(T &v, size_t i, Scalar<T> const val);

// Load/Store

template <typename T> VECCORE_BACKEND_FUNCTION void Load(T& v, Scalar<T> const *ptr);
template <typename T> VECCORE_BACKEND_FUNCTION void Store(T const &v, Scalar<T> *ptr);

// Gather/Scatter

template <typename T, typename S = Scalar<T>> VECCORE_BACKEND_FUNCTION
T Gather(S const *ptr, Index<T> const &idx);

template <typename T, typename S = Scalar<T>> VECCORE_BACKEND_FUNCTION
void Scatter(T const &v, S *ptr, Index<T> const &idx);

// Masking/Blending

template <typename M> bool MaskFull(M const &mask);
template <typename M> bool MaskEmpty(M const &mask);

template <typename T> void MaskedAssign(T &dst, const Mask<T> &mask, const T &src);
template <typename T> T Blend(const Mask<T> &mask, const T &src1, const T &src2);

VECCORE_BACKEND_FUNCTION constexpr Bool_s EarlyReturnAllowed();

} // namespace vecCore

#endif
