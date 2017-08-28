#ifndef VECCORE_BACKEND_VC_H
#define VECCORE_BACKEND_VC_H

#ifdef VECCORE_ENABLE_VC

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"

#include <Vc/Vc>

#pragma GCC diagnostic pop

#include "VcScalar.h"

#ifndef Vc_IMPL_Scalar
#include "VcVector.h"
#else
template <typename T>
using VcVectorT = VcScalarT<T>;
using VcVector  = VcScalar;
#endif

#include "VcSimdArray.h"

#endif // VECCORE_ENABLE_VC
#endif // VECCORE_BACKEND_VC_H
