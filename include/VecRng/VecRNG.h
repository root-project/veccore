#ifndef VECRNG_VECRNG_H
#define VECRNG_VECRNG_H 1

/**
 * VecRNG: The base class of SIMD/SIMT random number generators
 *
 * Requirements :
 * 1) DerivedT  : A pseudo-random number generator with multiple streams
 * 2) BackendT  : Scalar, Vector, Cuda
 * 3) RandomT   : A templated struct of DerivedT states with BackendT 
 */

#include "RngDefs.h"

namespace vecRng {
inline namespace VECRNG_IMPL_NAMESPACE {

template <typename DerivedT, typename BackendT, typename RandomT>
class VecRNG {

protected:
  // Use *this to access data members in the derived class
  RandomT *fState;

public:

  VECCORE_ATT_HOST_DEVICE
#ifndef VECCORE_CUDA
  VecRNG() { fState = (RandomT *)AlignedAlloc(VECCORE_SIMD_ALIGN, sizeof(RandomT)); }
#else
  VecRNG() { fState = new RandomT; }
#endif

  // Dummy Constructor for SIMT
  VECCORE_ATT_HOST_DEVICE
  VecRNG(RandomT *devState) {}

  VECCORE_ATT_HOST_DEVICE
#ifndef VECCORE_CUDA
  ~VecRNG() { AlignedFree(fState); }
#else
  ~VecRNG() { delete fState; }
#endif

  VECCORE_ATT_HOST_DEVICE
  VecRNG(const VecRNG &rng) = default;

  // Static interfaces (Required methods)
  
  // Initialization for SIMD
  VECCORE_ATT_HOST 
  void Initialize() 
  { static_cast<DerivedT *>(this)->template Initialize<BackendT>(); }

  // Initialization for SIMT
  VECCORE_ATT_HOST 
  void Initialize(RandomT *states, int blocks, int threads)
  { static_cast<DerivedT *>(this)->template Initialize<BackendT>(states,blocks,threads); }

  // Return ReturnTypeBackendT::Double_v of random numbers in [0,1)
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Uniform() 
  { return static_cast<DerivedT *>(this)->template Kernel<ReturnTypeBackendT>(*this->fState); }

  // Generate random numbers based on a given state
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Uniform(RandomT *state) 
  { return static_cast<DerivedT *>(this)->template Kernel<ReturnTypeBackendT>(*state); }

  // Auxiliary methods 

  VECCORE_ATT_HOST_DEVICE 
  void SetState(RandomT *state) { fState = state; }

  VECCORE_ATT_HOST_DEVICE 
  RandomT* GetState() const { return fState; }

  VECCORE_ATT_HOST 
  void PrintState() const { static_cast<DerivedT *>(this)->PrintState(); }

  //Common methods

  // Returns an array of random numbers of the type ReturnTypeBackendT::Double_v
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  void Array(const size_t nsize, typename ReturnTypeBackendT::Double_v *array);

  // Flat distribution in [min,max)
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Flat(typename ReturnTypeBackendT::Double_v min, 
                                             typename ReturnTypeBackendT::Double_v max) 
  { return min+(max-min)*static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>(); }

  // Flat distribution in [min,max] with a state
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Flat(RandomT *state,
                                             typename ReturnTypeBackendT::Double_v min, 
                                             typename ReturnTypeBackendT::Double_v max) 
  { return min+(max-min)*static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>(state); }

  // Exponential deviates: exp(-x/tau)
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Exp(typename ReturnTypeBackendT::Double_v tau);

  // Exponential deviates with a state
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Exp(RandomT *state,
                                            typename ReturnTypeBackendT::Double_v tau);

  // Gaussin deviates: 1/(2*pi*sigma^2)*exp[-(x-mean)^2/sigma^2]
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Gauss(typename ReturnTypeBackendT::Double_v mean, 
                                              typename ReturnTypeBackendT::Double_v sigma);

  // Gaussin deviates with a state
  template <typename ReturnTypeBackendT>
  VECCORE_ATT_HOST_DEVICE 
  typename ReturnTypeBackendT::Double_v Gauss(RandomT *state,
                                              typename ReturnTypeBackendT::Double_v mean, 
                                              typename ReturnTypeBackendT::Double_v sigma);

  // @syj: add methods to generate other random distributions
  // (Binomial, Chi-Square, Gamma, Poisson, Landau and etc)

};

// Implementation

// Common Methods

// Returns an array of random numbers of ReturnTypeBackendT::Double_v
template <typename DerivedT, typename BackendT, typename RandomT>
template <typename ReturnTypeBackendT>
VECCORE_ATT_HOST_DEVICE void
VecRNG<DerivedT, BackendT, RandomT>::Array(const size_t nsize, typename ReturnTypeBackendT::Double_v *array)
{
  using Double_v = typename ReturnTypeBackendT::Double_v;
  for (size_t i = 0; i < nsize ; ++i) {
    Double_v u01 = static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>();    
    array[i] = u01;
  }
}
 
// Exponential deviates: exp(-x/tau)
template <typename DerivedT, typename BackendT, typename RandomT>
template <typename ReturnTypeBackendT>
VECCORE_ATT_HOST_DEVICE typename ReturnTypeBackendT::Double_v
VecRNG<DerivedT, BackendT, RandomT>::Exp(typename ReturnTypeBackendT::Double_v tau)
{
  using Double_v = typename ReturnTypeBackendT::Double_v;

  Double_v u01 = static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>();
  //@syj: check for zero 
  return -tau*math::Log(u01);
}

// Exponential deviates with a state
template <typename DerivedT, typename BackendT, typename RandomT>
template <typename ReturnTypeBackendT>
VECCORE_ATT_HOST_DEVICE typename ReturnTypeBackendT::Double_v
VecRNG<DerivedT, BackendT, RandomT>::Exp(RandomT *state, typename ReturnTypeBackendT::Double_v tau)
{
  // Exp with a state
  using Double_v = typename ReturnTypeBackendT::Double_v;

  Double_v u01 = static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>(state);
  return -tau*math::Log(u01);
}

// Gaussian deviates with a state
template <typename DerivedT, typename BackendT, typename RandomT>
template <typename ReturnTypeBackendT>
VECCORE_ATT_HOST_DEVICE typename ReturnTypeBackendT::Double_v
VecRNG<DerivedT, BackendT, RandomT>::Gauss(RandomT *state, typename ReturnTypeBackendT::Double_v mean, 
                                           typename ReturnTypeBackendT::Double_v sigma)
{
  // Gauss with a state
  using Double_v = typename ReturnTypeBackendT::Double_v;

  Double_v u1= static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>(state);
  Double_v u2= static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>(state)*(2.0*M_PI);
  Double_v normal =  math::Sqrt(-2.0*math::Log(u1))*math::Cos(u2);
  return mean+sigma*normal;
}

// Gaussian deviates with (mean, sigma):  1/(2*pi*sigma^2)*exp[-(x-mean)^2/sigma^2]
template <typename DerivedT, typename BackendT, typename RandomT>
template <typename ReturnTypeBackendT>
VECCORE_ATT_HOST_DEVICE typename ReturnTypeBackendT::Double_v
VecRNG<DerivedT, BackendT, RandomT>::Gauss(typename ReturnTypeBackendT::Double_v mean, 
                                           typename ReturnTypeBackendT::Double_v sigma)
{
  // Using Box/Muller - use just one
  // normal1 = sqrt(-2*log(u1))*cos(2*pi*u2)
  // normal2 = sqrt(-2*log(u1))*sin(2*pi*u2)

  using Double_v = typename ReturnTypeBackendT::Double_v;

  Double_v u1= static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>();
  Double_v u2= static_cast<DerivedT *>(this)-> template Uniform<ReturnTypeBackendT>()*(2.0*M_PI);
  Double_v normal =  math::Sqrt(-2.0*math::Log(u1))*math::Cos(u2);
  return mean+sigma*normal;
}
 
} // end namespace impl
} // end namespace vecRng

#endif
