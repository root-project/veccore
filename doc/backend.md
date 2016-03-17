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

For each scalar integer and floating point type, the backend _must_ provide
equivalent SIMD vector types for the scalar type, and for its masks and
indices, designated by

```cpp
Int_v, Int16_v, Int32_v, UInt_v, UInt16_v, UInt32_v, Float_v, Double_v
```

and

```cpp
template <typename T> Backend::Mask_v<T>;
template <typename T> Backend::Index_v<T>;
template <typename T> Backend::VectorSize<T>(const T&);
```

where `T` might be a scalar or vector type. That is, `Mask_v(Real_v)`
and `Mask_v(Real_t)` are equivalent for a given backend.

The SIMD classes need to follow the interface requirements for constructors,
masking, etc, of the VecCore library.

Since for booleans the number of elements is dependent on the vector type
(e.g., in AVX2 32bit types have 8 elements, while 64bit types have only 4),
it is not possible to have a single `Bool_v` that works for all types.
Instead, each backend _must_ provide a SIMD template that takes a SIMD
type as argument and defines a `Type` alias inside it, such that a mask for
SIMD type `T` can be created as

```cpp
typename Backend::Mask_v<T> mask;
```

This structure is important to avoid using a dependent type name such as
`Real_v::Mask` that cannot be defined for scalar types, so that a purely
scalar backend can be defined where it is supported. Nevertheless, VecCore
backend mask types also _must_ follow a VecCore interface.

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
- An `operator[]` for indexing within the elements of the SIMD vector
  (care must be taken, since a pure scalar backend has been added that doesn't
  have such funtionality, requiring specialization in those cases).
- Backend functions `Load()` and `Store()` to allow reading from/writing to memory addresses

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
template <class Mask> Bool_t MaskEmpty(const Mask&);
template <class Mask> Bool_t MaskFull(const Mask&);
```

- Blend two variables using a mask
```cpp
template <class T, class Mask>
T Blend(const Mask mask, const T& tval, const T& fval);
```

- Perform assignment with a mask
```cpp
template <class T, class Mask>
void MaskedAssign(T& dest, const Mask mask, const T& src);
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

