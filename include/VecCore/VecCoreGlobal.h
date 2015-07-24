/// \file VecCoreGlobal.h
/// started 8.5.2015 Sandro Wenzel (sandro.wenzel@cern.ch) based on VecGeom Global

#ifndef VECCORE_BASE_GLOBAL_H_
#define VECCORE_BASE_GLOBAL_H_

// TODO:
// Generalize MACRO NAMES FROM CUDA_HEADER ... TO COPROCESSOR_HEADER OR SOMETHING

#include <cassert>
#include <cmath>
#include <cfloat>
#include <limits>
#include <memory>
#include <cstdio>

#define VECCORE

#if __cplusplus >= 201103L
  #define VECCORE_STD_CXX11
#endif

#if (defined(__CUDACC__) || defined(__NVCC__))
  // Compiling with nvcc
  #define VECCORE_NVCC
  #ifdef __CUDA_ARCH__
    // Compiling device code
    #define VECCORE_NVCC_DEVICE
  #endif
  #define HAVECUDANAMESPACE
  #define VECCORE_IMPL_NAMESPACE cuda
  #define VECCORE_NAMESPACE ::VecCore
  #define VECCORE_CUDA_HEADER_HOST __host__
  #define VECCORE_CUDA_HEADER_DEVICE __device__
  #define VECCORE_CUDA_HEADER_BOTH __host__ __device__
  #define VECCORE_CUDA_HEADER_GLOBAL __global__
  #define VECCORE_ALIGNED __align__((64))
  #define VECCORE_HOST_FORWARD_DECLARE(X) namespace cxx { X }
  #define VECCORE_DEVICE_FORWARD_DECLARE(X)
  #define VECCORE_DEVICE_DECLARE_CONV(X)
  #define VECCORE_DEVICE_DECLARE_NS_CONV(NS,X,Def)
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE(X,ArgType)
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_2v(X,ArgType1,Def1,ArgType2,Def2)
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_2v_1t(X,ArgType1,Def1,ArgType2,Def2,ArgType3)
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_3v(X,ArgType1,Def1,ArgType2,Def2,ArgType3,Def3)
  #undef VECCORE_VC
  #undef VECCORE_VC_ACCELERATION
  #undef VECCORE_CILK
  #undef VECCORE_ROOT
  #undef VECCORE_USOLIDS
  #undef VECCORE_GEANT4
  #undef VECCORE_BENCHMARK
#else
  // Not compiling with NVCC
#define HAVENORMALNAMESPACE
  #define VECCORE_IMPL_NAMESPACE cxx
  #define VECCORE_NAMESPACE ::VecCore
  #define VECCORE_CUDA_HEADER_HOST
  #define VECCORE_CUDA_HEADER_DEVICE
  #define VECCORE_CUDA_HEADER_BOTH
  #define VECCORE_CUDA_HEADER_GLOBAL
  #ifdef VECCORE_CUDA
    // CUDA is enabled, but currently compiling regular C++ code.
    // This enables methods that interface between C++ and CUDA environments
    #define VECCORE_CUDA_INTERFACE
  #endif
  namespace veccore{
     template <typename DataType> struct kCudaType;
     template <typename DataType> using CudaType_t = typename kCudaType<DataType>::type_t;
     template <> struct kCudaType<float> { using type_t = float; };
     template <> struct kCudaType<double> { using type_t = double; };
     template <> struct kCudaType<int> { using type_t = int; };
  }
  #define VECCORE_HOST_FORWARD_DECLARE(X)
  #define VECCORE_DEVICE_FORWARD_DECLARE(X)  namespace cuda { X }

  #define VECCORE_DEVICE_DECLARE_CONV(X) \
     namespace cuda { class X; } \
     inline namespace cxx  { class X; } \
     template <> struct kCudaType<cxx::X> { using type_t = cuda::X; };

  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE(X,ArgType) \
     namespace cuda { template <ArgType Arg> class X; } \
     inline namespace cxx  { template <ArgType Arg> class X; } \
     template <ArgType Arg> struct kCudaType<cxx::X<Arg> > \
     { using type_t = cuda::X<CudaType_t<Arg> >; };

