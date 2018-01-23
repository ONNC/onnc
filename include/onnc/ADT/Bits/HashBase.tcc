//===- HashBase.tcc -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

enum { NumOfInitBuckets = 16 };

//===----------------------------------------------------------------------===//
// internal non-member functions
//===----------------------------------------------------------------------===//
inline static unsigned int compute_bucket_size(unsigned int pNumOfBuckets)
{
  static const unsigned int bucket_size[] =
  {
    1, 3, 17, 37, 67, 97, 197, 419, 977, 2593, 4099, 8209, 12289,
    16411, 20483, 32771, 49157, 65537, 98317, 131101, 196613
  };

  const unsigned int buckets_count =
      sizeof(bucket_size) / sizeof(bucket_size[0]);

  unsigned int idx = 0;
  do {
    if (pNumOfBuckets < bucket_size[idx]) {
      return bucket_size[idx];
    }
    ++idx;
  } while(idx < buckets_count);

  return (pNumOfBuckets+131101); // rare case. increase constantly
}

//===----------------------------------------------------------------------===//
// template implementation of TableImpl
//===----------------------------------------------------------------------===//
template<typename K, typename V, typename H, template<class, class> class E>
TableImpl<K, V, H, E>::TableImpl()
  : m_Buckets(nullptr),
    m_NumOfBuckets(0),
    m_NumOfEntries(0),
    m_NumOfTombstones(0) {
}

template<typename K, typename V, typename H, template<class, class> class E>
TableImpl<K, V, H, E>::TableImpl(unsigned int pInitSize)
  : m_Buckets(nullptr),
    m_NumOfBuckets(0),
    m_NumOfEntries(0),
    m_NumOfTombstones(0) {

  if (0 != pInitSize) {
    init(pInitSize);
    return;
  }
}

template<typename K, typename V, typename H, template<class, class> class E>
TableImpl<K, V, H, E>::TableImpl(const TableImpl& pCopy)
  : m_Buckets(nullptr),
    m_NumOfBuckets(pCopy.m_NumOfBuckets),
    m_NumOfEntries(pCopy.m_NumOfEntries),
    m_NumOfTombstones(pCopy.m_NumOfTombstones) {

  m_Buckets = (Bucket*)malloc(m_NumOfBuckets * sizeof(Bucket));
  ::memcpy(m_Buckets, pCopy.m_Buckets, m_NumOfBuckets * sizeof(Bucket));

  // Because TableImpl doesn't handle with entries, here we doesn't copy entries
}

template<typename K, typename V, typename H, template<class, class> class E>
TableImpl<K, V, H, E>::TableImpl(TableImpl&& pOther)
  : m_Buckets(nullptr),
    m_NumOfBuckets(0),
    m_NumOfEntries(0),
    m_NumOfTombstones(0) {

  m_Buckets = pOther.m_Buckets;
  m_NumOfBuckets = pOther.m_NumOfBuckets;
  m_NumOfEntries = pOther.m_NumOfEntries;
  m_NumOfTombstones = pOther.m_NumOfTombstones;

  pOther.m_Buckets = nullptr;
  pOther.m_NumOfBuckets = 0;
  pOther.m_NumOfEntries = 0;
  pOther.m_NumOfTombstones = 0;
}

template<typename K, typename V, typename H, template<class, class> class E>
TableImpl<K, V, H, E>& TableImpl<K, V, H, E>::operator=(const TableImpl& pCopy)
{
  if (isInit())
    clear();

  m_NumOfBuckets = pCopy.m_NumOfBuckets;
  m_NumOfEntries = pCopy.m_NumOfEntries;
  m_NumOfTombstones = pCopy.m_NumOfTombstones;

  m_Buckets = (Bucket*)malloc(m_NumOfBuckets * sizeof(Bucket));
  ::memcpy(m_Buckets, pCopy.m_Buckets, m_NumOfBuckets * sizeof(Bucket));

  // Because TableImpl doesn't handle with entries, here we doesn't copy entries
  return *this;
}

// Move assignment
template<typename K, typename V, typename H, template<class, class> class E>
TableImpl<K, V, H, E>& TableImpl<K, V, H, E>::operator=(TableImpl&& pOther)
{
  if (this != &pOther) {
    m_Buckets = pOther.m_Buckets;
    m_NumOfBuckets = pOther.m_NumOfBuckets;
    m_NumOfEntries = pOther.m_NumOfEntries;
    m_NumOfTombstones = pOther.m_NumOfTombstones;

    pOther.m_Buckets = nullptr;
    pOther.m_NumOfBuckets = 0;
    pOther.m_NumOfEntries = 0;
    pOther.m_NumOfTombstones = 0;
  }
  return *this;
}

/// init - initialize the hash table.
template<typename K, typename V, typename H, template<class, class> class E>
bool TableImpl<K, V, H, E>::init(unsigned int pInitSize)
{
  m_NumOfBuckets = pInitSize ? compute_bucket_size(pInitSize)
                             : (unsigned int) NumOfInitBuckets;

  m_NumOfEntries = 0;
  m_NumOfTombstones = 0;

  /** calloc also set bucket.Item = Bucket::getEmptyStone() **/
  m_Buckets = (Bucket*)calloc(m_NumOfBuckets, sizeof(Bucket));
  return (nullptr != m_Buckets);
}

