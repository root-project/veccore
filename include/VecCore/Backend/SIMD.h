#ifndef VECCORE_BACKEND_STD_SIMD_H
#define VECCORE_BACKEND_STD_SIMD_H

#if __cplusplus >= 202002L && defined(__has_include)
#if __has_include(<experimental/simd>) && !defined(_LIBCPP_VERSION)
#define VECCORE_ENABLE_STD_SIMD
#endif
#endif

#ifdef VECCORE_ENABLE_STD_SIMD

#include <experimental/simd>

namespace vecCore {

template <typename T, class Abi>
struct TypeTraits<std::experimental::simd_mask<T, Abi>> {
  using IndexType = typename std::experimental::simd_mask<T, Abi>::simd_type;
  using ScalarType = typename std::experimental::simd_mask<T, Abi>::value_type;
  static constexpr size_t Size = std::experimental::simd<T, Abi>::size();
};

template <typename T, class Abi>
struct TypeTraits<std::experimental::simd<T, Abi>> {
  using MaskType = typename std::experimental::simd<T, Abi>::mask_type;
  using IndexType = typename std::experimental::simd<T, Abi>;
  using ScalarType = typename std::experimental::simd<T, Abi>::value_type;
  static constexpr size_t Size = std::experimental::simd<T, Abi>::size();
};

namespace backend {

template <class Abi> class SIMD {
public:
  using Real_v = std::experimental::simd<Real_s, Abi>;
  using Float_v = std::experimental::simd<float, Abi>;
  using Double_v = std::experimental::simd<double, Abi>;

  using Int_v = std::experimental::simd<int, Abi>;
  using Int16_v = std::experimental::simd<int16_t, Abi>;
  using Int32_v = std::experimental::simd<int32_t, Abi>;
  using Int64_v = std::experimental::simd<int64_t, Abi>;

  using UInt_v = std::experimental::simd<unsigned int, Abi>;
  using UInt16_v = std::experimental::simd<uint16_t, Abi>;
  using UInt32_v = std::experimental::simd<uint32_t, Abi>;
  using UInt64_v = std::experimental::simd<uint64_t, Abi>;
};

class SIMDNative {
public:
  using Real_v = std::experimental::native_simd<Real_s>;
  using Float_v = std::experimental::native_simd<float>;
  using Double_v = std::experimental::native_simd<double>;

  using Int_v = std::experimental::native_simd<int>;
  using Int16_v = std::experimental::native_simd<int16_t>;
  using Int32_v = std::experimental::native_simd<int32_t>;
  using Int64_v = std::experimental::native_simd<int64_t>;

  using UInt_v = std::experimental::native_simd<unsigned int>;
  using UInt16_v = std::experimental::native_simd<uint16_t>;
  using UInt32_v = std::experimental::native_simd<uint32_t>;
  using UInt64_v = std::experimental::native_simd<uint64_t>;
};

using SIMDScalar = SIMD<std::experimental::simd_abi::scalar>;

template <size_t N>
using SIMDVector = SIMD<std::experimental::simd_abi::fixed_size<N>>;

} // namespace backend

template <typename T, class Abi>
bool MaskEmpty(std::experimental::simd_mask<T, Abi> mask) {
  for (size_t i = 0; i < mask.size(); ++i)
    if (mask[i])
      return false;
  return true;
}

template <typename T, class Abi>
bool MaskFull(std::experimental::simd_mask<T, Abi> mask) {
  for (size_t i = 0; i < mask.size(); ++i)
    if (!mask[i])
      return false;
  return true;
}

template <typename T, class Abi>
struct IndexingImplementation<std::experimental::simd<T, Abi>> {
  using V = std::experimental::simd<T, Abi>;

  static inline T Get(const V &v, size_t i) { return v[i]; }
  static inline void Set(V &v, size_t i, T const val) { v[i] = val; }
};

template <typename T, class Abi>
struct IndexingImplementation<std::experimental::simd_mask<T, Abi>> {
  using V = std::experimental::simd_mask<T, Abi>;

  static inline T Get(const V &v, size_t i) { return v[i]; }
  static inline void Set(V &v, size_t i, T const val) { v[i] = !!val; }
};

template <typename T, class Abi>
struct LoadStoreImplementation<std::experimental::simd<T, Abi>> {
  using V = std::experimental::simd<T, Abi>;

  template <typename S = T> static inline void Load(V &v, S const *ptr) {
    for (size_t i = 0; i < V::size(); ++i)
      v[i] = ptr[i];
  }

  template <typename S = T> static inline void Store(V const &v, S *ptr) {
    for (size_t i = 0; i < V::size(); ++i)
      ptr[i] = static_cast<S>(v[i]);
  }
};

template <typename T, class Abi>
struct LoadStoreImplementation<std::experimental::simd_mask<T, Abi>> {
  using V = typename std::experimental::simd_mask<T, Abi>;

  template <typename S = T> static inline void Load(V &v, S const *ptr) {
    for (size_t i = 0; i < V::size(); ++i)
      v[i] = !!ptr[i];
  }

  template <typename S = T> static inline void Store(V const &v, S *ptr) {
    for (size_t i = 0; i < V::size(); ++i)
      ptr[i] = static_cast<S>(v[i]);
  }
};

template <typename T, class Abi>
struct MaskingImplementation<std::experimental::simd<T, Abi>> {
  using V = typename std::experimental::simd<T, Abi>;
  using M = typename std::experimental::simd<T, Abi>::mask_type;

  static inline void Assign(V &dst, M const &mask, V const &src) {
    where(mask, dst) = src;
  }

  static inline void Blend(V &dst, M const &mask, V const &src1,
                           V const &src2) {
    where(mask, dst) = src1;
    where(!mask, dst) = src2;
  }
};

template <class Abi>
struct GatherScatterImplementation<std::experimental::simd<float, Abi>> {
  using V = typename std::experimental::simd<float, Abi>;

  template <typename S = float>
  static inline void Gather(V &v, S const *ptr, V const &idx) {
    auto ii = std::experimental::static_simd_cast<int, float, Abi>(idx);
    for (size_t i = 0; i < V::size(); ++i)
      v[i] = ptr[ii[i]];
  }

  template <typename S = float>
  static inline void Scatter(V const &v, S *ptr, V const &idx) {
    auto ii = std::experimental::static_simd_cast<int, float, Abi>(idx);
    for (size_t i = 0; i < V::size(); ++i)
      ptr[ii[i]] = v[i];
  }
};

template <class Abi>
struct GatherScatterImplementation<std::experimental::simd<double, Abi>> {
  using V = typename std::experimental::simd<double, Abi>;

  template <typename S = double>
  static inline void Gather(V &v, S const *ptr, V const &idx) {
    auto ii = std::experimental::static_simd_cast<int64_t, double, Abi>(idx);
    for (size_t i = 0; i < V::size(); ++i)
      v[i] = ptr[ii[i]];
  }

  template <typename S = double>
  static inline void Scatter(V const &v, S *ptr, V const &idx) {
    auto ii = std::experimental::static_simd_cast<int64_t, double, Abi>(idx);
    for (size_t i = 0; i < V::size(); ++i)
      ptr[ii[i]] = v[i];
  }
};

} // namespace vecCore

#endif
#endif
