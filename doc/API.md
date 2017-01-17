# VecCore API Reference

VecCore defines *backend* structures in order to group similar
[SIMD](https://en.wikipedia.org/wiki/SIMD) vector types together to allow their
use in hardware independent implementations of vectorized algorithms. The
backend types are also used to define a uniform interface for operations such as
masking, loading and storing data, etc.

## Backend Types

VecCore defines the following scalar types:

```cpp
namespace vecCore {
  using Bool_s   = bool;

  using Float_s  = float;
  using Double_s = double;

  using Int_s    = int32_t;
  using Int32_s  = int32_t;
  using Int64_s  = int64_t;

  using UInt_s   = uint32_t;
  using UInt32_s = uint32_t;
  using UInt64_s = uint64_t;
}
```

Each VecCore backend then uses these scalar types to provide equivalent vector
types. For example, the [Vc](https://github.com/VcDevel/Vc) backend defines the
following types:

```cpp
namespace vecCore {
namespace backend {

struct VcVector {
  using Float_v  = Vc::Vector<Float_s>;
  using Double_v = Vc::Vector<Double_s>;

  using Int_v    = Vc::Vector<Int_s>;
  using Int32_v  = Vc::Vector<Int32_s>;
  using Int64_v  = Vc::Vector<Int64_s>;

  using UInt_v   = Vc::Vector<UInt_s>;
  using UInt32_v = Vc::Vector<UInt32_s>;
  using UInt64_v = Vc::Vector<UInt64_s>;
};

}
}
```

The absence of a boolean type above is intentional. Since most SIMD hardware
units have a fixed width in bits (i.e. 128 bits for SSE, 256 bits for AVX, etc),
the number of elements in a vector boolean type is different for each scalar
type. For AVX, the type `Double_v` contains 4 elements, while `Float_v` contains
8 elements. Moreover, there can be substantial differences in hardware as well.
[Intel® Xeon Phi™](http://www.intel.com/xeonphi) processors and coprocessors
introduced hardware mask registers instead of using regular SIMD registers for
masks, as is done in Core i7 processors (Skylake and earlier). For more
information on SIMD programming on Intel® hardware, please consult the
[Intel® 64 and IA-32 Architectures Software Developer Manuals](https://software.intel.com/en-us/articles/intel-sdm).
In VecCore, mask types are defined in terms of their associated vector types.
For example, `Mask<Float_v>` is the mask type for `Float_v`, and
`Mask<Double_v>` the mask type for `Double_v`. Similarly, `Index<T>` denotes a
suitable vector index type for `T`, and `Scalar<T>` denotes the underlying
scalar type of `T`. These dependent types are needed to define the interfaces
for gather/scatter in terms of pointers to scalars and vector indices, for
example, but are also useful in various other situations.

## VecCore API

For each backend type `T`, and associated mask type `M`, VecCore defines the
following API functions:

```cpp
namespace vecCore {
  template <typename T> constexpr size_t VectorSize();

  template <typename T> Scalar<T> Get(const T &v, size_t i);
  template <typename T> void Set(T &v, size_t i, Scalar<T> const val);

  template <typename T> void Load(T &v, Scalar<T> const *ptr);
  template <typename T> void Store(T const &v, Scalar<T> *ptr);

  template <typename T, typename S = Scalar<T>>
  T Gather(S const *ptr, Index<T> const &idx);

  template <typename T, typename S = Scalar<T>>
  void Scatter(T const &v, S *ptr, Index<T> const &idx);

  template <typename M> bool MaskFull(M const &mask);
  template <typename M> bool MaskEmpty(M const &mask);

  template <typename T> void MaskedAssign(T &dst, const Mask<T> &mask, const T &src);
  template <typename T> T Blend(const Mask<T> &mask, const T &src1, const T &src2);
}
```

## Arithmetics, Comparisons, and Logical Operations

VecCore backend types support usual arithmetic operations, such as addition,
multiplication, etc, except for the modulus operation, which is not supported by
most hardware. Comparisons and logical operations are also supported, with the
caveat that the result of a vector comparison or logical operation is not a
boolean, but an appropriate mask type. Therefore, such operations should not be
part of an `if` or similar construct, as this will almost certainly lead to a
bug. The correct way to use such expressions in branching code is to test wether
the mask has none, any, or all of its elements set, and take appropriate action:

```cpp
// example: set negative elements of x to 0.0

Double_v Clamp(Double_v x) {
  Mask<Double_v> m = x < 0.0;
  if (!MaskEmpty(m)) {
    MaskedAssign(x, m, 0.0);
  }
}
```

In the case above, the condition may be ommitted, but in cases where the
operations inside the condition are expensive, it is worth to check if any
elements really need to be calculated.

