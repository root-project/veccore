#ifndef VECCORE_BACKEND_VC_H
#define VECCORE_BACKEND_VC_H

#ifdef VECCORE_ENABLE_VC

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wshadow"

#include <Vc/Vc>

#pragma GCC diagnostic pop

#include "VcScalar.h"
#include "VcVector.h"
#include "VcSimdArray.h"

#endif // VECCORE_ENABLE_VC
#endif // VECCORE_BACKEND_VC_H
