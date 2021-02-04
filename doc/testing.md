# Testing VecCore

VecCore uses [Google Test](https://github.com/google/googletest) for testing
current and new backend implementations.

## Backend Conformance Test

The [backend conformance test](test/backend.cc) covers calls to the
[VecCore API](doc/api.md). If a backend passes this conformance test, it is
deemed compatible with VecCore. Any code breakage then implies a failure of
either the backend itself or the backend test, and should be fixed by
updating the test or the failing backend classes.

## Running the Backend Test

To run the backend test, use the following commands:

```shell
$ cd VecCore
$ mkdir build && cd build
$ cmake .. -DBUILD_TESTING=ON
$ make
$ ./test/backend 
```

Make sure the backend you intend to test is enabled in your
cmake command line options (e.g. -DVC=ON or -DUMESIMD=ON).

