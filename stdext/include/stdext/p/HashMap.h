/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/p/HashMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-9-6 10:08:57
// 
// $Id: HashMap.h 2009-9-6 10:08:57 xushiwei Exp $
// -----------------------------------------------------------------------*/

/*
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#ifndef STDEXT_P_HASHMAP_H
#define STDEXT_P_HASHMAP_H

#ifndef STDEXT_P_HASHTABLE_H
#include "HashTable.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

template <class _Pair>
struct Select1st_ : public unary_function<_Pair, typename _Pair::first_type> {
	const typename _Pair::first_type& operator()(const _Pair& x) const {
		return x.first;
	}
};

// -------------------------------------------------------------------------
// class PHashMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class PoolT = ScopedPool>
class PHashMap
{
private:
  typedef typename HashCompT::hasher _HashFcn;
  typedef typename HashCompT::key_equal _EqualKey;
  typedef PHashtable<std::pair<const KeyT, DataT>, KeyT, _HashFcn,
                    Select1st_<std::pair<const KeyT, DataT> >, _EqualKey, PoolT> _Ht;
  _Ht m_ht;

public:
  typedef typename _Ht::key_type key_type;
  typedef DataT data_type;
  typedef DataT mapped_type;
  typedef typename _Ht::value_type value_type;
  typedef typename _Ht::hasher hasher;
  typedef typename _Ht::key_equal key_equal;
  
  typedef typename _Ht::size_type size_type;
  typedef typename _Ht::difference_type difference_type;
  typedef typename _Ht::pointer pointer;
  typedef typename _Ht::const_pointer const_pointer;
  typedef typename _Ht::reference reference;
  typedef typename _Ht::const_reference const_reference;

  typedef typename _Ht::iterator iterator;
  typedef typename _Ht::const_iterator const_iterator;

  hasher hash_funct() const { return m_ht.hash_funct(); }
  key_equal key_eq() const { return m_ht.key_eq(); }

public:
  explicit PHashMap(PoolT& a) : m_ht(a) {}

  PHashMap(PoolT& a, size_type n)
    : m_ht(a, n) {}

  template <class _InputIterator>
  PHashMap(PoolT& a, _InputIterator f, _InputIterator l)
	: m_ht(a)
    { m_ht.insert_unique(f, l); }

  template <class _InputIterator>
  PHashMap(PoolT& a, _InputIterator f, _InputIterator l, size_type n)
	: m_ht(a, n)
	{ m_ht.insert_unique(f, l); }

public:
  size_type size() const { return m_ht.size(); }
  size_type max_size() const { return m_ht.max_size(); }
  bool empty() const { return m_ht.empty(); }
  void swap(PHashMap& hs) { m_ht.swap(hs.m_ht); }
  void copy(const PHashMap& from) {
	  m_ht.copy(from.m_ht);
  }

  bool operator==(const PHashMap& rhs) const { return m_ht == rhs.m_ht; }
  bool operator!=(const PHashMap& rhs) const { return m_ht != rhs.m_ht; }

  iterator begin() { return m_ht.begin(); }
  iterator end() { return m_ht.end(); }
  const_iterator begin() const { return m_ht.begin(); }
  const_iterator end() const { return m_ht.end(); }

public:
  std::pair<iterator,bool> insert(const value_type& obj)
    { return m_ht.insert_unique(obj); }
  
  template <class _InputIterator>
  void insert(_InputIterator f, _InputIterator l)
    { m_ht.insert_unique(f,l); }

  std::pair<iterator,bool> insert_noresize(const value_type& obj)
    { return m_ht.insert_unique_noresize(obj); }    

  iterator find(const key_type& key) { return m_ht.find(key); }
  const_iterator find(const key_type& key) const 
    { return m_ht.find(key); }

  DataT& operator[](const key_type& key) {
    return m_ht.find_or_insert(value_type(key, DataT())).second;
  }

  size_type count(const key_type& key) const { return m_ht.count(key); }
  
  std::pair<iterator, iterator> equal_range(const key_type& key)
    { return m_ht.equal_range(key); }
  std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    { return m_ht.equal_range(key); }

  size_type erase(const key_type& key) {return m_ht.erase(key); }
  void erase(iterator it) { m_ht.erase(it); }
  void erase(iterator f, iterator l) { m_ht.erase(f, l); }
  void clear() { m_ht.clear(); }

  void resize(size_type hint) { m_ht.resize(hint); }
  size_type bucket_count() const { return m_ht.bucket_count(); }
  size_type elems_in_bucket(size_type n) const
    { return m_ht.elems_in_bucket(n); }

  static size_type node_size() {
	  return _Ht::node_size();
  }
};

// -------------------------------------------------------------------------
// class PHashMultiMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class PoolT = ScopedPool>
class PHashMultiMap
{
private:
  typedef typename HashCompT::hasher _HashFcn;
  typedef typename HashCompT::key_equal _EqualKey;
  typedef PHashtable<std::pair<const KeyT, DataT>, KeyT, _HashFcn,
                    Select1st_<std::pair<const KeyT, DataT> >, _EqualKey, PoolT> _Ht;
  _Ht m_ht;

public:
  typedef typename _Ht::key_type key_type;
  typedef DataT data_type;
  typedef DataT mapped_type;
  typedef typename _Ht::value_type value_type;
  typedef typename _Ht::hasher hasher;
  typedef typename _Ht::key_equal key_equal;

  typedef typename _Ht::size_type size_type;
  typedef typename _Ht::difference_type difference_type;
  typedef typename _Ht::pointer pointer;
  typedef typename _Ht::const_pointer const_pointer;
  typedef typename _Ht::reference reference;
  typedef typename _Ht::const_reference const_reference;

  typedef typename _Ht::iterator iterator;
  typedef typename _Ht::const_iterator const_iterator;

  hasher hash_funct() const { return m_ht.hash_funct(); }
  key_equal key_eq() const { return m_ht.key_eq(); }

public:
  explicit PHashMultiMap(PoolT& a) : m_ht(a) {}

  PHashMultiMap(PoolT& a, size_type n)
    : m_ht(a, n) {}

  template <class _InputIterator>
  PHashMultiMap(PoolT& a, _InputIterator f, _InputIterator l)
    : m_ht(a)
    { m_ht.insert_equal(f, l); }

  template <class _InputIterator>
  PHashMultiMap(PoolT& a, _InputIterator f, _InputIterator l, size_type n)
    : m_ht(a, n)
    { m_ht.insert_equal(f, l); }

public:
  size_type size() const { return m_ht.size(); }
  size_type max_size() const { return m_ht.max_size(); }
  bool empty() const { return m_ht.empty(); }
  void swap(PHashMultiMap& hs) { m_ht.swap(hs.m_ht); }
  void copy(const PHashMultiMap& from) {
	  m_ht.copy(from.m_ht);
  }

  bool operator==(const PHashMultiMap& rhs) const { return m_ht == rhs.m_ht; }
  bool operator!=(const PHashMultiMap& rhs) const { return m_ht != rhs.m_ht; }

  iterator begin() { return m_ht.begin(); }
  iterator end() { return m_ht.end(); }
  const_iterator begin() const { return m_ht.begin(); }
  const_iterator end() const { return m_ht.end(); }

public:
  iterator insert(const value_type& obj) 
    { return m_ht.insert_equal(obj); }

  template <class _InputIterator>
  void insert(_InputIterator f, _InputIterator l) 
    { m_ht.insert_equal(f,l); }

  iterator insert_noresize(const value_type& obj)
    { return m_ht.insert_equal_noresize(obj); }    

  iterator find(const key_type& key) { return m_ht.find(key); }
  const_iterator find(const key_type& key) const 
    { return m_ht.find(key); }

  size_type count(const key_type& key) const { return m_ht.count(key); }
  
  std::pair<iterator, iterator> equal_range(const key_type& key)
    { return m_ht.equal_range(key); }
  std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    { return m_ht.equal_range(key); }

  size_type erase(const key_type& key) {return m_ht.erase(key); }
  void erase(iterator it) { m_ht.erase(it); }
  void erase(iterator f, iterator l) { m_ht.erase(f, l); }
  void clear() { m_ht.clear(); }

public:
  void resize(size_type hint) { m_ht.resize(hint); }
  size_type bucket_count() const { return m_ht.bucket_count(); }
  size_type elems_in_bucket(size_type n) const
    { return m_ht.elems_in_bucket(n); }

  static size_type node_size() {
	  return _Ht::node_size();
  }
};

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_P_HASHMAP_H */

// Local Variables:
// mode:C++
// End:
