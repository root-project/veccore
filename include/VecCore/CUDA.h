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
#define VECCORE_DECLARE_CUDA_TEMPLATE(x) /* empty */
#else
#if (defined(__CUDACC__) || defined(__NVCC__))
#ifdef __CUDA_ARCH__
#define VECCORE_CUDA_ARCH
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

#define VECCORE_DECLARE_CUDA(T) T; namespace cuda { T; }

#define VECCORE_DECLARE_CUDA_TYPE(T)                                           \
  namespace vecCore {                                                          \
  template <> struct cuda_t<T> { using value_type = cuda::T; };                \
  }

#define VECCORE_DECLARE_CUDA_CLASS(x)                                          \
  VECCORE_DECLARE_CUDA_TYPE(x) VECCORE_DECLARE_CUDA(class x)

#define VECCORE_DECLARE_CUDA_STRUCT(x)                                         \
  VECCORE_DECLARE_CUDA_TYPE(x) VECCORE_DECLARE_CUDA(struct x)

#define VECCORE_DECLARE_CUDA_TEMPLATE(x, arg_t)                                \
  VECCORE_DECLARE_CUDA(template <arg_t arg> class x)                           \
  namespace vecCore {                                                          \
  template <arg_t arg> struct cuda_t<x<arg>> {                                 \
    using value_type = cuda::x<cuda_t<arg>>;                                   \
  };                                                                           \
  }

namespace vecCore {
template <typename T> struct cuda_t;

template <> struct cuda_t<float> { using type = float; };
template <> struct cuda_t<double> { using type = double; };

template <> struct cuda_t<int8_t> { using type = int8_t; };
template <> struct cuda_t<int16_t> { using type = int16_t; };
template <> struct cuda_t<int32_t> { using type = int32_t; };
template <> struct cuda_t<int64_t> { using type = int64_t; };
template <> struct cuda_t<uint8_t> { using type = uint8_t; };
template <> struct cuda_t<uint16_t> { using type = uint16_t; };
template <> struct cuda_t<uint32_t> { using type = uint32_t; };
template <> struct cuda_t<uint64_t> { using type = uint64_t; };

template <typename T> using cuda_type = typename cuda_t<T>::type;
}
#endif // defined (__CUDACC__) || defined(__NVCC__)
#endif // defined (VECCORE_ENABLE_CUDA)
#endif
