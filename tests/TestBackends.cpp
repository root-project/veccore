/*
 * TestBackends.cpp
 *
 *  Created on: 11.05.2015
 *      Author: swenzel
 */

// file meant to systematically test the backend api

#include "backend/Vc/Backend.h"
#include "backend/scalar/Backend.h"
#undef NDEBUG
#include <cassert>

using VecCore::Backend::Vector::GetComponent;
using VecCore::Backend::Vector::ComponentAssign;
using VecCore::Backend::Scalar::GetComponent;
using VecCore::Backend::Scalar::ComponentAssign;

template <typename Backend>
void TestBackends( typename Backend::Real_v const & input ) {
// At least one test for all backendfunctions

    // get all components/lanes of input
    for( int i=0; i<Backend::kRealVectorSize; ++i )
        std::cout << " input[" << i << "] " << GetComponent( input, i ) << "\n";

    typename Backend::Real_v copy = input;

    for( int i=0; i<Backend::kRealVectorSize; ++i )
        ComponentAssign( i , typename Backend::Real_t(i), copy );

    for( int i=0; i<Backend::kRealVectorSize; ++i )
        std::cout << " copy[" << i << "] " << GetComponent( copy, i ) << "\n";

    /*
    typename Vc::Vector<Type>::EntryType &
    GetWritableComponent( typename Vc::Vector<Type> & x, int index ) {
        return x(index);
    }

    // same for Mask
    template <typename Type>
    VECGEOM_INLINE
    static
    bool &
    GetWritableComponent( typename Vc::Vector<Type>::Mask & x, int index ) {
        return x(index);
    }

    // might need to abstract on other things
    // LoadFromArray; StoreToArray


    template <typename Type>
    VECGEOM_INLINE
    static
    void CondAssign(typename Vc::Vector<Type>::Mask const &cond,
                    Vc::Vector<Type> const &thenval,
                    Vc::Vector<Type> const &elseval,
                    Vc::Vector<Type> *const output) {
      (*output)(cond) = thenval;
      (*output)(!cond) = elseval;
    }

    template <typename Type>
    VECGEOM_INLINE
    void CondAssign(typename Vc::Vector<Type>::Mask const &cond,
                    Type const &thenval,
                    Type const &elseval,
                    Vc::Vector<Type> *const output) {
      (*output)(cond) = thenval;
      (*output)(!cond) = elseval;
    }

    template <typename Type>
    VECGEOM_INLINE
    void MaskedAssign(typename Vc::Vector<Type>::Mask const &cond,
                      Vc::Vector<Type> const &thenval,
                      Vc::Vector<Type> *const output) {
      (*output)(cond) = thenval;
    }

    template <typename Type>
    VECGEOM_INLINE
    void MaskedAssign(typename Vc::Vector<Type>::Mask const &cond,
                      Type const &thenval,
                      Vc::Vector<Type> *const output) {
      (*output)(cond) = thenval;
    }

    // special version of MaskedAssignment when output
    // is of type int_v
    VECGEOM_INLINE
    void MaskedAssign(VcBool const &cond,
                      const Inside_t thenval,
                      VcInside *const output) {
      (*output)(VcInside::Mask(cond)) = thenval;
    }

    // returns if all lanes/slots in (vector) condition are true
    template <typename Type>
    VECGEOM_INLINE
    bool IsFull(typename Vc::Vector<Type>::Mask const &cond) {
      return cond.isFull();
    }

    // returns if any lane/slot in (vector) condition is true
    template <typename Type>
    VECGEOM_INLINE
    bool Any(typename Vc::Vector<Type>::Mask const &cond) {
      return !cond.isEmpty();
    }

    // returns if all lanes/slots in (vector) condition are false
    template <typename Type>
    VECGEOM_INLINE
    bool IsEmpty(typename Vc::Vector<Type>::Mask const &cond) {
      return cond.isEmpty();
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> Abs(typename Vc::Vector<Type> const &val) {
      return Vc::abs(val);
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> Sqrt(typename Vc::Vector<Type> const &val) {
      return Vc::sqrt(val);
    }

    template <typename Type>
    VECGEOM_INLINE
    VcPrecision ATan2(typename Vc::Vector<Type> const &y,
                      typename Vc::Vector<Type> const &x) {
      return Vc::atan2(y, x);
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> sin(typename Vc::Vector<Type> const &x) {
      return Vc::sin(x);
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> cos(typename Vc::Vector<Type> const &x) {
      return Vc::cos(x);
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> tan(typename Vc::Vector<Type> const &radians) {
      // apparently Vc does not have a tan function
      // return Vc::tan(radians);
      // emulating it for the moment
      VcPrecision s,c;
      Vc::sincos(radians,&s,&c);
      return s/c;
    }

    // ??????
    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> Pow(typename Vc::Vector<Type> const &x,
                                  typename Vc::Vector<Type> & arg) {
        // What about a Vc version ?
        return std::pow(x,arg);
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> Min(typename Vc::Vector<Type> const &val1,
                                  typename Vc::Vector<Type> const &val2) {
      return Vc::min(val1, val2);
    }

    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> Max(typename Vc::Vector<Type> const &val1,
                                  typename Vc::Vector<Type> const &val2) {
      return Vc::max(val1, val2);
    }


    template <typename Type>
    VECGEOM_INLINE
    typename Vc::Vector<Type> Floor( typename Vc::Vector<Type> const &val ){
      return Vc::floor( val );
    }
*/


}

// template alias to set default backends
template<typename T>
   using DefaultVectorBackend = typename VecCore::Backend::Vector::kVc<T>;

template<typename T>
   using DefaultScalarBackend = typename VecCore::Backend::Scalar::kScalar<T>;

int main(){
    typename VecCore::Backend::Vector::kVc<float>::Real_v input1(1.);
    typename VecCore::Backend::Vector::kVc<double>::Real_v input2(1.);


    TestBackends<VecCore::Backend::Vector::kVc<float> >( input1 );
    TestBackends<VecCore::Backend::Vector::kVc<double> >( input2 );

    TestBackends<DefaultVectorBackend<float> >( input1 );

    DefaultScalarBackend<float>::Real_v sinput1(1.);
    TestBackends<DefaultScalarBackend<float> >(sinput1);

    // test a scalar API
    // TestBackends<VecCore::Backend::Scakar::kScalar<float> >( input );

    return 0;
}




