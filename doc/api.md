## VecCore API

VecCore defines *backend* structures in order to group similar
[SIMD](https://en.wikipedia.org/wiki/SIMD) vector types together to allow their
use in hardware independent implementations of vectorized algorithms. The
backend types are also used to define a uniform interface for operations such as
masking, loading and storing data, etc. Below we give an overview of the common
interfaces VecCore offers to abstract over SIMD vector types from each backend.

### Backend Types

Historically, VecCore has offered the following alias to common scalar types:

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

Since these types are part of standard C++, newer versions of VecCore use them
directly in its implementation, and users are also encouraged to switch. The
type aliases above should be considered deprecated and will be removed in a
future version of VecCore.

Each VecCore backend uses the scalar types to provide equivalent SIMD vector
types. For example, the [Vc](https://github.com/VcDevel/Vc) backend defines the
following types:

```cpp
namespace vecCore {
namespace backend {
  struct VcVector {
    using Float_v  = Vc::Vector<float>;
    using Double_v = Vc::Vector<double>;
 
    using Int_v    = Vc::Vector<int>;
    using Int32_v  = Vc::Vector<int32_t>;
    using Int64_v  = Vc::Vector<int64_t>;
 
    using UInt_v   = Vc::Vector<unsigned>;
    using UInt32_v = Vc::Vector<uint32_t>;
    using UInt64_v = Vc::Vector<uint64_t>;
  };
} // namespace backend
} // namespace vecCore
```

Note that the absence of a boolean type above is intentional. Since most SIMD
hardware units have a fixed width in bits (i.e. 128 bits for SSE, 256 bits for
AVX, etc), the number of elements in a vector boolean type (i.e. a SIMD mask)
is different for each scalar type. For example, the native SIMD vector type
for `Double_v` when AVX2 is enabled usually contains 4 elements, while `Float_v`
contains 8 elements. Moreover, there can be substantial differences in hardware
as well. [Intel® Xeon Phi™](http://www.intel.com/xeonphi) processors and
coprocessors introduced hardware mask registers instead of using regular SIMD
registers for masks, as is done in Core i7 processors (Skylake and earlier).
Later, with the addition of AVX512, hardware mask registers have also become 
available on Xeon and newer Core processors. For more information about SIMD
programming on Intel® hardware, please consult the [Intel® 64 and IA-32 Architectures
Software Developer Manuals](https://software.intel.com/en-us/articles/intel-sdm).

In VecCore, mask types are defined in terms of their associated vector types via
type traits. For example, `Mask<Float_v>` is the mask type for `Float_v`, and
`Mask<Double_v>` the mask type for `Double_v`. Similarly, the type traits define
the alias `Index<T>` which denotes a suitable vector index type for `T`, and
`Scalar<T>` which maps to the underlying scalar type of `T`. These dependent
types are needed to define the interfaces for gather/scatter in terms of pointers
to scalars and vector indices, for example, but are also useful in various other
situations.

### SIMD Operations

For each backend type `T` representing a SIMD vector type, `M` its associated mask
type, VecCore defines the following functions:

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

  bool EarlyReturnAllowed();

  template <typename T> bool EarlyReturnMaxLength(T &v, size_t n);

  template <typename T> Scalar<T> ReduceAdd(const T &v);
  template <typename T> Scalar<T> ReduceMin(const T &v);
  template <typename T> Scalar<T> ReduceMax(const T &v);
}
```

![VecCore API](api.svg)

These functions can be used to write SIMD-enabled algorithms that can be
instantiated with different backends depending on the configuration during
compilation. For example, below is an example of such algorithm for computing
the [Mandelbrot Set](https://en.wikipedia.org/wiki/Mandelbrot_set). First, the
scalar version:

```cpp
template<typename T>
void mandelbrot(T xmin, T xmax, size_t nx,
                T ymin, T ymax, size_t ny,
                size_t max_iter, unsigned char *image)
{
    T dx = (xmax - xmin) / T(nx);
    T dy = (ymax - ymin) / T(ny);

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; ++j) {
            size_t k = 0;
            T x = xmin + T(i) * dx, cr = x, zr = x;
            T y = ymin + T(j) * dy, ci = y, zi = y;

            do {
                x  = zr*zr - zi*zi + cr;
                y  = T(2.0) * zr*zi + ci;
                zr = x;
                zi = y;
            } while (++k < max_iter && (zr*zr + zi*zi < T(4.0)));

            image[ny*i + j] = k;
        }
    }
}
```

Then its VecCore equivalent implementation:

```cpp
template<typename T>
void mandelbrot_v(Scalar<T> xmin, Scalar<T> xmax, size_t nx,
                  Scalar<T> ymin, Scalar<T> ymax, size_t ny,
                  Scalar<Index<T>> max_iter, unsigned char *image)
{
    T iota;
    for (size_t i = 0; i < VectorSize<T>(); ++i)
        Set<T>(iota, i, i);

    T dx = T(xmax - xmin) / T((Scalar<T>)nx);
    T dy = T(ymax - ymin) / T((Scalar<T>)ny), dyv = iota * dy;

    for (size_t i = 0; i < nx; ++i) {
        for (size_t j = 0; j < ny; j += VectorSize<T>()) {
            Scalar<Index<T>> k(0);
            T x = xmin + T((Scalar<T>)i) * dx,       cr = x, zr = x;
            T y = ymin + T((Scalar<T>)j) * dy + dyv, ci = y, zi = y;

            Index<T> kv(0);
            Mask<T> m(true);

            do {
                x = zr*zr - zi*zi + cr;
                y = T((Scalar<T>)2.0) * zr*zi + ci;
                MaskedAssign<T>(zr, m, x);
                MaskedAssign<T>(zi, m, y);
                MaskedAssign<Index<T>>(kv, m, ++k);
                m = zr*zr + zi*zi < T((Scalar<T>)4.0);
            } while (k < max_iter && !MaskEmpty(m));

            for (size_t k = 0; k < VectorSize<T>(); ++k)
                image[ny*i + j + k] = (unsigned char) Get(kv, k);
        }
    }
}
```

While the two implementations are similar to each other, some important
differences should be noted. The `do/while` loop, for example, needs to process
several pixels of the image at once, hence a mask is required to check for loop
termination rather than a simple counter. The mask is also used to restrict the
elements to which an update should be applied at each iteration. This is a
common theme in SIMD programming. For more examples of the application of the
VecCore interfaces, we encourage the reader to read the Mandelbrot and other
examples in the [VecCore repository on GitHub](https://github.com/root-project/veccore/tree/master/bench).

### Arithmetics, Comparisons, and Logical Operations

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

An important aspect of VecCore to keep in mind is that the backends provide
direct aliases to the real types offered by the different libraries like Vc,
UME::SIMD, and now std::experimental::simd. This is the main reason why we
cannot use `v[i]` to access elements of a SIMD vector, as that would not work
with plain scalars (i.e. `float` or `int`). We need to provide this interface
via a function instead, in the form of `vecCore::Get(v, i)`. Nevertheless, if
the user knows they will never need to use the plain scalar backend (or if using
the provided wrappers for scalars in the `ScalarWrapper` backend, despite a
potential performance penalty), then it is fine to use `v[i]` to access elements
of SIMD vectors, as that is actually provided by all SIMD backends. However,
other restrictions also apply. For example, in order to construct a SIMD vector
of `float`s using `std::experimental::simd`, a plain constant like `0.0` is not
acceptable, so it may be necessary to cast integer and double precision
constants to `float`. The Vc and UME::SIMD backends do not have this restriction.
Comparing masks is also not recommended, as the result may be either another
mask or a boolean that is true if the masks are identical depending on the
backend.

### Mathematical Functions

VecCore provides implementations for mathematical functions with SIMD types for
each backend in the namespace `vecCore::math`. In recent versions, this
namespace has been converted into an inline namespace to avoid long names for
the functions. Function names were capitalized to avoid clashes with standard
functions, but they map to standard functions when the backend library does not
offer a more specialized implementation. Therefore, calling `vecCore::Sin(x)`,
for example, is possible with any of the backend types. Please access the online
[Doxygen documentation](https://root-project.github.io/veccore/namespacevecCore_1_1math.html)
for a full list of all the mathematical functions defined in the backends.
