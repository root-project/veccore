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

using veccore::backend::vector::GetComponent;
using veccore::backend::vector::ComponentAssign;
using veccore::backend::scalar::GetComponent;
using veccore::backend::scalar::ComponentAssign;
using veccore::backend::scalar::CondAssign;
using veccore::backend::vector::CondAssign;
using veccore::backend::scalar::GetMaskComponent;
using veccore::backend::vector::GetMaskComponent;
using veccore::backend::scalar::SetComponent;
using veccore::backend::vector::SetComponent;

using veccore::backend::vector::StoreTo;
using veccore::backend::vector::LoadFrom;
using veccore::backend::scalar::StoreTo;
using veccore::backend::scalar::LoadFrom;

using veccore::backend::vector::Any;
using veccore::backend::vector::IsFull;
using veccore::backend::vector::IsEmpty;
using veccore::backend::scalar::Any;
using veccore::backend::scalar::IsFull;
using veccore::backend::scalar::IsEmpty;
using veccore::backend::vector::Abs;
using veccore::backend::scalar::Abs;
using veccore::backend::vector::Sqrt;
using veccore::backend::scalar::Sqrt;
using veccore::backend::vector::ATan2;
using veccore::backend::scalar::ATan2;
using veccore::backend::vector::Pow;
using veccore::backend::scalar::Pow;
using veccore::backend::vector::Cos;
using veccore::backend::scalar::Cos;
using veccore::backend::vector::Sin;
using veccore::backend::scalar::Sin;
using veccore::backend::vector::Tan;
using veccore::backend::scalar::Tan;
using veccore::backend::vector::Min;
using veccore::backend::scalar::Min;
using veccore::backend::vector::Max;
using veccore::backend::scalar::Max;
using veccore::backend::vector::Floor;
using veccore::backend::scalar::Floor;
using veccore::backend::vector::SinCos;
using veccore::backend::scalar::SinCos;

using namespace veccore;

