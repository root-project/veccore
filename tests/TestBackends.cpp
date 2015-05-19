/*
 * TestBackends.cpp
 *
 *  Created on: 11.05.2015
 *      Author: swenzel
 */

// file meant to systematically test the backend api

#include "backend/Vc/Backend.h"
#include "backend/scalar/Backend.h"
#include "base/Vector3D.h"
#include <mm_malloc.h>
#include <cmath>
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
using VecCore::Backend::Vector::ATan2;
using VecCore::Backend::Scalar::ATan2;
using VecCore::Backend::Vector::Pow;
using VecCore::Backend::Scalar::Pow;
using VecCore::Backend::Vector::Cos;
using VecCore::Backend::Scalar::Cos;
using VecCore::Backend::Vector::Sin;
using VecCore::Backend::Scalar::Sin;
using VecCore::Backend::Vector::Tan;
using VecCore::Backend::Scalar::Tan;
using VecCore::Backend::Vector::Min;
using VecCore::Backend::Scalar::Min;
using VecCore::Backend::Vector::Max;
using VecCore::Backend::Scalar::Max;
using VecCore::Backend::Vector::Floor;
using VecCore::Backend::Scalar::Floor;
using VecCore::Backend::Vector::SinCos;
using VecCore::Backend::Scalar::SinCos;

using namespace VecCore;

 /**
   * Example Kernel: Propagate track along Helix on constant B=(0,0,Bz) field
   */
  template<typename Backend>
  void ConstBzFieldHelixStepperDoStepKernel(
          Vector3D<typename Backend::Real_v> const & pos,
          Vector3D<typename Backend::Real_v> const & dir,
          typename Backend::Int_v const & charge,
          typename Backend::Real_v const & momentum,
          typename Backend::Real_v const & step,
          Vector3D<typename Backend::Real_v> & pos_out,
          Vector3D<typename Backend::Real_v> & dir_out,
          typename Backend::Real_v const & Bz)
  {
      typedef typename Backend::Real_v Real_v;
      const Real_v kB2C_local(-0.299792458e-3);
      const Real_v kSmall(1.E-30);
      // could do a fast square root here
      Real_v dt = Sqrt((dir.x()*dir.x()) + (dir.y()*dir.y())) + kSmall;
      Real_v invnorm=1./dt;
      // radius has sign and determines the sense of rotation
      Real_v R = momentum*dt/((kB2C_local*Real_v(charge))*(Bz));

      Real_v cosa= dir.x()*invnorm;
      Real_v sina= dir.y()*invnorm;
      Real_v phi = step * Real_v(charge) * Bz * kB2C_local / momentum;

      Real_v cosphi;
      Real_v sinphi;
      SinCos(phi, &sinphi, &cosphi);

      pos_out.x() = pos.x() + R*(-sina - (-cosphi*sina - sinphi*cosa));
      pos_out.y() = pos.y() + R*( cosa - (-sina*sinphi + cosphi*cosa));
      pos_out.z() = pos.z() + step * dir.z();

      dir_out.x() = dir.x() * cosphi - sinphi * dir.y();
      dir_out.y() = dir.x() * sinphi + cosphi * dir.y();
      dir_out.z() = dir.z();
  }

