VecCore Library Backend Requirements
====================================

The VecCore library will house the basic utilities such as SIMD backends, random
number generation, basic linear algebra, etc, that are common to both VecGeom
and GeantV. This document describes the requirements that a SIMD backend needs
to satisfy in order to be used in VecGeom/GeantV from VecCore.

A backend is simply a type traits structure used to group vector types to be
used together in generic algorithm implementations. A backend provides the basic
scalar types, namely, Bool_t, Float_t, Double_t, Real_t (configurable to either
float or double), Int_t (matching Real_t), and vector types that are the SIMD
equivalents of the scalar types (with the exception of Bool_v, since masks can
have different types). The vector types are Real_v, Int_v, etc. Their respective
mask types need to be provided as Real_v::Mask, Int_v::Mask, and so on, so that
the mask size matches the number of elements of its associated type. The basic
requirements and interfaces for the vector types are described below.

For each vector type (we will use Real_v as an example throughout this
document), the following types, interfaces, and operations must be defined:

- Backend Structure
  - The backend structure must accept a parameter template that determines
    the default precision for floating point calculations (i.e., the type
    of Real_t), and it must choose types for Int_t, and UInt_t that match
    the size of Real_t

- Constructor
  - From a constant literal (with auto-promotion in arithmetic expressions)
  - From a compatible scalar variable (e.g. auto-promotion of Real_t to Real_v)
  - From a reference or pointer (e.g. pointer to float or double for Real_v)
  - From a similar vector type constant or another variable

- Load/Store
  - Backend types (Int_v, Real_v, etc) must provide member functions load()
    and store() to allow reading from and writing into a memory address

- Basic interface
  - Each vector class must define a Size constant that returns the number of
    elements (e.g. Real_v::Size)
  - Each vector class should provide operator[] for indexing
  - Additionally, operator()(Mask&) should allow for assignments with a
    mask (e.g.: Real_v x, y; x(y < 0.0) = 0.0; should be possible)
  - Each vector class should provide operator overloading for common arithmetic
    operations supported by their associated scalar types (e.g. Int_t vs Int_v)
  - Similarly, overloading should be defined for bitwise and logical operations

- External Interface and Standard Math Functions
  - Backend types must be convertible to SIMD types in such a way as to be
    acceptable as a parameter to standard math functions such as std::min(),
    std::sin(), etc.
  - In order to allow generic treatment of SIMD code, a few functions must be
    defined:
    - `template <class Mask> bool IsFull(Mask&);`
    - `template <class Mask> bool IsEmpty(Mask&);`
    - `template <typename T> T Blend(T::Mask&, T val1, T val2);`
    These functions are used for checking if a Mask has any active/inactive SIMD
    lanes, and to allow blending two variables into a third using a mask.
  - Generic templates are provided in VecCore for some of these functions. If
    the generic template don't work for a specific backend, specializations must
    be provided in the backend header for these interface functions in such a way
    as to not conflict with other backends.

- Testing
  - Backend compatibility/conformance is to be checked via unit tests
  - If a backend passes the unit tests, it is deemed compatible, and any
    breakage must be fixed either by adding new requirements to this document
    and/or respecitve changes to the tests, or by forbidding the broken syntax

Backend Example
===============

An example of a backend structure for the Vc library is shown below:

```cpp
#ifndef VECCORE_BACKEND_VC_VECTOR_H
#define VECCORE_BACKEND_VC_VECTOR_H

#ifdef VECCORE_ENABLE_VC

#include <cstdint>

#include <Vc/Vc>

namespace vecCore {
namespace backend {

template <typename FloatT>
struct VcVector {

  typedef bool     Bool_t;

  typedef int16_t  Int16_t;
  typedef int32_t  Int32_t;
  typedef int64_t  Int64_t;

  typedef uint16_t UInt16_t;
  typedef uint32_t UInt32_t;
  typedef uint64_t UInt64_t;

  typedef float    Float_t;
  typedef double   Double_t;
  typedef FloatT   Real_t;

  template <typename>  Int;
  template <> Int<float>  { typedef int32_t type; }
  template <> Int<double> { typedef int64_t type; }

  typedef typename Int<Real_t>::type Int_t;
  typedef typename unsigned Int<Real_t>::type UInt_t;

  typedef Vc::Vector<Real_t>   Real_v;
  typedef Vc::Vector<Float_t>  Float_v;
  typedef Vc::Vector<Double_t> Double_v;

  typedef Vc::Vector<Int_t>    Int_v;
  typedef Vc::Vector<Int16_t>  Int16_v;
  typedef Vc::Vector<Int32_t>  Int32_v;
  typedef Vc::Vector<Int64_t>  Int64_v;

  typedef Vc::Vector<UInt_t>   UInt_v;
  typedef Vc::Vector<UInt16_t> UInt16_v;
  typedef Vc::Vector<UInt32_t> UInt32_v;
  typedef Vc::Vector<UInt64_t> UInt64_v;

};

} // namespace backend
} // namespace vecCore

#endif
#endif
```
