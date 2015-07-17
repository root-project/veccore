#ifndef VECCORE_MAP_H
#define VECCORE_MAP_H

#include "RBTree.h"
namespace VecCore {
VECGEOM_DEVICE_FORWARD_DECLARE(template <typename Type>  class map; )
 

inline namespace VECGEOM_IMPL_NAMESPACE {


template <typename P>
struct select1st{
  VECGEOM_CUDA_HEADER_BOTH
  typename P::first_type const& operator() (P const &p) const {
     return p.first;
  }
};

template <class _Key, class _Tp, 
          class _Compare = less<_Key>>
class map;

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator==(const map<_Key,_Tp,_Compare>& __x, 
                       const map<_Key,_Tp,_Compare>& __y);

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator<(const map<_Key,_Tp,_Compare>& __x, 
                      const map<_Key,_Tp,_Compare>& __y);

template <class _Key, class _Tp, class _Compare>
class map {
public:


// typedefs:

  typedef _Key                  key_type;
  typedef _Tp                   data_type;
  typedef _Tp                   mapped_type;
  typedef std::pair< _Key, _Tp> value_type;
  typedef _Compare              key_compare;


template <class Key, class T, class Compare>
class value_compare
{  
  friend class map;
protected:
  Compare comp;
  VECGEOM_CUDA_HEADER_BOTH
  value_compare (Compare c) : comp(c) {}
public:
  typedef bool result_type;
  typedef value_type first_argument_type;
  typedef value_type second_argument_type;
  VECGEOM_CUDA_HEADER_BOTH
  bool operator() (const value_type& x, const value_type& y) const
  {
    return comp(x.first, y.first);
  }
};


private:
  typedef _Rb_tree<key_type, value_type, 
                   select1st<value_type>, key_compare> _Rep_type;
                   //std::_Select1st<value_type>, key_compare> _Rep_type;
  _Rep_type _M_t;  // red-black tree representing map
public:
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  //typedef typename _Rep_type::reverse_iterator reverse_iterator;
  //typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;


  // Constructors
  VECGEOM_CUDA_HEADER_BOTH
  map() : _M_t( _Compare()) {}
  VECGEOM_CUDA_HEADER_BOTH
  map(const value_type* __first, const value_type* __last)
    : _M_t(_Compare())
    { _M_t.insert_unique(__first, __last); }
  VECGEOM_CUDA_HEADER_BOTH
  map(const value_type* __first,
      const value_type* __last, const _Compare& __comp)
    : _M_t(__comp) { _M_t.insert_unique(__first, __last); }
  VECGEOM_CUDA_HEADER_BOTH
  map(const_iterator __first, const_iterator __last)
    : _M_t(_Compare()) 
    { _M_t.insert_unique(__first, __last); }
  VECGEOM_CUDA_HEADER_BOTH
  map(const_iterator __first, const_iterator __last, const _Compare& __comp)
    : _M_t(__comp) { _M_t.insert_unique(__first, __last); }

  VECGEOM_CUDA_HEADER_BOTH
  map(const map<_Key,_Tp,_Compare>& __x) : _M_t(__x._M_t) {}
  VECGEOM_CUDA_HEADER_BOTH
  map<_Key,_Tp,_Compare>&
  operator=(const map<_Key, _Tp, _Compare>& __x)
  {
    _M_t = __x._M_t;
    return *this; 
  }

  //key/value compare funtions
  VECGEOM_CUDA_HEADER_BOTH
  key_compare key_comp() const { return _M_t.key_comp(); }
  VECGEOM_CUDA_HEADER_BOTH
  value_compare<_Key,_Tp,_Compare> value_comp() const { return value_compare<_Key,_Tp,_Compare>(_M_t.key_comp()); }

  //iterators
  VECGEOM_CUDA_HEADER_BOTH
  iterator begin() { return _M_t.begin(); }
  VECGEOM_CUDA_HEADER_BOTH
  const_iterator begin() const { return _M_t.begin(); }
  VECGEOM_CUDA_HEADER_BOTH
  iterator end() { return _M_t.end(); }
  VECGEOM_CUDA_HEADER_BOTH 
  const_iterator end() const { return _M_t.end(); }
/*
  reverse_iterator rbegin() { return _M_t.rbegin(); }
  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
  reverse_iterator rend() { return _M_t.rend(); }
  const_reverse_iterator rend() const { return _M_t.rend(); }
*/
  VECGEOM_CUDA_HEADER_BOTH
  bool empty() const { return _M_t.empty(); }
  VECGEOM_CUDA_HEADER_BOTH
  size_type size() const { return _M_t.size(); }
  VECGEOM_CUDA_HEADER_BOTH
  size_type max_size() const { return _M_t.max_size(); }
  VECGEOM_CUDA_HEADER_BOTH
  _Tp& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    // __i->first is greater than or equivalent to __k.
    if (__i == end() || key_comp()(__k, (*__i).first))
      __i = insert(__i, value_type(__k, _Tp()));
    return (*__i).second;
  }
 //void swap(map<_Key,_Tp,_Compare,_Alloc>& __x) { _M_t.swap(__x._M_t); }

  // insert/erase
  VECGEOM_CUDA_HEADER_BOTH
  std::pair<iterator,bool> insert(const value_type& __x) 
    { return _M_t.insert_unique(__x); }
  VECGEOM_CUDA_HEADER_BOTH
  iterator insert(iterator position, const value_type& __x)
    { return _M_t.insert_unique(position, __x); }
  VECGEOM_CUDA_HEADER_BOTH
  void insert(const value_type* __first, const value_type* __last) {
    _M_t.insert_unique(__first, __last);
  }
  VECGEOM_CUDA_HEADER_BOTH
  void insert(const_iterator __first, const_iterator __last) {
    _M_t.insert_unique(__first, __last);
  }

  VECGEOM_CUDA_HEADER_BOTH
  void erase(iterator __position) { _M_t.erase(__position); }
  VECGEOM_CUDA_HEADER_BOTH
  size_type erase(const key_type& __x) { return _M_t.erase(__x); }
  VECGEOM_CUDA_HEADER_BOTH
  void erase(iterator __first, iterator __last)
    { _M_t.erase(__first, __last); }
  VECGEOM_CUDA_HEADER_BOTH
  void clear() { _M_t.clear(); }

  // map operations:
  VECGEOM_CUDA_HEADER_BOTH
  iterator find(const key_type& __x) { return _M_t.find(__x); }
  VECGEOM_CUDA_HEADER_BOTH
  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }
  VECGEOM_CUDA_HEADER_BOTH
  size_type count(const key_type& __x) const {
    return _M_t.find(__x) == _M_t.end() ? 0 : 1; 
  }
  VECGEOM_CUDA_HEADER_BOTH
  iterator lower_bound(const key_type& __x) {return _M_t.lower_bound(__x); }
  VECGEOM_CUDA_HEADER_BOTH
  const_iterator lower_bound(const key_type& __x) const {
    return _M_t.lower_bound(__x); 
  }
  VECGEOM_CUDA_HEADER_BOTH
  iterator upper_bound(const key_type& __x) {return _M_t.upper_bound(__x); }
  VECGEOM_CUDA_HEADER_BOTH
  const_iterator upper_bound(const key_type& __x) const {
    return _M_t.upper_bound(__x); 
  }
  VECGEOM_CUDA_HEADER_BOTH
  std::pair<iterator,iterator> equal_range(const key_type& __x) {
    return _M_t.equal_range(__x);
  }
  VECGEOM_CUDA_HEADER_BOTH
  std::pair<const_iterator,const_iterator> equal_range(const key_type& __x) const {
    return _M_t.equal_range(__x);
  }

};

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator==(const map<_Key,_Tp,_Compare>& __x, 
                       const map<_Key,_Tp,_Compare>& __y) {
  return __x._M_t == __y._M_t;
}

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH 
inline bool operator<(const map<_Key,_Tp,_Compare>& __x, 
                      const map<_Key,_Tp,_Compare>& __y) {
  return __x._M_t < __y._M_t;
}


template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator!=(const map<_Key,_Tp,_Compare>& __x, 
                       const map<_Key,_Tp,_Compare>& __y) {
  return !(__x == __y);
}

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator>(const map<_Key,_Tp,_Compare>& __x, 
                      const map<_Key,_Tp,_Compare>& __y) {
  return __y < __x;
}

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator<=(const map<_Key,_Tp,_Compare>& __x, 
                       const map<_Key,_Tp,_Compare>& __y) {
  return !(__y < __x);
}

template <class _Key, class _Tp, class _Compare>
VECGEOM_CUDA_HEADER_BOTH
inline bool operator>=(const map<_Key,_Tp,_Compare>& __x, 
                       const map<_Key,_Tp,_Compare>& __y) {
  return !(__x < __y);
}
/*
template <class _Key, class _Tp, class _Compare>
inline void swap(map<_Key,_Tp,_Compare>& __x, 
                 map<_Key,_Tp,_Compare>& __y) {
  __x.swap(__y);
}
*/
}
}
#endif
