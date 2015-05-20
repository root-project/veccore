/// \file VecCoreGlobal.h
/// started 8.5.2015 Sandro Wenzel (sandro.wenzel@cern.ch) based on VecGeom Global

#ifndef VECCORE_BACKEND_GLOBAL_H_
#define VECCORE_BACKEND_GLOBAL_H_

#include "VecCoreGlobal.h"

#include "backend/scalar/Backend.h"
#ifdef VCBACKEND
#include "backend/Vc/Backend.h"
#endif

namespace VecCore {
inline namespace VECCORE_IMPL_NAMESPACE{

// CHOOSE THE DEFAULT BACKENDS IN FUTURE HERE:
// include the appropriate backend files
#ifdef VCBACKEND
template <typename T> using DefaultVectorBackend = VecCore::Backend::Vector::kVc<T>;
#define LOADOP &
#else
template <typename T> using DefaultVectorBackend = VecCore::Backend::Scalar::kScalar<T>;
#define LOADOP
#endif
template <typename T> using DefaultScalarBackend = VecCore::Backend::Scalar::kScalar<T>;


} } // End global namespace

#endif // VECCORE_BASE_GLOBAL_H_
