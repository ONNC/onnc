//===- HashIterator.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_HASH_ITERATOR_H
#define ONNC_ADT_HASH_ITERATOR_H

#include <cstddef>
#include <iterator>
#include <cassert>

namespace onnc {

/** \class ChainIterBase
 *  \brief ChaintIteratorBase follows the HashEntryTy with the same hash value.
 */
template<typename HashTable>
class ChainIterBase
{
public:
  typedef HashTable hash_table;
  typedef typename HashTable::key_type key_type;
  typedef typename HashTable::entry_type entry_type;
  typedef typename HashTable::bucket_type bucket_type;

public:
  ChainIterBase();
  ChainIterBase(HashTable* pTable, const key_type& pKey);
  ChainIterBase(const ChainIterBase& pCopy);

  ChainIterBase& assign(const ChainIterBase& pCopy);

  const bucket_type* getBucket() const;
  bucket_type*       getBucket();

  const entry_type* getEntry() const;
  entry_type*       getEntry();

  void reset();

  void advance();

  bool operator==(const ChainIterBase& pCopy) const;

  bool operator!=(const ChainIterBase& pCopy) const;

private:
  HashTable* m_pHashTable;
  unsigned int m_Index;
  unsigned int m_HashValue;
  unsigned int m_EndIndex;
};

/** \class EntryIterBase
 *  \brief EntryIterBase walks over hash table by the natural layout of the
 *  buckets
 */
template<typename HashTable>
class EntryIterBase
{
public:
  typedef HashTable hash_table;
  typedef typename HashTable::key_type key_type;
  typedef typename HashTable::entry_type entry_type;
  typedef typename HashTable::bucket_type bucket_type;

public:
  EntryIterBase();
  EntryIterBase(HashTable* pTable, unsigned int pIndex);
  EntryIterBase(const EntryIterBase& pCopy);

  EntryIterBase& assign(const EntryIterBase& pCopy);

  const bucket_type* getBucket() const;
  bucket_type* getBucket();

  const entry_type* getEntry() const;
  entry_type* getEntry();

  void reset();

  void advance();

  bool operator==(const EntryIterBase& pCopy) const;

  bool operator!=(const EntryIterBase& pCopy) const;

private:
  HashTable* m_pHashTable;
  unsigned int m_Index;
};

/** \class HashIterator
 *  \brief HashIterator provides a policy-based iterator.
 *
 *  HashTable has two kinds of iterators. One is used to traverse buckets
 *  with the same hash value; the other is used to traverse all entries of the
 *  hash table.
 *
 *  HashIterator is a template policy-based iterator, which can change its
 *  behavior by change the template argument IteratorBase. HashTable defines
 *  above two iterators by defining HashIterators with different IteratorBase.
 */
template<typename IteratorBase>
class HashIterator : public IteratorBase
{
public:
  typedef std::forward_iterator_tag  iterator_category;

  typedef size_t       size_type;
  typedef ptrdiff_t    difference_type;
  typedef IteratorBase super;

  typedef typename super::hash_table  hash_table;
  typedef typename super::key_type    key_type;
  typedef typename super::entry_type  entry_type;
  typedef typename super::bucket_type bucket_type;

  typedef const entry_type* const_pointer;
  typedef       entry_type* pointer;
  typedef const entry_type& const_reference;
  typedef       entry_type& reference;

  typedef HashIterator<IteratorBase> Self;

public:
  HashIterator()
  : IteratorBase()
  { }

  /// HashIterator - constructor for EntryIterator
  HashIterator(hash_table* pTable, unsigned int pIndex)
  : IteratorBase(pTable, pIndex) { }

  /// HashIterator - constructor for ChainIterator
  HashIterator(hash_table* pTable, const key_type& pKey, int)
  : IteratorBase(pTable, pKey) { }

  HashIterator(const HashIterator& pCopy)
  : IteratorBase(pCopy) { }

  ~HashIterator() { }

  using super::assign;

  using super::advance;

  HashIterator& operator=(const HashIterator& pCopy) {
    this->assign(pCopy);
    return *this;
  }

  const_reference operator*() const { return *(this->getEntry()); }
  reference       operator*()       { return *(this->getEntry()); }

  const_pointer   operator->() const { return this->getEntry(); }
  pointer         operator->()       { return this->getEntry(); }

  Self& operator++() {
    this->advance();
    return *this;
  }

  Self operator++(int) {
    Self tmp = *this;
    this->advance();
    return tmp;
  }
};

#include "Bits/HashIterator.tcc"

} // namespace of onnc

#endif
