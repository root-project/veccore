# VecCore Library

|**GitLab CI**|**Travis CI**|
|-------------|-------------|
|[![build status](https://gitlab.cern.ch/VecGeom/VecCore/badges/master/build.svg)](https://gitlab.cern.ch/VecGeom/VecCore/commits/master)|[![Build Status](https://travis-ci.org/root-project/veccore.svg?branch=master)](https://travis-ci.org/root-project/veccore)|

**VecCore** is an open source SIMD abstraction library. It provides an
architecture independent [API](doc/API.md) for expressing vector operations on
data.

VecCore was originally developed as part of the
[VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom) library, to enable portable
SIMD programming in both [VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom) and
[GeantV](https://geant.cern.ch) projects.

VecCore is now under the umbrella of the ROOT project on GitHub, and is available at
 - [https://github.com/root-project/veccore](https://github.com/root-project/veccore).

## Supported Platforms

VecCore supports Linux and Mac OS X. To compile software using the VecCore
library, you need a compiler with support for C++11. We recommend using at least
the following compiler versions:

 - GCC 5.3.0
 - Clang 3.8.0
 - AppleClang 7.3.0
 - Intel® C/C++ Compiler 16.0.2

Additionally, you will need CMake 3.6 or later, and you may want to install
a SIMD library such as

 - [Vc](https://github.com/VcDevel/Vc) (version 1.3.3 or later)
 - [UME::SIMD](https://github.com/edanor/umesimd) (version 0.8.1 or later)

and/or

 - [Nvidia's CUDA SDK](http://developer.nvidia.com/cuda) (version 7.5 or later).

## Support and Contact

For support and other questions, please contact Guilherme Amadio at <amadio@cern.ch>.