//
// Example Kernel: Propagate track along Helix on constant B=(0,0,Bz) field
//
template<typename Backend>
void ConstBzFieldHelixStepperDoStepKernel(
      Vector3D<typename Backend::Real_v> const & pos,
      Vector3D<typename Backend::Real_v> const & dir,
      // mixing of vector types ( int - floating point ) is VERY Dangerous
      // and has to be avoided
      // typename Backend::Int_v const & charge,
      // better to give charge directly as Real_v
      typename Backend::Real_v const & charge,
      typename Backend::Real_v const & momentum,
      typename Backend::Real_v const & step,
      Vector3D<typename Backend::Real_v> & pos_out,
      Vector3D<typename Backend::Real_v> & dir_out,
      typename Backend::Real_v const & Bz) {
 // std::cerr << "input posx " << pos.x() << "\n";
 // std::cerr << "input dirx " << dir.x() << "\n";

  typedef typename Backend::Real_v Real_v;
  const Real_v kB2C_local(-0.299792458e-3);
  const Real_v kSmall(1.E-30);
  // could do a fast square root here
  Real_v dt = Sqrt((dir.x()*dir.x()) + (dir.y()*dir.y())) + kSmall;
  Real_v invnorm=Real_v(1.)/dt;
  // radius has sign and determines the sense of rotation
 // std::cerr << "charge " << charge << "\n";

  Real_v R = momentum*dt/((kB2C_local*charge)*(Bz));

  Real_v cosa= dir.x()*invnorm;
  Real_v sina= dir.y()*invnorm;
  Real_v phi = step * charge * Bz * kB2C_local / momentum;

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

// same kernel using boxed types
template<typename Backend>
void ConstBzFieldHelixStepperDoStepKernel2(
      Vector3D<typename Backend::BoxedReal_v> const & pos,
      Vector3D<typename Backend::BoxedReal_v> const & dir,
      // mixing of vector types ( int - floating point ) is VERY Dangerous
      // and has to be avoided
      // typename Backend::Int_v const & charge,
      // better to give charge directly as Real_v
      typename Backend::BoxedReal_v const & charge,
      typename Backend::BoxedReal_v const & momentum,
      typename Backend::BoxedReal_v const & step,
      Vector3D<typename Backend::BoxedReal_v> & pos_out,
      Vector3D<typename Backend::BoxedReal_v> & dir_out,
      typename Backend::BoxedReal_v const & Bz) {
 // std::cerr << "input posx " << pos.x() << "\n";
 // std::cerr << "input dirx " << dir.x() << "\n";

  typedef typename Backend::Real_v Real_v;
  const Real_v kB2C_local(-0.299792458e-3);
  const Real_v kSmall(1.E-30);
  // could do a fast square root here
  Real_v dt = Sqrt((dir.x()*dir.x()) + (dir.y()*dir.y())) + kSmall;
  Real_v invnorm=Real_v(1.)/dt;
  // radius has sign and determines the sense of rotation
//  std::cerr << "charge " << charge << "\n";

  Real_v R = momentum*dt/((kB2C_local*charge)*(Bz));

  Real_v cosa= dir.x()*invnorm;
  Real_v sina= dir.y()*invnorm;
  Real_v phi = step * charge * Bz * kB2C_local / momentum;

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
template <typename Backend, typename FloatType>
void DoStep_v(
        FloatType const * _R_ posx, FloatType const * _R_ posy, FloatType const * _R_ posz,
        FloatType const * _R_ dirx, FloatType const * _R_ diry, FloatType const * _R_ dirz,
        int const * _R_ charge, FloatType const * _R_ momentum, FloatType const * _R_ step,
        FloatType * _R_ newposx, FloatType * _R_ newposy, FloatType * _R_ newposz,
        FloatType * _R_ newdirx, FloatType * _R_ newdiry, FloatType * _R_ newdirz,
        int np )
{
  // do a study if this loop autovectorizes if DefaultVectorBackend = kScalar
  for (int i=0;i<np;i+= Backend::kRealVectorSize )
  {
    typedef typename Backend::Real_v Real_v;
    typedef typename Backend::Real_t Real_t;
    // output values
    Vector3D<Real_v> newpos;
    Vector3D<Real_v> newdir;

    // since charge is given as int --> need to make a Real_v value
    Real_v castedcharge;
    for( int j=0; j< Backend::kRealVectorSize; ++j ){
      int tmp = charge[ i + j ];
      ComponentAssign( j, Real_t(1.)*tmp, castedcharge);
    }

       ConstBzFieldHelixStepperDoStepKernel<Backend>(
       Vector3D<Real_v>( Real_v( Backend::GrabVectorStartAt( posx[i] ) ),
                         Real_v( Backend::GrabVectorStartAt( posy[i] ) ),
                         Real_v( Backend::GrabVectorStartAt( posz[i] ) ) ),
       Vector3D<Real_v>( Real_v( Backend::GrabVectorStartAt( dirx[i] ) ),
                         Real_v( Backend::GrabVectorStartAt( diry[i] ) ),
                         Real_v( Backend::GrabVectorStartAt( dirz[i] ) ) ),
       castedcharge,
       Real_v( Backend::GrabVectorStartAt( momentum[i] ) ),
       Real_v( Backend::GrabVectorStartAt( step[i] ) ),
       newpos, newdir, Real_v(1000.0));

    // write results to output arrays
    StoreTo(newpos.x(), &newposx[i] );
    StoreTo(newpos.y(), &newposy[i] );
    StoreTo(newpos.z(), &newposz[i] );
    StoreTo(newdir.x(), &newdirx[i] );
    StoreTo(newdir.y(), &newdiry[i] );
    StoreTo(newdir.z(), &newdirz[i] );
  }
  // tail part/treatment should follow here
}


// demonstration of new unified way to instantiate Backend::Real_v types and to store them back to an
// array
#define _R_ __restrict__
template <typename Backend, typename FloatType>
void DoStep_WithOperators_v(
        FloatType const * _R_ posx, FloatType const * _R_ posy, FloatType const * _R_ posz,
        FloatType const * _R_ dirx, FloatType const * _R_ diry, FloatType const * _R_ dirz,
        int const * _R_ charge, FloatType const * _R_ momentum, FloatType const * _R_ step,
        FloatType * _R_ newposx, FloatType * _R_ newposy, FloatType * _R_ newposz,
        FloatType * _R_ newdirx, FloatType * _R_ newdiry, FloatType * _R_ newdirz,
        int np )
{
  // do a study if this loop autovectorizes if DefaultVectorBackend = kScalar
  for (int i=0;i<np;i+= Backend::kRealVectorSize )
  {
    typedef typename Backend::BoxedReal_v Real_v;
    typedef typename Backend::Real_t Real_t;

    // since charge is given as int --> need to make a Real_v value
    Real_v castedcharge;
    for( int j=0; j< Backend::kRealVectorSize; ++j )
      castedcharge[j]=Real_t(charge[i+j]);

    // output values:
    Vector3D<Real_v> newpos;
    Vector3D<Real_v> newdir;

    // call kernel with init of parameters from array
    ConstBzFieldHelixStepperDoStepKernel2<Backend >(
       Vector3D<Real_v>( Real_v(&posx[i]), Real_v(&posy[i]), Real_v(&posz[i]) ),
       Vector3D<Real_v>( Real_v(&dirx[i]), Real_v(&diry[i]), Real_v(&dirz[i]) ),
       castedcharge,
       Real_v( &momentum[i] ), Real_v( &step[i] ), newpos, newdir, Real_v(1000.0) );

    // write results to output arrays
    newpos.x().store( &newposx[i] );
    newpos.y().store( &newposy[i] );
    newpos.z().store( &newposz[i] );
    newdir.x().store( &newdirx[i] );
    newdir.y().store( &newdiry[i] );
    newdir.z().store( &newdirz[i] );
  }
  // tail part/treatment should follow here
}



// a kernel testing new operator API provided by pod-wrapper class
template <typename Backend>
__attribute__((noinline))
typename Backend::Real_v TestOperatorApproach( typename Backend::BoxedReal_v const a,
                                               typename Backend::BoxedReal_v const b,
                                               Vector3D<typename Backend::BoxedReal_v> const v,
                                               Vector3D<typename Backend::BoxedReal_v> & v2){
  typename Backend::BoxedReal_v tmp = a;
  // with operator notation as the replacement of MaskedAssign + [] accessor to vector lane
  tmp( b > a ) -= b + 2.*a[0] + v.x();
  v2.x()( a > 1. ) = 1./a;
  return tmp;
}


// the traditional alternative kernel is
template <typename Backend>
__attribute__((noinline))
typename Backend::Real_v TestTraditionalApproach( typename Backend::Real_v const a,
                                                  typename Backend::Real_v const b,
                                                  Vector3D<typename Backend::Real_v> const v,
                                                  Vector3D<typename Backend::Real_v> & v2
                                                ){
  typename Backend::Real_v tmp = a;
  // using functions like GetComponent and MaskedAssign
  typename Backend::Real_v newvalue = tmp - (b + 2. * GetComponent(a,0) + v.x());
  MaskedAssign( b > a, newvalue, &tmp);
  // modify vector v2
  typename Backend::Real_v & tmp2 = v2.x();
  MaskedAssign( a > 1., 1./a, &tmp2 );
  return tmp;
}

// these functions are here instantiate the template kernels;
// to inspect and compare the assembly code
// to test the correctness
__attribute__((noinline))
void TestOperatorApproach(){
  BoxedPrimitive<double > a(2.);
  BoxedPrimitive<double > b(3.);
  Vector3D<BoxedPrimitive<double> > v1(1.,0.,0.);
  Vector3D<BoxedPrimitive<double> > v2(11.,0.,0.);
  auto r = TestOperatorApproach<DefaultScalarBackend<double> >( a, b, v1,  v2 );
  assert( r == -6. );
  assert( v1.x() == 1. );
  assert( v2.x() == 1./2 );
}

__attribute__((noinline))
void TestTraditionalApproach(){
  double a(2.);
  double b(3.);
  Vector3D<double> v1(1.,0.,0.);
  Vector3D<double> v2(11.,0.,0.);
  auto r = TestTraditionalApproach<DefaultScalarBackend<double> >( a, b, v1, v2 );
  assert( r == -6. );
  assert( v1.x() == 1. );
  assert( v2.x() == 1./2 );
}


// another real life example (from box) to compare traditional style coding with
// operator style coding
template <class Backend>
__attribute__((always_inline))
void BoxDistanceToOutKernelTrad( Vector3D<double  > const &dimensions,
                                 Vector3D<typename Backend::Real_v> const &point,
                                 Vector3D<typename Backend::Real_v> const &direction,
                                 typename Backend::Real_v const &stepMax,
                                 typename Backend::Real_v &distance) {

  typedef typename Backend::Real_v Real_v;
  distance = kInfinity;

  Vector3D<Real_v> inverseDirection(
        1. / (direction[0] + kMinimum),
        1. / (direction[1] + kMinimum),
        1. / (direction[2] + kMinimum));

  Vector3D<Real_v> distances(
        (dimensions[0] - point[0]) * inverseDirection[0],
        (dimensions[1] - point[1]) * inverseDirection[1],
        (dimensions[2] - point[2]) * inverseDirection[2] );

  MaskedAssign(direction[0] < 0,
               (-dimensions[0] - point[0]) * inverseDirection[0],
               &distances[0]);
  MaskedAssign(direction[1] < 0,
               (-dimensions[1] - point[1]) * inverseDirection[1],
               &distances[1]);
  MaskedAssign(direction[2] < 0,
               (-dimensions[2] - point[2]) * inverseDirection[2],
               &distances[2]);

  distance = distances[0];
  MaskedAssign(distances[1] < distance, distances[1], &distance);
  MaskedAssign(distances[2] < distance, distances[2], &distance);
}

// the same box kernel using operator notation ( plus some more cleanup )
template <class Backend>
__attribute__((always_inline))
void BoxDistanceToOutKernelOperator(
   Vector3D<BoxedPrimitive< double > > const &dimensions,
   Vector3D<typename Backend::BoxedReal_v> const &point,
   Vector3D<typename Backend::BoxedReal_v> const &direction,
   typename Backend::BoxedReal_v const &stepMax,
   typename Backend::BoxedReal_v &distance) {

   typedef typename Backend::BoxedReal_v Real_v;

   // TODO: there should be a sign in front of kMinimum
   // TODO: Is kMinimum a good value??
   Vector3D<Real_v> inverseDirection( 1./(direction + kMinimum) );
   Vector3D<Real_v> distances( (dimensions - point)*inverseDirection );

   // TODO: we could offer a Vector3D interface for this
   distances[0]( direction[0] < 0.) = -distances[0];
   distances[1]( direction[1] < 0.) = -distances[1];
   distances[2]( direction[2] < 0.) = -distances[2];

   distance = distances[0];
   distance( distances[1] < distance ) = distances[1];
   distance( distances[2] < distance ) = distances[2];
}

// functions to instantiate the box kernels
__attribute__((noinline))
double TestTradBox( Vector3D<double> const & s,
                    Vector3D<double> const & p,
                    Vector3D<double> const & d, double stepmax ) {
  double dist;
  BoxDistanceToOutKernelTrad<DefaultScalarBackend<double> >( s, p, d, stepmax, dist);
  return dist;
}

__attribute__((noinline))
double TestOperatorBox( Vector3D<double> const & s,
                        Vector3D<double> const & p,
                        Vector3D<double> const & d, double stepmax ) {
  typedef DefaultScalarBackend<double>::BoxedReal_v Real_v;
  Real_v dist;
  BoxDistanceToOutKernelOperator<DefaultScalarBackend<double> >(
                Vector3D<Real_v>(s),
                Vector3D<Real_v>(p),
                Vector3D<Real_v>(d),
                Real_v(stepmax), dist);
  return dist;
}

// just a helper function
template <typename Type>
Type * Alloc(size_t size){
  return (Type *) _mm_malloc( sizeof(Type)* size, 32);
}

template <typename Backend, typename FloatType, bool usetraditional>
__attribute__((noinline))
void TestDoStep(){
  int np = 8;
  FloatType * posx = Alloc<FloatType>(np);
  FloatType * posy = Alloc<FloatType>(np);
  FloatType * posz = Alloc<FloatType>(np);
  FloatType * dirx = Alloc<FloatType>(np);
  FloatType * diry = Alloc<FloatType>(np);
  FloatType * dirz = Alloc<FloatType>(np);
  int    * charge = Alloc<int>(np);
  FloatType * momentum = Alloc<FloatType>(np);
  FloatType * step = Alloc<FloatType>(np);
  FloatType * newposx = Alloc<FloatType>(np);
  FloatType * newposy = Alloc<FloatType>(np);
  FloatType * newposz = Alloc<FloatType>(np);
  FloatType * newdirx = Alloc<FloatType>(np);
  FloatType * newdiry = Alloc<FloatType>(np);
  FloatType * newdirz = Alloc<FloatType>(np);

  //init some data
  for(int i=0 ; i<np ; ++i){
    posx[i] = i+0.1;
    posy[i] = 2*i;
    posz[i] = -i;
    dirx[i] = i % 3 == 0 ? 1. : 0.;
    diry[i] = (i + 1) % 3 == 0. ? 1. : 0.;
    dirz[i] = (i + 2) % 3 == 0. ? 1. : 0.;
    charge[i] = (i%2)? -1*i: 1*i;
    momentum[i] = i;
    step[i] = 1.1*i;
  }
  // process data
  if( usetraditional ){
      DoStep_v<Backend>( posx, posy, posz, dirx, diry, dirz, charge, momentum, step, newposx, newposy,
                   newposz, newdirx, newdiry, newdirz, np );
  }
  else{
      DoStep_WithOperators_v<Backend>( posx, posy, posz, dirx, diry, dirz, charge, momentum, step, newposx, newposy,
                   newposz, newdirx, newdiry, newdirz, np );
  }
  for(int i=0; i<np; ++i ){
    std::cerr << i << " old pos << " << posx[i] << ", " << posy[i] << " , " << posz[i] << "\n";
    std::cerr << i << " old dir << " << dirx[i] << ", " << diry[i] << " , " << dirz[i] << "\n";
    std::cerr << i << " new pos << " << newposx[i] << ", " << newposy[i] << " , " << newposz[i] << "\n";
    std::cerr << i << " new dir << " << newdirx[i] << ", " << newdiry[i] << " , " << newdirz[i] << "\n";
  }

}


// test function checking basic (traditional) API of Backends
// ( hence no operators )
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
  // TODO!!

  // check Loads and Stores from Array
  std::cout << "-- TESTING LOAD/STORE ABSTRACTION --\n";
  {
    //
    // initialize an array first
    Real_t * array = (Real_t *) _mm_malloc( sizeof( Real_t ) * 2* Backend::kRealVectorSize, 32 );
    for( int i=0;i<2*Backend::kRealVectorSize;++i ) array[i]=i;
    StoreTo( copy, array );
    // assert that array really contains the stuff from copy
    for( int i=0; i<Backend::kRealVectorSize; ++i )
      assert( GetComponent( copy, i ) == array[i] && "problem in StoreTo");

    Real_v copy2;
    LoadFrom( copy2, &array[ Backend::kRealVectorSize ] );
    for( int i=0; i<Backend::kRealVectorSize; ++i ){
      std::cout << " copy2[" << i << "] " << GetComponent( copy2, i ) << "\n";
      assert( GetComponent( copy2, i) == array[Backend::kRealVectorSize + i] && "problem in LoadFrom");
    }
    _mm_free( array );
  }

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
          assert( std::abs( GetComponent( result, i ) - array2[i] )/array2[i] < 1E-3 && "Problem in Pow" );
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

   // test do step in all variations: scalar x vector x ( traditional or wrapped pods)
   TestDoStep<DefaultScalarBackend<double>, double, true>();
   TestDoStep<DefaultScalarBackend<double>, double, false>();
   TestDoStep<DefaultScalarBackend<float>, float, true>();
   TestDoStep<DefaultScalarBackend<float>, float, false>();
   TestDoStep<DefaultVectorBackend<double>, double, true>();
   TestDoStep<DefaultVectorBackend<double>, double, false>();
   TestDoStep<DefaultVectorBackend<float>, float, true>();
   TestDoStep<DefaultVectorBackend<float>, float, false>();

   // test operator approach
   TestOperatorApproach();
   TestTraditionalApproach();

   // test the operator box kernel
   std::cerr << TestOperatorBox( Vector3D<double>(1.,1.,1.),
               Vector3D<double>(0.5,0.5,0.5),
               Vector3D<double>(1.0,0,0), 10. ) << "\n";
   // need to provide some assert here

   return 0;
}




