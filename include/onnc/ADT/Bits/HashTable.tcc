//===- HashTable.tcc ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// constructor with initial size
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
HashTable<K, V, H, E, A>::HashTable(size_type pSize)
  : internal::TableImpl<K, V, H>(pSize) {
}

// copy constructor
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
HashTable<K, V, H, E, A>::HashTable(const HashTable& pCopy)
  : internal::TableImpl<K, V, H>(pCopy) {
  if (empty())
    return;

  // for all entries in pCopy, allocate an entry.
  for (unsigned int i=0; i < numOfBuckets(); ++i) {
    if (EmptyBucket() != super::m_Buckets[i].entry &&
        Tombstone() != super::m_Buckets[i].entry ) {
      // copy entry
      const key_type& key = pCopy.super::m_Buckets[i].entry->key();
      super::m_Buckets[i].entry = m_EntryAlloc.allocate(1, &key);
      entry_type* entry = new (super::m_Buckets[i].entry) entry_type(key);
      entry->setValue(pCopy.m_Buckets[i].entry->value());
    }
  }
}

// Move constructor
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
HashTable<K, V, H, E, A>::HashTable(HashTable&& pOther)
  : internal::TableImpl<K, V, H>(std::move(pOther)) {
  if (std::is_move_assignable<allocator_type>::value)
    m_EntryAlloc = std::move(pOther.m_EntryAlloc);
}

// destructor
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
HashTable<K, V, H, E, A>::~HashTable()
{
  if (empty())
    return;

  /** destruct all **/
  for (unsigned int i=0; i < numOfBuckets(); ++i) {
    if (EmptyBucket() != super::m_Buckets[i].entry &&
        Tombstone() != super::m_Buckets[i].entry ) {
      auto* const entry = super::m_Buckets[i].entry;
      m_EntryAlloc.destroy(entry);
      m_EntryAlloc.deallocate(entry);
    }
  }
}

// Copy assignment
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
HashTable<K, V, H, E, A>&
HashTable<K, V, H, E, A>::operator=(const HashTable& pCopy)
{

  internal::TableImpl<K, V, H>::operator=(pCopy);

  // for all entries in pCopy, allocate an entry.
  for (unsigned int i=0; i < numOfBuckets(); ++i) {
    if (EmptyBucket() != super::m_Buckets[i].entry &&
        Tombstone() != super::m_Buckets[i].entry ) {
      // copy entry
      const key_type& key = pCopy.m_Buckets[i].entry->key();
      super::m_Buckets[i].entry = m_EntryAlloc.allocate(1, &key);
      entry_type* entry = new (super::m_Buckets[i].entry) entry_type(key);
      entry->setValue(pCopy.super::m_Buckets[i].entry->value());
    }
  }
  return *this;
}