/// clear - clear the hash table.
template<typename K, typename V, typename H, template<class, class> class E>
void TableImpl<K, V, H, E>::clear()
{
  ::free(m_Buckets);

  m_Buckets = 0;
  m_NumOfBuckets = 0;
  m_NumOfEntries = 0;
  m_NumOfTombstones = 0;
}

/// lookupBucketFor - look up the bucket whose key is pKey
template<typename K, typename V, typename H, template<class, class> class E>
unsigned int TableImpl<K, V, H, E>::lookupBucketFor(const key_type& pKey)
{
  if (!isInit()) {
    init(NumOfInitBuckets);
  }

  hasher_type hasher;
  unsigned int full_hash = hasher(pKey);
  unsigned int index = full_hash % numOfBuckets();

  const unsigned int probe = 1;
  int first_tombstone = -1;

  // linear probing
  while(true) {
    Bucket& bucket = m_Buckets[index];
    // If we found an empty bucket, this key isn't in the table yet, return it.
    if (EmptyBucket() == bucket.entry) {
      if (-1 != first_tombstone) {
        m_Buckets[first_tombstone].hash = full_hash;
        return first_tombstone;
      }

      bucket.hash = full_hash;
      return index;
    }

    if (Tombstone() == bucket.entry) {
      if (-1 == first_tombstone) {
        first_tombstone = index;
      }
    }
    else if (bucket.hash == full_hash) {
      if (bucket.entry->key() == pKey) {
        return index;
      }
    }

    index += probe;
    if (index == numOfBuckets())
      index = 0;
  }
}

template<typename K, typename V, typename H, template<class, class> class E>
int TableImpl<K, V, H, E>::findKey(const key_type& pKey) const
{
  if (!isInit())
    return -1;

  hasher_type hasher;
  unsigned int full_hash = hasher(pKey);
  unsigned int index = full_hash % numOfBuckets();

  const unsigned int probe = 1;
  // linear probing
  while (true) {
    Bucket &bucket = m_Buckets[index];

    if (EmptyBucket() == bucket.entry)
      return -1;

    if (Tombstone() == bucket.entry) {
      // Ignore tombstones.
    }
    else if (full_hash == bucket.hash) {
      // get string, compare, if match, return index
      if (bucket.entry->key() == pKey)
        return index;
    }
    index += probe;
    if (index == numOfBuckets())
      index = 0;
  }
}

template<typename K, typename V, typename H, template<class, class> class E>
void TableImpl<K, V, H, E>::mayRehash()
{
  unsigned int new_size;

  // If the hash table is now more than 3/4 full, or if fewer than 1/8 of
  // the buckets are empty (meaning that many are filled with tombstones),
  // grow/rehash the table.
  if ((m_NumOfEntries<<2) > m_NumOfBuckets*3)
    new_size = compute_bucket_size(m_NumOfBuckets);
  else if (((m_NumOfBuckets-(m_NumOfEntries+m_NumOfTombstones))<<3) < m_NumOfBuckets)
    new_size = m_NumOfBuckets;
  else
    return;

  doRehash(new_size);
}

template<typename K, typename V, typename H, template<class, class> class E>
void TableImpl<K, V, H, E>::doRehash(unsigned int pNewSize)
{
  Bucket* new_table = (Bucket*)calloc(pNewSize, sizeof(Bucket));

  // Rehash all the items into their new buckets.  Luckily :) we already have
  // the hash values available, so we don't have to recall hash function again.
  Bucket* e = m_Buckets + m_NumOfBuckets;
  for (Bucket* ib = m_Buckets; ib != e; ++ib) {
    if (ib->entry != EmptyBucket() && ib->entry != Tombstone()) {
      // Fast case, bucket available.
      unsigned full_hash = ib->hash;
      unsigned new_bucket = full_hash % pNewSize;
      if (EmptyBucket() == new_table[new_bucket].entry) {
        new_table[new_bucket].entry = ib->entry;
        new_table[new_bucket].hash = full_hash;
        continue;
      }

      // Otherwise probe for a spot.
      const unsigned int probe = 1;
      do {
        new_bucket += probe;
        if (new_bucket == pNewSize)
          new_bucket = 0;
      } while (new_table[new_bucket].entry != EmptyBucket());

      // Finally found a slot.  Fill it in.
      new_table[new_bucket].entry = ib->entry;
      new_table[new_bucket].hash = full_hash;
    }
  }

  ::free(m_Buckets);

  m_Buckets = new_table;
  m_NumOfBuckets = pNewSize;
  m_NumOfTombstones = 0;
}

template<typename K, typename V, typename H, template<class, class> class E>
typename TableImpl<K, V, H, E>::entry_type*
TableImpl<K, V, H, E>::EmptyBucket()
{
  static entry_type* empty_bucket = reinterpret_cast<entry_type*>(0x0);
  return empty_bucket;
}

template<typename K, typename V, typename H, template<class, class> class E>
typename TableImpl<K, V, H, E>::entry_type*
TableImpl<K, V, H, E>::Tombstone()
{
  static entry_type* tombstone = reinterpret_cast<entry_type*>(0x1);
  return tombstone;
}
