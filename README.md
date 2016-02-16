VecCore Library
===============

The VecCore library provides a SIMD abstraction layer to VecGeom and other
components of the GeantV project.

The official repository for VecCore is hosted at CERN's GitLab at
[https://gitlab.cern.ch/VecGeom/VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom).

Supported Platforms
-------------------

VecCore supports Linux and Mac OS X. To compile VecCore you need a modern
compiler with support for C++11. VecCore is tested with the following compilers:

- GCC 4.8.1
- Clang 3.1
- Intel® C/C++ Compiler 15.0.3

Additionally, you have to install CMake 3.0 or greater, and you may need to
install a SIMD library such as [Vc](https://github.com/VcDevel/Vc) version 1.0.0
or greater, and/or [Nvidia's CUDA SDK](http://developer.nvidia.com/cuda) version
7.5 or greater.

VecCore Support and Contact
---------------------------

If you encounter any bugs, please report them via
[VecGeom's JIRA Issue Tracker](https://its.cern.ch/jira/browse/VECGEOM).

For questions, please write a message to the GeantV development team at
<geant-dev@cern.ch>.

