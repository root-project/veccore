#ifndef VECCORE_CUDA_MACROS_H
#define VECCORE_CUDA_MACROS_H

#if (defined(__CUDACC__) || defined(__NVCC__))

#   define VECCORE_CUDA
#   define VECCORE_CUDA_NAMESPACE cuda

#   ifdef __CUDA_ARCH__
#       define VECCORE_CUDA_DEVICE
#   endif

#   define VECCORE_CUDA_HOST        __host__
#   define VECCORE_CUDA_DEVICE      __device__
#   define VECCORE_CUDA_HOST_DEVICE __host__ __device__
#   define VECCORE_CUDA_GLOBAL      __global__
#   define VECCORE_CUDA_ALIGN       __align__((64))
#   define VECCORE_DECLARE_CUDA(x) /* do nothing */

#else

#   ifdef VECCORE_CUDA
#       define VECCORE_CUDA_INTERFACE /* CUDA enabled, but compiling regular C++ code */
#   endif

#   define VECCORE_CUDA_NAMESPACE
#   define VECCORE_CUDA_HOST
#   define VECCORE_CUDA_DEVICE
#   define VECCORE_CUDA_HOST_DEVICE
#   define VECCORE_CUDA_GLOBAL

#   define VECCORE_DECLARE_CUDA(T) T; namespace CUDA { T; }
#   define VECCORE_DECLARE_CUDA_TYPE(T) template <> struct cuda_t<T> { using value_type = cuda::T; };

#   define VECCORE_DECLARE_CUDA_CLASS(x)  VECCORE_DECLARE_CUDA(class x)  VECCORE_DECLARE_CUDA_TYPE(x)
#   define VECCORE_DECLARE_CUDA_STRUCT(x) VECCORE_DECLARE_CUDA(struct x) VECCORE_DECLARE_CUDA_TYPE(x)

#   define VECCORE_DECLARE_CUDA_TEMPLATE(x, arg_t) VECCORE_DECLARE_CUDA(template <arg_t arg> class x) \
        template <arg_t arg> struct cuda_t< x<arg> > { using value_type = cuda::x< cuda_t<arg> >; }

    namespace VecCore {
        template <typename T> struct cuda_t;
        template <typename T> using cuda_type = typename cuda_t<T>::value_type;

        template <> struct cuda_t<int>    { using value_type = int;    };
        template <> struct cuda_t<float>  { using value_type = float;  };
        template <> struct cuda_t<double> { using value_type = double; };
    }

#endif /* (defined(__CUDACC__) || defined(__NVCC__)) */

#endif
