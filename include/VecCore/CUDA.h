#ifndef VECCORE_CUDA_H
#define VECCORE_CUDA_H

#if !defined(VECCORE_ENABLE_CUDA)

#define VECCORE_CUDA_HOST                /* empty */
#define VECCORE_CUDA_DEVICE              /* empty */
#define VECCORE_CUDA_HOST_DEVICE         /* empty */
#define VECCORE_CUDA_GLOBAL              /* empty */
#define VECCORE_CUDA_ALIGN               /* empty */
#define VECCORE_DECLARE_CUDA(x)          /* empty */
#define VECCORE_DECLARE_CUDA_TYPE(T)     /* empty */
#define VECCORE_DECLARE_CUDA_CLASS(x)    /* empty */
#define VECCORE_DECLARE_CUDA_STRUCT(x)   /* empty */

#elif (defined(__CUDACC__) || defined(__NVCC__))

#define VECCORE_NVCC
#ifdef __CUDA_ARCH__
#define VECCORE_NVCC_DEVICE
#endif

#define VECCORE_CUDA_HOST __host__
#define VECCORE_CUDA_DEVICE __device__
#define VECCORE_CUDA_HOST_DEVICE __host__ __device__
#define VECCORE_CUDA_GLOBAL __global__
#define VECCORE_CUDA_ALIGN __align__((64))
#define VECCORE_DECLARE_CUDA(x)          /* empty */
#define VECCORE_DECLARE_CUDA_TYPE(T)     /* empty */
#define VECCORE_DECLARE_CUDA_CLASS(x)    /* empty */
#define VECCORE_DECLARE_CUDA_STRUCT(x)   /* empty */
#define VECCORE_DECLARE_CUDA_TEMPLATE(x) /* empty */

#else // CUDA enabled, but compiling regular C++ code

#define VECCORE_CUDA_INTERFACE
#define VECCORE_CUDA_HOST        /* empty */
#define VECCORE_CUDA_DEVICE      /* empty */
#define VECCORE_CUDA_HOST_DEVICE /* empty */
#define VECCORE_CUDA_GLOBAL      /* empty */
#define VECCORE_CUDA_ALIGN       /* empty */

#define VECCORE_CUDA_BASIC_TYPES                                               \
template <typename T> struct CudaTypeTraits;                                   \
template <> struct CudaTypeTraits<Float_t>  { using Type = Float_t;  };        \
template <> struct CudaTypeTraits<Double_t> { using Type = Double_t; };        \
template <> struct CudaTypeTraits<Int8_t>   { using Type = Int8_t;   };        \
template <> struct CudaTypeTraits<Int16_t>  { using Type = Int16_t;  };        \
template <> struct CudaTypeTraits<Int32_t>  { using Type = Int32_t;  };        \
template <> struct CudaTypeTraits<Int64_t>  { using Type = Int64_t;  };        \
template <> struct CudaTypeTraits<Uint8_t>  { using Type = Uint8_t;  };        \
template <> struct CudaTypeTraits<Uint16_t> { using Type = Uint16_t; };        \
template <> struct CudaTypeTraits<Uint32_t> { using Type = Uint32_t; };        \
template <> struct CudaTypeTraits<Uint64_t> { using Type = Uint64_t; };        \
template <typename T> using CudaType = typename CudaTypeTraits<T>::Type;

#define VECCORE_DECLARE_CUDA(T) T; namespace cuda { T; }

#define VECCORE_DECLARE_CUDA_TYPE(T)                                           \
  template <> struct CudaTypeTraits<T> { using Type = cuda::T; };              \

#define VECCORE_DECLARE_CUDA_CLASS(x)                                          \
  VECCORE_DECLARE_CUDA(class x) VECCORE_DECLARE_CUDA_TYPE(x)

#define VECCORE_DECLARE_CUDA_STRUCT(x)                                         \
  VECCORE_DECLARE_CUDA(struct x) VECCORE_DECLARE_CUDA_TYPE(x)

#endif // defined (VECCORE_ENABLE_CUDA)

#endif
