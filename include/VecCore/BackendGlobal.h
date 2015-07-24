/// \file VecCoreGlobal.h
/// started 8.5.2015 Sandro Wenzel (sandro.wenzel@cern.ch) based on VecGeom Global

#ifndef VECCORE_BACKEND_GLOBAL_H_
#define VECCORE_BACKEND_GLOBAL_H_

#include "VecCoreGlobal.h"

#include "backend/scalar/Backend.h"
#include "backend/Vc/Backend.h"

namespace veccore{
inline namespace VECCORE_IMPL_NAMESPACE{

// CHOOSE THE DEFAULT BACKENDS IN FUTURE HERE:
// include the appropriate backend files
#ifdef VCBACKEND
template <typename T> using DefaultVectorBackend = veccore::backend::vector::kVc<T>;
#else
template <typename T> using DefaultVectorBackend = veccore::backend::scalar::kScalar<T>;
#endif
template <typename T> using DefaultScalarBackend = veccore::backend::scalar::kScalar<T>;

// alternative shorter names
template <typename T> using DfltVBckEnd = DefaultVectorBackend<T>;
template <typename T> using DfltSBckEnd = DefaultScalarBackend<T>;


} } // End global namespace

#endif // VECCORE_BASE_GLOBAL_H_
