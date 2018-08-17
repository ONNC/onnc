//===- HashBase.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_BITS_HASHBASE_H
#define ONNC_ADT_BITS_HASHBASE_H
#include <onnc/Support/DataTypes.h>
#include <cstdlib>
#include <cstring>

namespace onnc {
namespace internal {

template<typename Key, typename Value>
struct Entry
{
public:
  typedef Key   key_type;
  typedef Value value_type;

public:
  Entry(const key_type& pKey) : m_Key(pKey) { }

  const Key& key() const { return m_Key; }
  Key&       key()       { return m_Key; }

  const Value& value() const { return m_Value; }
  Value&       value()       { return m_Value; }

  void setValue(const Value& pValue) { m_Value = pValue; }

protected:
  Key m_Key;
  Value m_Value;
};

/** \class TableImpl
 *
 *  TableImpl uses open-addressing, linear probing hash table.
 *  linear probing hash table obviously has high performance when the
 *  load factor is less than 0.7.
 *  The drawback is that the number of the stored items can not be more
 *  than the size of the hash table.
 *
 *  We tries to merge every things in the same HashEntry. It can
 *  keep every thing in the same cache line and improve the locality
 *  efficiently. TableImpl provides a template argument to change the
 *  definition of HashEntries.
 *
 *  Entry must provide getKey() and getKenLength() functions.
 *
 *  Different environments have different demands of HashFunctions. For
 *  example, on-device linkers needs a more light-weight hash function
 *  than static linkers. TableImpl also provides a template argument to
 *  change the hash functions.
 */
template<typename KeyType, typename ValueType, typename HasherType,
         template<class, class> class EntryType = Entry>
class TableImpl
{
public:
  typedef size_t     size_type;
  typedef KeyType    key_type;
  typedef ValueType  value_type;
  typedef HasherType hasher_type;
  typedef EntryType<KeyType, ValueType> entry_type;

  /// \brief defines an element in the hash table.
  struct Bucket
  {
    unsigned int hash;
    entry_type* entry;
  };

  typedef Bucket bucket_type;

public:
  TableImpl();

  explicit TableImpl(unsigned int pInitSize);

  virtual ~TableImpl() { clear(); }

  bool empty() const { return (0 == m_NumOfEntries); }

  const Bucket& bucket(unsigned int pIdx) const { return m_Buckets[pIdx]; }

  Bucket&       bucket(unsigned int pIdx)       { return m_Buckets[pIdx]; }

  size_t numOfBuckets() const { return m_NumOfBuckets; }

  size_t numOfEntries() const { return m_NumOfEntries; }

  size_t numOfTombstones() const { return m_NumOfTombstones; }

  /// return EmptyBucket singleton.
  static entry_type* EmptyBucket();

  /// return Tombstone singleton.
  static entry_type* Tombstone();

protected:
  TableImpl(const TableImpl& pCopy);

  TableImpl(TableImpl&& pOther);

  TableImpl& operator=(const TableImpl& pCopy);

  TableImpl& operator=(TableImpl&& pOther);

  /// initialize the hash table.
  /// @return true only if the initialization succeeds.
  bool init(unsigned int pInitSize);

  void clear();

  /// lookupBucketFor - search the index of bucket whose key is pKey.
  /// If we can't found any one, then return a reusable bucket.
  /// @return the index of the found bucket
  unsigned int lookupBucketFor(const key_type& pKey);

  /// findKey - finds an element with key pKey
  //  return the index of the element, or -1 when the element does not exist.
  int findKey(const key_type& pKey) const;

  /// mayRehash - check the load_factor, compute the new size, and then doRehash
  void mayRehash();

  /// doRehash - re-new the hash table, and rehash all elements into the new buckets
  void doRehash(unsigned int pNewSize);

  /// isInit - return true if we've already initialized buckets.
  bool isInit() const { return (NULL != m_Buckets); }

protected:
  // Array of Buckets
  Bucket* m_Buckets;
  unsigned int m_NumOfBuckets;
  unsigned int m_NumOfEntries;
  unsigned int m_NumOfTombstones;
};

#include "HashBase.tcc"

} // namespace of internal
} // namespace of onnc

#endif
