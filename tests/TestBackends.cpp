/*
 * TestBackends.cpp
 *
 *  Created on: 11.05.2015
 *      Author: swenzel
 */

// file meant to systematically test the backend api

#include "backend/Vc/Backend.h"
#include "backend/scalar/Backend.h"
#include <mm_malloc.h>
#undef NDEBUG
#include <cassert>

using VecCore::Backend::Vector::GetComponent;
using VecCore::Backend::Vector::ComponentAssign;
using VecCore::Backend::Scalar::GetComponent;
using VecCore::Backend::Scalar::ComponentAssign;
using VecCore::Backend::Scalar::CondAssign;
using VecCore::Backend::Vector::CondAssign;
using VecCore::Backend::Scalar::GetMaskComponent;
using VecCore::Backend::Vector::GetMaskComponent;

using VecCore::Backend::Vector::StoreTo;
using VecCore::Backend::Vector::LoadFrom;
using VecCore::Backend::Scalar::StoreTo;
using VecCore::Backend::Scalar::LoadFrom;

using VecCore::Backend::Vector::Any;
using VecCore::Backend::Vector::IsFull;
using VecCore::Backend::Vector::IsEmpty;
using VecCore::Backend::Scalar::Any;
using VecCore::Backend::Scalar::IsFull;
using VecCore::Backend::Scalar::IsEmpty;
using VecCore::Backend::Vector::Abs;
using VecCore::Backend::Scalar::Abs;
using VecCore::Backend::Vector::Sqrt;
using VecCore::Backend::Scalar::Sqrt;


template <typename Backend>
void TestBackends( typename Backend::Real_v const & input ) {
    // some typedefs
    typedef typename Backend::Real_v Real_v;
    typedef typename Backend::Real_t Real_t;

    std::cout << "***** START OF TESTING BACKEND FUNCTION *******\n";
    // At least one test for all backendfunctions

    // get all components/lanes of input
    for( int i=0; i<Backend::kRealVectorSize; ++i )
        std::cout << " input[" << i << "] " << GetComponent( input, i ) << "\n";

    typename Backend::Real_v copy = input;

    // check ComponentAssign
    for( int i=0; i<Backend::kRealVectorSize; ++i )
        ComponentAssign( i , typename Backend::Real_t(i), copy );

    for( int i=0; i<Backend::kRealVectorSize; ++i )
        std::cout << " copy[" << i << "] " << GetComponent( copy, i ) << "\n";

    // check ConditionalAssign
    std::cout << "-- CONDITIONAL ASSIGN + MaskComponent --\n";
    typename Backend::Bool_v condition = copy > input;

    CondAssign( condition, Real_v(-10.), Real_v(10.), &copy );
    for( int i=0; i<Backend::kRealVectorSize; ++i ){
        std::cout << " condition[" << i << "] " << GetMaskComponent(condition,i) << "\n";
        std::cout << " copy[" << i << "] " << GetComponent( copy, i ) << "\n";
    }

    // check ConditionalAssign with primitives
    std::cout << "-- CONDITIONAL ASSIGN  WITH primitives--\n";
    condition = copy > input;
    CondAssign( condition, Real_t(-10.), Real_t(10.), &copy );
    for( int i=0; i<Backend::kRealVectorSize; ++i )
          std::cout << " copy[" << i << "] " << GetComponent( copy, i ) << "\n";

    std::cout << "-- TESTING AVAILABILITY OF ISFULL, ANY, ISEMPTY --\n";
    if( IsFull(condition) ){
        std::cout << "Every lane in condition " << condition << " satisfied\n";
        // assert that this really the case
        for( int i=0; i<Backend::kRealVectorSize; ++i ) {
           assert( GetMaskComponent( condition, i) == true && "Problem in IsFull");
        }

    }
    if( Any(condition) ){
           std::cout << "Some lane in condition " << condition << " satisfied\n";
           bool valid = true;
           for( int i=0; i<Backend::kRealVectorSize; ++i)
                if( GetMaskComponent( condition, i) == true ) valid=true;
           assert( valid && "Problem in Any");
    }
    if( IsEmpty(condition) ){
      std::cout << "No lane in condition " << condition << " satisfied\n";
      for( int i=0; i<Backend::kRealVectorSize; ++i ) {
        //  assert( GetMaskComponent( condition, i) == false && "Problem in IsEmpty");
      }
    }

    // check masked assignments
    std::cout << "-- TESTING MASKED ASSIGN -- \n";


    // check Loads and Stores from Array
    std::cout << "-- TESTING LOAD/STORE ABSTRACTION --\n";
    // initialize an array first
    Real_t * array = (Real_t *) _mm_malloc( sizeof( Real_t ) * 8, 32 );
    for( int i=0;i<8;++i ) array[i]=i;
    StoreTo( copy, array );
    // assert that array really contains the stuff from copy
    for( int i=0; i<Backend::kRealVectorSize; ++i )
        assert( GetComponent( copy, i ) == array[i] && "problem in StoreTo");

    Real_v copy2;
    LoadFrom( copy2, &array[4] );
    for( int i=0; i<Backend::kRealVectorSize; ++i ){
          std::cout << " copy2[" << i << "] " << GetComponent( copy2, i ) << "\n";
          assert( GetComponent( copy2, i) == array[4+i] && "problem in LoadFrom");
    }
    _mm_free( array );


    // "-- TESTING MATHEMATICAL FUNCTIONS --"
    {
        std::cout << "-- TESTING ABS --\n";
        Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
        Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
        for( int i=0; i<Backend::kRealVectorSize; ++i ) { array1[i]=-i; array2[i]=i; }
        Real_v val1; LoadFrom( val1, array1 );
        Real_v val2; LoadFrom( val2, array2 );
        assert( Abs( val1 ) == val2 );
        _mm_free(array1);
        _mm_free(array2);
    }
    // tesing Sqrt
    {
      std::cout << "-- TESTING SQRT --\n";
      Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      for( int i=0; i<Backend::kRealVectorSize; ++i ) { array1[i]=(i+1)*(i+1); array2[i]=i+1; }
      Real_v val1; LoadFrom( val1, array1 );
      Real_v val2; LoadFrom( val2, array2 );
      assert( Sqrt( val1 ) == val2 );
      _mm_free(array1);
      _mm_free(array2);
    }
    /*
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
    VecCore::Backend::Vector::kVc<float>::Real_v input1(1.);
    VecCore::Backend::Vector::kVc<double>::Real_v input2(1.);

   TestBackends<DefaultVectorBackend<float> >( input1 );
   // TestBackends<DefaultVectorBackend<double> >( input2 );

   // DefaultScalarBackend<float>::Real_v sinput1(1.);
    // TestBackends<DefaultScalarBackend<float> >(sinput1);

    // test a scalar API
    // TestBackends<VecCore::Backend::Scakar::kScalar<float> >( input );

    return 0;
}




