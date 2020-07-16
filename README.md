# VecCore

**VecCore** is a simple abstraction layer on top of other vectorization
libraries. It provides an architecture-independent [API](doc/api.md) for
expressing vector operations on data. Code written with this API can then
be dispatched to one of several *backends* like [Vc](https://github.com/VcDevel/Vc),
[UME::SIMD](https://github.com/edanor/umesimd), or a scalar implementation.
This allows one to get the best performance on platforms supported by Vc and
UME::SIMD without losing portability to unsupported architectures like ARM
and PowerPC, for example, where the scalar backend can be used instead without
requiring changes in user code. Another advantage is that, unlike with compiler
intrinsics, the same code can be compiled for SSE, AVX2, AVX512, etc, without
modifications.

VecCore was originally developed as part of the
[VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom) library, to enable portable
SIMD programming in both [VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom) and
[GeantV](https://geant.cern.ch) projects.

VecCore is now part of the [ROOT project](https://github.com/root-project) on
GitHub. The main repository is available at
[https://github.com/root-project/veccore](https://github.com/root-project/veccore).

## Supported Platforms

VecCore supports Linux, Mac OS X, and Windows. To compile software using
VecCore, you will need a compiler with support for C++11. We recommend using at
least the following compiler versions:

 - GCC 5.3.0
 - Clang 4.0
 - AppleClang 7.3.0
 - Intel® C/C++ Compiler 18.0
 - Microsoft Visual Studio 15 2017

Additionally, you will need CMake 3.9 or later, and you may want to install
a SIMD library such as

 - [Vc](https://github.com/VcDevel/Vc) (version 1.4 or later)
 - [UME::SIMD](https://github.com/edanor/umesimd) (version 0.8.1 or later)

and/or

 - [Nvidia's CUDA SDK](http://developer.nvidia.com/cuda) (version 8.0 or later).

## Support and Contact

For support and other questions, please contact Guilherme Amadio at <amadio@cern.ch>.

