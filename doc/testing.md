# Testing VecCore

VecCore uses [Google Test](https://github.com/google/googletest) for testing
current and new backend implementations. The main test is `test-backends` in
the `test` directory. This is a conformance test to verify that all VecCore
API functions and their fallback implementations work with a given backend.

## Running VecCore tests

A CTest script has been added to the repository to aid testing VecCore in
many configurations with as minimal effort as possible. It can be run as

```shell
$ ctest -VV -S cmake/TestVecCore.cmake
```

In order to test a different compiler, set the `CC` and `CXX` variables

```shell
$ CC=clang CXX=clang++ ctest -VV -S cmake/TestVecCore.cmake
```

To test a specific target microarchitecture, pass it as an argument to the
CTest script. For example, here is how to test for SSE4.2, AVX2, and AVX512

```
$ ctest -VV -S cmake/TestVecCore.cmake,SSE4.2
$ ctest -VV -S cmake/TestVecCore.cmake,AVX2
$ ctest -VV -S cmake/TestVecCore.cmake,AVX512
```

It is also possible to specify `Native` in most cases to test on the native SIMD
architecture of your machine. This will add `-march=native` to the compilation
flags. Note, however, that Visual Studio does not offer such option, and both
GCC and Clang usually only support `-march=native` on `x86_64`. On ARM/ARM64,
the recommended way to enable SIMD instructions is by setting `CMAKE_CXX_FLAGS`
manually during configuration. For example, on a Raspberry Pi 4B, the flags `-O3
-ffast-math -mcpu=cortex-a72` should give optimal performance with both GCC and
Clang. It may also be possible to use `-mcpu=native` instead of a specific CPU
model. The flag `-ffast-math` or at least `-funsafe-math-optimizations` needs to
be added to the compilation flags because NEON instructions are not fully
compliant with the IEEE 754 standard for floating-point arithmetic which may
lead to loss of precision.

Running CTest as shown above will by default upload build and test results to
[CERN's CDash](https://cdash.cern.ch/index.php?project=VecCore) instance.
If you do not wish to upload your results, please comment out the CDash settings
in `cmake/TestVecCore.cmake` before running CTest.

The CTest script also supports a few options to control the build:

* Use `-DPARALLEL=1` to enable parallel builds using the number of available
  physical cores on your machine
* Use `-DMEMCHECK=1` to enable the `ctest_memcheck()` step with `valgrind`
  when `valgrind` is available
* Use `-DCMAKE_ARGS=...` to pass extra configuration options for VecCore,
  like for enabling/disabling a backend, setting C++ standard, etc

Although the CMake default generator is detected and set by the script,
exporting the `CMAKE_GENERATOR` variable allows to run with, e.g. Ninja, rather
than with the default of UNIX Makefiles. Other CMake/CTest environment variables
such as `CTEST_PARALLEL_LEVEL` and `CMAKE_BUILD_PARALLEL_LEVEL` can be exported
to control the build. The build type can be set by exporting the environment
variable `CTEST_CONFIGURATION_TYPE` or via the command line, as in the example
below

```
$ ctest -VV -C Release -S cmake/TestVecCore.cmake
```

All of the CTest commands above should work on Linux, Mac OS, and Windows.

The `TestVecCore.cmake` script is what runs on GitHub Actions and GitLab CI, so
if you have your own fork of the repository, you can use the CI actions instead
of running tests locally on your machine.