// Move assignment
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
HashTable<K, V, H, E, A>&
HashTable<K, V, H, E, A>::operator=(HashTable&& pOther)
{
  internal::TableImpl<K, V, H>::operator=(std::move(pOther));
  if (std::is_move_assignable<allocator_type>::value)
    m_EntryAlloc = std::move(pOther.m_EntryAlloc);
  return *this;
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
void HashTable<K, V, H, E, A>::clear()
{
  if (empty())
    return;

  /** clean up **/
  for (unsigned int i=0; i < numOfBuckets(); ++i) {
    if (EmptyBucket() != super::m_Buckets[i].entry ) {
      if (Tombstone() != super::m_Buckets[i].entry ) {
        auto* const entry = super::m_Buckets[i].entry;
        m_EntryAlloc.destroy(entry);
        m_EntryAlloc.deallocate(entry);
      }
      super::m_Buckets[i].entry = EmptyBucket();
    }
  }

  super::clear();
}

/// insert - insert a new element to the container. If the element already
//  exist, return the element.
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::entry_type*
HashTable<K, V, H, E, A>::insert(const key_type& pKey, bool& pExist)
{
  unsigned int index = super::lookupBucketFor(pKey);
  bucket_type& bucket = super::m_Buckets[index];
  entry_type* entry = bucket.entry;
  if (EmptyBucket() != entry && Tombstone() != entry) {
    // Already exist in the table
    pExist = true;
    return entry;
  }

  // find a tombstone
  if (Tombstone() == entry)
    --super::m_NumOfTombstones;

  // construct an entry
  entry = m_EntryAlloc.allocate(1, &pKey);

  // constructor with key type is necessary.
  new (entry) entry_type(pKey);

  // set up the bucket
  bucket.entry = entry;

  ++super::m_NumOfEntries;
  super::mayRehash();
  pExist = false;
  return entry;
}

/// erase
template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
bool HashTable<K, V, H, E, A>::erase(const key_type& pKey)
{
  int index;
  if (-1 == (index = super::findKey(pKey)))
    return false;

  bucket_type& bucket = super::m_Buckets[index];
  m_EntryAlloc.destroy(bucket.entry);
  m_EntryAlloc.deallocate(bucket.entry);
  bucket.entry = Tombstone();

  --super::m_NumOfEntries;
  ++super::m_NumOfTombstones;
  super::mayRehash();
  return true;
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::iterator
HashTable<K, V, H, E, A>::find(const key_type& pKey)
{
  int index;
  if (-1 == (index = super::findKey(pKey)))
    return end();
  return iterator(this, index);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::const_iterator
HashTable<K, V, H, E, A>::find(const key_type& pKey) const
{
  int index;
  if (-1 == (index = super::findKey(pKey)))
    return end();
  return const_iterator(this, index);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::size_type
HashTable<K, V, H, E, A>::count(const key_type& pKey) const
{
  const_chain_iterator bucket, bEnd = end(pKey);
  size_type count = 0;
  for (bucket = begin(pKey); bucket != bEnd; ++bucket)
    ++count;
  return count;
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
float HashTable<K, V, H, E, A>::load_factor() const
{
  return ((float)super::m_NumOfEntries/(float)super::m_NumOfBuckets);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
void HashTable<K, V, H, E, A>::rehash()
{
  super::mayRehash();
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
void HashTable<K, V, H, E, A>::rehash(size_type pCount)
{
  super::doRehash(pCount);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::iterator
HashTable<K, V, H, E, A>::begin()
{
  if (super::empty())
    return end();
  unsigned int index = 0;
  while (Tombstone() == super::m_Buckets[index].entry ||
         EmptyBucket() == super::m_Buckets[index].entry) {
    ++index;
  }
  return iterator(this, index);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::iterator
HashTable<K, V, H, E, A>::end()
{
  return iterator(NULL, 0);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::const_iterator
HashTable<K, V, H, E, A>::begin() const
{
  if (super::empty())
    return end();
  unsigned int index = 0;
  while (Tombstone() == super::m_Buckets[index].entry ||
         EmptyBucket() == super::m_Buckets[index].entry) {
    ++index;
  }
  return const_iterator(this, index);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::const_iterator
HashTable<K, V, H, E, A>::end() const
{
  return const_iterator(NULL, 0);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::chain_iterator
HashTable<K, V, H, E, A>::begin(const key_type& pKey)
{
  return chain_iterator(this, pKey, 0x0);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::chain_iterator
HashTable<K, V, H, E, A>::end(const key_type& pKey)
{
  return chain_iterator();
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::const_chain_iterator
HashTable<K, V, H, E, A>::begin(const key_type& pKey) const
{
  return const_chain_iterator(this, pKey, 0x0);
}

template<typename K, typename V, typename H,
         template<class, class> class E, template<class> class A>
typename HashTable<K, V, H, E, A>::const_chain_iterator
HashTable<K, V, H, E, A>::end(const key_type& pKey) const
{
  return const_chain_iterator();
}
