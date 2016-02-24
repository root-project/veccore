VecCore Library Backend Requirements
====================================

The VecCore library contains the SIMD backends and other utilities that are
common to both VecGeom and GeantV. This document describes the requirements
that a SIMD backend needs to satisfy in order to be used in VecGeom/GeantV
from VecCore.

A backend is a structure used to group SIMD vector types together to allow
their use in generic algorithm implementations. The role of the backend is
to abstract away the differences in interface of SIMD class implementations,
exposing them to VecGeom and GeantV through a common VecCore interface,
which is described below.

Backend Interface Requirements
------------------------------

VecCore provides the following scalar types (in namespace vecCore):

```cpp
Bool_t, Int_t, Int16_t, Int32_t, UInt_t, UInt16_t, UInt32_t, Float_t, Double_t
```

For each integer and floating point type, the backend _must_ provide an
equivalent SIMD class vector types, designated either by

```cpp
template <typename T> Backend::Vector<T>;
```

for a given type `T` in the set of scalars (integer and floating point), or by

```cpp
Int_v, Int16_v, Int32_v, UInt_v, UInt16_v, UInt32_v, Float_v, Double_v
```

that follow the interface requirements for constructors, masking, etc, of
the VecCore library.

Since for booleans the number of elements is dependent on the vector type
(e.g., in AVX2 32bit types have 8 elements, while 64bit types have only 4),
it is not possible to have a single `Bool_v` that works for all types.
Instead, each SIMD vector class must provide a mask type alias inside it,
such that a mask can be created, for example, as

```cpp
typename Float_v::Mask m;
```

VecCore backend mask types must also follow a VecCore interface.

SIMD Vector Class Interface Requirements
----------------------------------------

For each SIMD vector class in a VecCore backend, the following interfaces
and operations must be defined:

### Constructors

- From a scalar constant literal (with auto-promotion in arithmetic expressions)
- From a compatible scalar variable (e.g. auto-promotion of `Real_t` to `Real_v`)
- From a pointer or reference to scalar (e.g. use `Float_t*` to load `Float_v`)

### SIMD Vector Class Interface

- Operator overloading for common bitwise, logical, and arithmetic operations
- A static member function `size()` returning the number of elements in the class
- An `operator[]` for indexing within the elements of the SIMD vector
- An `operator()(Mask&)` for masked assignments (e.g.: `Float_v x, y; x(y < 0.0) = 0.0;`)
- Member functions `load()` and `store()` to allow reading from/writing to memory addresses

### Standard Math Functions

Backend types must be convertible to native SIMD types in such as way as to
be acceptable as a parameter to standard math functions, or these functions
must be defined in the backend allowing calls to, e.g., `sin()` and `cos()`
using `Float_v` in the same way as a `Float_t`.

SIMD Vector Masking Interface Requirements
------------------------------------------

Each SIMD backend must provide specializations for the following template
functions:

- Check if a mask empty or full
```cpp
template <class Mask> Bool_t IsEmpty(const Mask&);
template <class Mask> Bool_t IsFull(const Mask&);
```

- Blend two variables using a mask
```cpp
template <class T, class Mask>
T Blend(const Mask mask, const T& tval, const T& fval);
```

- Perform assignment with a mask
```cpp
template <class T, class Mask>
void MaskAssign(T& dest, const T::Mask mask, const T& src);
```

SIMD Scatter/Gather Interface
-----------------------------

This part of the interface has not been specified yet.

VecCore Backend Testing
-----------------------

The compatibility of a backend can be tested in VecCore with a backend
conformance test covering calls to the interface defined above. If a backend
passes the conformance test, it is deemed compatible with VecCore. Any code
breakage then implies a failure of either the backend itself or the backend
test, and should be fixed by updating the test or the failing backend classes.

