#ifndef VECCORE_TYPES_H
#define VECCORE_TYPES_H

#include <cstddef>
#include <cstdint>

namespace vecCore {

using Bool_t = bool;

using Float_t = float;
using Double_t = double;

#ifdef VECCORE_SINGLE_PRECISION
using Real_t = Float_t;
#else
using Real_t = Double_t;
#endif

using Size_t = size_t;

using Int_t = int32_t;
using Int16_t = int16_t;
using Int32_t = int32_t;
using Int64_t = int64_t;

using UInt_t = uint32_t;
using UInt16_t = uint16_t;
using UInt32_t = uint32_t;
using UInt64_t = uint64_t;

template <class T>
struct ScalarType { using Type = T; };

template <class T, template <typename> class VectorType>
struct ScalarType<VectorType<T>> { using Type = T; };

template <class T, size_t N, template <typename, size_t> class VectorType>
struct ScalarType<VectorType<T,N>> { using Type = T; };

}

#endif