#define _R_ __restrict__
  // a client making use of the generic kernel
  void DoStep_v(
            double const * _R_ posx, double const * _R_ posy, double const * _R_ posz,
            double const * _R_ dirx, double const * _R_ diry, double const * _R_ dirz,
            int const * _R_ charge, double const * _R_ momentum, double const * _R_ step,
            double * _R_ newposx, double * _R_ newposy, double * _R_ newposz,
            double * _R_ newdirx, double * _R_ newdiry, double * _R_ newdirz,
            int np )
   {
       // we have choice here: ( try autovectorization: )

//#pragma ivdep
//      for (int i=0;i<np;++i){
//            DoStep( posx[i], posy[i], posz[i], dirx[i], diry[i], dirz[i],
//                    charge[i], momentum[i], step[i],
//                    newposx[i], newposy[i], newposz[i],
//                    newdirx[i], newdiry[i], newdirz[i]
//                  );
//       }

       // alternative loop with Vc:
       for (int i=0;i<np;i+= DefaultVectorBackend<double>::kRealVectorSize )
       {
            // results cannot not be temporaries
           typedef typename DefaultVectorBackend<double>::Real_v Real_v;
           Vector3D<Real_v> newpos;
           Vector3D<Real_v> newdir;

           ConstBzFieldHelixStepperDoStepKernel<DefaultVectorBackend<double> >(
                   Vector3D<Real_v>( Real_v(posx[i]),
                                     Real_v(posy[i]),
                                     Real_v(posz[i]) ),
                   Vector3D<Real_v>( Real_v(dirx[i]),
                                     Real_v(diry[i]),
                                     Real_v(dirz[i]) ),
                   typename DefaultVectorBackend<double>::Int_v(charge[i]),
                   Real_v(momentum[i]),
                   Real_v(step[i]),
                   newpos, newdir, Real_v(1.0));

            // write results
            StoreTo(newpos.x(), &newposx[i] );
            StoreTo(newpos.y(), &newposy[i] );
            StoreTo(newpos.z(), &newposz[i] );
            StoreTo(newdir.x(), &newdirx[i] );
            StoreTo(newdir.y(), &newdiry[i] );
            StoreTo(newdir.z(), &newdirz[i] );
       }
       // tail part: tobedone
   }



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
    condition = copy > input;

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
           assert( GetMaskComponent( condition, i) == false && "Problem in IsEmpty");
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

    {
      std::cout << "-- TESTING ATAN2 --\n";
      Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      for( int i=0; i<Backend::kRealVectorSize; ++i ){
          array1[i]=(i+1); array2[i]=std::atan2(array1[i], array1[i] + 1); }
      Real_v val1; LoadFrom( val1, array1 );
      Real_v val2; LoadFrom( val2, array2 );
      assert( ATan2( val1, val1 + 1 ) == val2 );
      _mm_free(array1);
      _mm_free(array2);
    }

    {
      std::cout << "-- TESTING POW --\n";
      Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      for( int i=0; i<Backend::kRealVectorSize; ++i ){
          array1[i]=(i+2); array2[i]=std::pow(array1[i], 1.25*array1[i] + 1); }
      Real_v val1; LoadFrom( val1, array1 );
      Real_v val2; LoadFrom( val2, array2 );
      Real_v result = Pow( val1, Real_t(1.25)*val1 + 1 );
      for( int i=0; i<Backend::kRealVectorSize; ++i ){
         // assert on relative error
          assert( std::abs( GetComponent( result, i ) - array2[i] )/array2[i] < 1E-6 && "Problem in Pow" );
      }
      _mm_free(array1);
      _mm_free(array2);
    }
    {
       std::cout << "-- TESTING COS --\n";
       Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
       Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
       for( int i=0; i<Backend::kRealVectorSize; ++i ){
              array1[i]=1.25*(i+1); array2[i]=std::cos(array1[i]); }
       Real_v val1; LoadFrom( val1, array1 );
       Real_v result = Cos( val1 );
       for( int i=0; i<Backend::kRealVectorSize; ++i ){
             // assert on relative error
              assert( std::abs( GetComponent( result, i ) - array2[i] ) < 1E-6 && "Problem in Cos" );
       }
      _mm_free(array1);
      _mm_free(array2);
    }
    {
       std::cout << "-- TESTING SIN --\n";
       Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
       Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
       for( int i=0; i<Backend::kRealVectorSize; ++i ){
              array1[i]=1.25*(i+1); array2[i]=std::sin(array1[i]); }
       Real_v val1; LoadFrom( val1, array1 );
       Real_v result = Sin( val1 );
       for( int i=0; i<Backend::kRealVectorSize; ++i ){
             // assert on relative error
              assert( std::abs( GetComponent( result, i ) - array2[i] ) < 1E-6 && "Problem in Cos" );
       }
      _mm_free(array1);
      _mm_free(array2);
    }
    {
       std::cout << "-- TESTING TAN --\n";
       Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
       Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
       for( int i=0; i<Backend::kRealVectorSize; ++i ){
              array1[i]=1.25*(i+1); array2[i]=std::tan(array1[i]); }
       Real_v val1; LoadFrom( val1, array1 );
       Real_v result = Tan( val1 );
       for( int i=0; i<Backend::kRealVectorSize; ++i ){
             // assert on relative error
              assert( std::abs( GetComponent( result, i ) - array2[i] ) < 1E-6 && "Problem in Cos" );
       }
      _mm_free(array1);
      _mm_free(array2);
     }

    {
        std::cout << "-- TESTING FLOOR --\n";
        Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
        Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
        for( int i=0; i<Backend::kRealVectorSize; ++i ){
               array1[i]=1.25*(i+1); array2[i]=std::floor(array1[i]); }
        Real_v val1; LoadFrom( val1, array1 );
        Real_v result = Floor( val1 );
        for( int i=0; i<Backend::kRealVectorSize; ++i ){
              // assert on relative error
               assert( std::abs( GetComponent( result, i ) - array2[i] ) < 1E-6 && "Problem in Floor" );
        }
       _mm_free(array1);
       _mm_free(array2);
      }

    {
      std::cout << "-- TESTING MIN --\n";
      Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
      for( int i=0; i<Backend::kRealVectorSize; ++i ){
             array1[i]=i+1; array2[i]=-array1[i];
      }
      Real_v val1; LoadFrom( val1, array1 );
      Real_v val2; LoadFrom( val2, array2 );

      Real_v result = Min( val1, val2 );
      for( int i=0; i<Backend::kRealVectorSize; ++i ){
            // assert on relative error
             assert( GetComponent( result, i ) == array2[i]  && "Problem in Min" );
      }
     _mm_free(array1);
     _mm_free(array2);
    }
    {
        std::cout << "-- TESTING MAX --\n";
        Real_t * array1 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
        Real_t * array2 = (Real_t *) _mm_malloc( sizeof( Real_t ) * Backend::kRealVectorSize, 32 );
        for( int i=0; i<Backend::kRealVectorSize; ++i ){
               array1[i]=i+1; array2[i]=-array1[i];
        }
        Real_v val1; LoadFrom( val1, array1 );
        Real_v val2; LoadFrom( val2, array2 );

        Real_v result = Max( val1, val2 );
        for( int i=0; i<Backend::kRealVectorSize; ++i ){
              // assert on relative error
               assert( GetComponent( result, i ) == array1[i]  && "Problem in Max" );
        }
       _mm_free(array1);
       _mm_free(array2);
    }


}


int main(){
    DefaultVectorBackend<float>::Real_v input1(1.);
    DefaultVectorBackend<double>::Real_v input2(1.);

   TestBackends<DefaultVectorBackend<float> >( input1 );
   TestBackends<DefaultVectorBackend<double> >( input2 );

   DefaultScalarBackend<float>::Real_v sinput1(1.);
   TestBackends<DefaultScalarBackend<float> >(sinput1);
   DefaultScalarBackend<double>::Real_v sinput2(1.);
   TestBackends<DefaultScalarBackend<double> >(sinput2);

   // test a scalar API
   // TestBackends<VecCore::Backend::Scakar::kScalar<float> >( input );
   return 0;
}




