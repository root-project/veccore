#ifndef VECCORE_LIMITS_H
#define VECCORE_LIMITS_H

#include <limits>

namespace vecCore {
namespace backend {

template <typename T>
struct ScalarType { typedef T Type; };

#ifndef __CUDACC__
template <>
template <typename T, template <typename> class VectorType>
struct ScalarType<VectorType<T>> { typedef T Type; };
#endif

}

template <typename T>
class NumericLimits {
public:
  using ScalarT = typename backend::ScalarType<T>::Type;

  static constexpr T Min() noexcept { return T(std::numeric_limits<ScalarT>::min()); }
  static constexpr T Max() noexcept { return T(std::numeric_limits<ScalarT>::max()); }

  static constexpr T Lowest() noexcept { return T(std::numeric_limits<ScalarT>::lowest()); }
  static constexpr T Highest() noexcept { return T(std::numeric_limits<ScalarT>::max()); }

  static constexpr T Epsilon() noexcept { return T(std::numeric_limits<ScalarT>::epsilon()); }
  static constexpr T Infinity() noexcept { return T(std::numeric_limits<ScalarT>::infinity()); }
};

} // namespace vecCore

#endif
