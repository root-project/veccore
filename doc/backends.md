## VecCore Backends

This page describes each backend available in VecCore. Each backend has
advantages and disadvantages, depending on the circumstances. The advantage of
using VecCore rather than the libraries themselves is that with VecCore the
backend can usually be swapped without requiring any changes to the user's own
algorithms, to take advantage of the implementation with the best performance,
while also having a fallback on architectures not supported by the SIMD
libraries themselves. VecCore interfaces are also annotated for CUDA, so in
combination with the scalar backend this allows one to write code that is
portable between CPU and GPU where that makes sense. With the addition of new
backends, such as the backend based on C++20 `std::experimental::simd`, VecCore
also allows users to take advantage of new features automatically.

### Scalar Backend

This backend maps VecCore types to plain scalars. This choice was made such that
algorithms could be specialized for the scalar case if necessary without having
a performance penalty. It can also be used as a fallback on architectures not
supported by other backends, like on PowerPC architecture, and when writing code
that needs to run on GPUs, as VecCore API functions are annotated for CUDA.

### ScalarWrapper Backend

This backend is similar to the scalar backend, but maps VecCore types to
wrappers around plain scalars, so that a better syntax can be used for accessing
elements (e.g. `v[i]` instead of `vecCore::Get(v, i)`). Although it may incur a
performance penalty relative to the plain scalar backend in some cases, it often
performs almost the same, while allowing more readable code to be written in a
portable way.

### Vc Backends

The [Vc](https://github.com/VcDevel/Vc) library is used to offer several
different backends. The `VcScalar` backend functions in a similar way to the
`ScalarWrapper` backend that is native to VecCore. It simply maps to wrappers
around scalars and allow writing code with better syntax. The `VcVector` backend
is the main SIMD backend used by VecCore users. It offers excellent performance
with SSE2, SSE4, and AVX2. However, it should not be used with AVX-only since Vc
does not add fallback implementations for the integer vector types when they are
not available in hardware. This backend does not support newer instructions sets
like AVX512. The Vc library has been in maintenance mode since a few years, with
new effort being focused on the C++20 standard for vector data types
(`std::experimental::simd`). VecCore also offers wrappers to vectors of fixed
length using Vc, via the `VcSimdArray<N>` backend. This backend is templated on the
SIMD length and can be used where varying the vector length depending on the
architecture is not desirable.

### UME::SIMD Backends

The [UME::SIMD](https://github.com/edanor/umesimd) backend was added in order to
have a way to support AVX512, as that was the focus of the library when it was
created. However, since the UME::SIMD library has not seen any development in a
long time, this backend should be considered deprecated and shall be removed in
a future version of VecCore. Although performance with AVX512 is very good with
the UME::SIMD backends, UME::SIMD only compiles with the Intel C/C++ compiler in
AVX512 mode and therefore cannot be used with GCC or Clang. Although this
problem does not affect SSE4 or AVX2, this backend is not recommended for these
microarchitectures, since the UME::SIMD implementation often falls back to
scalar code for SSE4/AVX2 and does not perform as well as the Vc backends in
these cases.

### C++20 SIMD Backend (using `std::experimental::simd`)

The latest backend to be added to VecCore, the C++20 SIMD backend uses the
implementation of `std::experimental::simd` provided by GCC 11. This backend
offers the best of both Vc and UME::SIMD, as it performs well in most cases,
from SSE2 up to AVX512. However, it does require a recent version of GCC and
compiling your code with C++20. This backend also offers scalar wrappers in the
form of the `SIMDScalar` backend in VecCore, which uses the scalar ABI provided
by `std::experimental::simd`. The recommended way to use this backend is via the
`SIMDNative` VecCore backend, which will map all vector types to their native
SIMD types depending on what is enabled at compile time. While VecCore does
offer the templated `SIMDArray<N>` backend that maps to fixed-size SIMD types
from the standard implementation, it is not recommended to be used for the
moment, as performance is significantly impacted relative to the native backend.