#ifdef VECCORE_CUDA_VOLUME_SPECIALIZATION

  #define VECCORE_DEVICE_DECLARE_NS_CONV(NS,X,Def)     \
     namespace cuda { namespace NS { class X; } } \
     inline namespace cxx { namespace NS { class X; } } \
     template <> struct kCudaType<cxx::NS::X> { using type_t = cuda::NS::X; };

  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_2v(X,ArgType1,Def1,ArgType2,Def2) \
     namespace cuda { template <ArgType1 Arg1,ArgType2 Arg2> class X; } \
     inline namespace cxx  { template <ArgType1 Arg1,ArgType2 Arg2> class X; } \
     template <ArgType1 Arg1,ArgType2 Arg2> struct kCudaType<cxx::X<Arg1,Arg2> > \
     { using type_t = cuda::X<Arg1,Arg2 >; };
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_2v_1t(X,ArgType1,Def1,ArgType2,Def2,ArgType3) \
     namespace cuda { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     inline namespace cxx  { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> struct kCudaType<cxx::X<Arg1,Arg2,Arg3> > \
     { using type_t = cuda::X<Arg1, Arg2, CudaType_t<Arg3> >; };
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_3v(X,ArgType1,Def1,ArgType2,Def2,ArgType3,Def3) \
     namespace cuda { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     inline namespace cxx  { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> struct kCudaType<cxx::X<Arg1,Arg2,Arg3> > \
     { using type_t = cuda::X<Arg1,Arg2,Arg3 >; };

#else // VECCORE_CUDA_VOLUME_SPECIALIZATION

  #define VECCORE_DEVICE_DECLARE_NS_CONV(NS,X,Def)     \
     namespace cuda { namespace NS { class Def; } } \
     inline namespace cxx { namespace NS { class X; } } \
     template <> struct kCudaType<cxx::NS::X> { using type_t = cuda::NS::Def; };

  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_2v(X,ArgType1,Def1,ArgType2,Def2) \
     namespace cuda { template <ArgType1 Arg1,ArgType2 Arg2> class X; } \
     inline namespace cxx  { template <ArgType1 Arg1,ArgType2 Arg2> class X; } \
     template <ArgType1 Arg1,ArgType2 Arg2> struct kCudaType<cxx::X<Arg1,Arg2> > \
     { using type_t = cuda::X<Def1, Def2 >; };
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_2v_1t(X,ArgType1,Def1,ArgType2,Def2,ArgType3) \
     namespace cuda { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     inline namespace cxx  { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> struct kCudaType<cxx::X<Arg1,Arg2,Arg3> > \
     { using type_t = cuda::X<Def2, Def2, CudaType_t<Arg3> >; };
  #define VECCORE_DEVICE_DECLARE_CONV_TEMPLATE_3v(X,ArgType1,Def1,ArgType2,Def2,ArgType3,Def3) \
     namespace cuda { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     inline namespace cxx  { template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> class X; } \
     template <ArgType1 Arg1,ArgType2 Arg2,ArgType3 Arg3> struct kCudaType<cxx::X<Arg1,Arg2,Arg3> > \
     { using type_t = cuda::X<Def1,Def2,Def3 >; };

#endif // VECCORE_CUDA_VOLUME_SPECIALIZATION

#endif

#ifdef __INTEL_COMPILER
  // Compiling with icc
  #define VECCORE_INTEL
  #define VECCORE_INLINE inline
#else
  // Functionality of <mm_malloc.h> is automatically included in icc
  #include <mm_malloc.h>
  #if !defined(__NO_INLINE__) && !defined( VECCORE_NOINLINE )
    #define VECCORE_INLINE inline __attribute__((always_inline))
    #ifndef VECCORE_NVCC
      #define VECCORE_ALIGNED __attribute__((aligned(64)))
    #endif
  #else
//#pragma message "forced inlining disabled"
  // Clang or forced inlining is disabled ( by falling back to compiler decision )
    #define VECCORE_INLINE inline
    #ifndef VECCORE_NVCC
      #define VECCORE_ALIGNED
    #endif
  #endif
#endif

#ifndef NULL
  #define NULL nullptr
#endif

// Allow constexpr variables and functions if possible
#ifdef VECCORE_STD_CXX11
  #define VECCORE_CONSTEXPR constexpr
  #define VECCORE_CONSTEXPR_RETURN constexpr
#else
  #define VECCORE_CONSTEXPR const
  #define VECCORE_CONSTEXPR_RETURN
#endif

// Qualifier(s) of global constants
#ifdef VECCORE_NVCC_DEVICE
    // constexpr not supported on device in CUDA 6.5
    #define VECCORE_GLOBAL static __constant__ const
    #define VECCORE_CLASS_GLOBAL static const
#else
  #define VECCORE_GLOBAL static constexpr
  #define VECCORE_CLASS_GLOBAL static constexpr
#endif

namespace veccore{
inline namespace VECCORE_IMPL_NAMESPACE{
#ifdef VECCORE_FLOAT_PRECISION
typedef float DefaultPrecision_t;
#else
typedef double DefaultPrecision_t;
#endif

#ifndef VECCORE_NVCC
   using std::unique_ptr;

#else

   template <typename T>
   class unique_ptr {
      T *fValue;
   public:
     VECCORE_CUDA_HEADER_BOTH
     unique_ptr(T *in) : fValue(in) {}

     VECCORE_CUDA_HEADER_BOTH
     ~unique_ptr() { delete fValue; }

     VECCORE_CUDA_HEADER_BOTH
     T* operator->() { return fValue; }
   };

   template <typename T>
   class unique_ptr<T[]> {
      T *fValue;
   public:
     VECCORE_CUDA_HEADER_BOTH
     unique_ptr(T *in) : fValue(in) {}

     VECCORE_CUDA_HEADER_BOTH
     ~unique_ptr() { delete [] fValue; }

     VECCORE_CUDA_HEADER_BOTH
     T &operator[](size_t idx) { return fValue[idx]; }
   };
#endif

#ifdef __MIC__
VECCORE_GLOBAL int kAlignmentBoundary = 64;
#else
VECCORE_GLOBAL int kAlignmentBoundary = 32;
#endif

// TODO: I think we want to have
// veccore::Numeric_Constants< type > in the future

VECCORE_GLOBAL DefaultPrecision_t kPi = 3.14159265358979323846;
VECCORE_GLOBAL DefaultPrecision_t kTwoPi = 2.*kPi;
VECCORE_GLOBAL DefaultPrecision_t kTwoPiInv = 1./kTwoPi;
VECCORE_GLOBAL DefaultPrecision_t kDegToRad = kPi/180.;
VECCORE_GLOBAL DefaultPrecision_t kRadToDeg = 180./kPi;
//
VECCORE_GLOBAL DefaultPrecision_t kTolerance = 1E-8;

VECCORE_GLOBAL DefaultPrecision_t kInfinity =
#ifndef VECCORE_NVCC
    std::numeric_limits<DefaultPrecision_t>::infinity();
#else
    INFINITY;
#endif
VECCORE_GLOBAL DefaultPrecision_t kEpsilon =
#ifndef VECCORE_NVCC
    std::numeric_limits<DefaultPrecision_t>::epsilon();
#elif VECCORE_FLOAT_PRECISION
    FLT_EPSILON;
#else
    DBL_EPSILON;
#endif
VECCORE_GLOBAL DefaultPrecision_t kMinimum =
#ifndef VECCORE_NVCC
    std::numeric_limits<DefaultPrecision_t>::min();
#elif VECCORE_FLOAT_PRECISION
    FLT_MIN;
#else
    DBL_MIN;
#endif
VECCORE_GLOBAL DefaultPrecision_t kMaximum =
#ifndef VECCORE_NVCC
    std::numeric_limits<DefaultPrecision_t>::max();
#elif VECCORE_FLOAT_PRECISION
    FLT_MAX;
#else
    DBL_MAX;
#endif

// this could be a MACRO AND NOT A FUNCTION
VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
void Assert(const bool condition, char const *const message) {
#ifndef VECCORE_NVCC
  assert(condition && message);
#else
  if (!condition) printf("Assertion failed: %s", message);
#endif
}

VECCORE_CUDA_HEADER_BOTH
VECCORE_INLINE
void Assert(const bool condition) {
  Assert(condition, "");
}

namespace details {
   template <typename DataType, typename Target> struct UseIfSameType {
      VECCORE_CUDA_HEADER_BOTH
      static Target const *Get(DataType*) { return nullptr; }
   };
   template <typename DataType> struct UseIfSameType<DataType,DataType> {
      VECCORE_CUDA_HEADER_BOTH
      static DataType const *Get(DataType *ptr) { return ptr; }
   };
}

} } // End global namespace

#endif // VECCORE_BASE_GLOBAL_H_
