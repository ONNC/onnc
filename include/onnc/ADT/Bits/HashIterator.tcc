//===- HashIterator.tcc ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// ChainIterBase
//===----------------------------------------------------------------------===//
template<typename HT>
ChainIterBase<HT>::ChainIterBase()
  : m_pHashTable(0), m_Index(0), m_HashValue(0), m_EndIndex(0) {
}

template<typename HT>
ChainIterBase<HT>::ChainIterBase(HT* pTable, const key_type& pKey)
  : m_pHashTable(pTable) {
  typename HT::hasher_type hasher;
  m_HashValue = hasher(pKey);
  m_EndIndex = m_Index = m_HashValue % m_pHashTable->numOfBuckets();
  const unsigned int probe = 1;
  while(true) {
    const bucket_type &bucket = m_pHashTable->bucket(m_Index);
    if (hash_table::Tombstone() == bucket.entry) {
      // Ignore tombstones.
    }
    else if (m_HashValue == bucket.hash) {
      if (bucket.entry->key() == pKey) {
        m_EndIndex = m_Index;
        break;
      }
    }

    m_Index += probe;
    if (m_Index == m_pHashTable->numOfBuckets())
      m_Index = 0;
    // doesn't exist
    if (m_EndIndex == m_Index) {
      reset();
      break;
    }
  }
}

template<typename HT>
ChainIterBase<HT>::ChainIterBase(const ChainIterBase& pCopy)
  : m_pHashTable(pCopy.m_pHashTable),
    m_Index(pCopy.m_Index),
    m_HashValue(pCopy.m_HashValue),
    m_EndIndex(pCopy.m_EndIndex) {
}

template<typename HT> ChainIterBase<HT>&
ChainIterBase<HT>::assign(const ChainIterBase& pCopy)
{
  m_pHashTable = pCopy.m_pHashTable;
  m_Index = pCopy.m_Index;
  m_HashValue = pCopy.m_HashValue;
  m_EndIndex = pCopy.m_EndIndex;
  return *this;
}

template<typename HT>
typename ChainIterBase<HT>::bucket_type*
ChainIterBase<HT>::getBucket()
{
  assert(NULL != m_pHashTable && "trivial iterator points to no bucket");
  return &(m_pHashTable->bucket(m_Index));
}

template<typename HT>
const typename ChainIterBase<HT>::bucket_type*
ChainIterBase<HT>::getBucket() const
{
  assert(NULL != m_pHashTable && "trivial iterator points to no bucket");
  return &(m_pHashTable->bucket(m_Index));
}

template<typename HT>
const typename ChainIterBase<HT>::entry_type*
ChainIterBase<HT>::getEntry() const
{
  assert(NULL != m_pHashTable && "trivial iterator points to no entry");
  return m_pHashTable->bucket(m_Index).entry;
}

template<typename HT>
typename ChainIterBase<HT>::entry_type*
ChainIterBase<HT>::getEntry()
{
  assert(NULL != m_pHashTable && "trivial iterator points to no entry");
  return m_pHashTable->bucket(m_Index).entry;
}

template<typename HT>
void ChainIterBase<HT>::reset()
{
  m_pHashTable = NULL;
  m_Index = 0;
  m_EndIndex = 0;
  m_HashValue = 0;
}

template<typename HT>
void ChainIterBase<HT>::advance()
{
  if (0 == m_pHashTable)
    return;

  const unsigned int probe = 1;
  while(true) {
    m_Index += probe;
    if (m_Index == m_pHashTable->numOfBuckets())
      m_Index = 0;
    // reach end()
    if (m_Index == m_EndIndex) {
      reset();
      return;
    }

    const bucket_type &bucket = m_pHashTable->bucket(m_Index);

    if (hash_table::Tombstone() == bucket.entry ||
        hash_table::EmptyBucket() == bucket.entry) {
      // Ignore tombstones.
    }
    else if (m_HashValue == bucket.hash) {
      return;
    }
  }
}

template<typename HT> bool
ChainIterBase<HT>::operator==(const ChainIterBase& pCopy) const
{
  if (m_pHashTable == pCopy.m_pHashTable) {
    if (0 == m_pHashTable)
      return true;
    return ((m_HashValue == pCopy.m_HashValue) &&
            (m_EndIndex == pCopy.m_EndIndex) &&
            (m_Index == pCopy.m_Index));
  }
  return false;
}

template<typename HT> bool
ChainIterBase<HT>::operator!=(const ChainIterBase& pCopy) const
{
  return !(*this == pCopy);
}

//===----------------------------------------------------------------------===//
// EntryIterBase
//===----------------------------------------------------------------------===//
template<typename HT>
EntryIterBase<HT>::EntryIterBase()
  : m_pHashTable(0), m_Index(0) {
}

template<typename HT>
EntryIterBase<HT>::EntryIterBase(HT* pTable, unsigned int pIndex)
  : m_pHashTable(pTable), m_Index(pIndex) {
}

template<typename HT>
EntryIterBase<HT>::EntryIterBase(const EntryIterBase& pCopy)
  : m_pHashTable(pCopy.m_pHashTable), m_Index(pCopy.m_Index) {
}

template<typename HT> EntryIterBase<HT>&
EntryIterBase<HT>::assign(const EntryIterBase& pCopy)
{
  m_pHashTable = pCopy.m_pHashTable;
  m_Index = pCopy.m_Index;
  return *this;
}

template<typename HT>
typename EntryIterBase<HT>::bucket_type*
EntryIterBase<HT>::getBucket()
{
  assert(NULL != m_pHashTable && "trivial iterator doesn't have bucket");
  return &(m_pHashTable->bucket(m_Index));
}

template<typename HT>
const typename EntryIterBase<HT>::bucket_type*
EntryIterBase<HT>::getBucket() const
{
  assert(NULL != m_pHashTable && "trivial iterator doesn't have bucket");
  return &(m_pHashTable->bucket(m_Index));
}

template<typename HT>
typename EntryIterBase<HT>::entry_type*
EntryIterBase<HT>::getEntry()
{
  assert(NULL != m_pHashTable && "trivial iterator doesn't have entry");
  return m_pHashTable->bucket(m_Index).entry;
}

template<typename HT>
const typename EntryIterBase<HT>::entry_type*
EntryIterBase<HT>::getEntry() const
{
  assert(NULL != m_pHashTable && "trivial iterator doesn't have entry");
  return m_pHashTable->bucket(m_Index).entry;
}

template<typename HT>
void EntryIterBase<HT>::reset()
{
  m_pHashTable = 0;
  m_Index = 0;
}

template<typename HT>
void EntryIterBase<HT>::advance()
{
  assert(NULL != m_pHashTable && "trivial iterator can't advance");
  do {
    ++m_Index;
    if (m_pHashTable->numOfBuckets() == m_Index) { // to the end
      reset();
      return;
    }
  } while(hash_table::EmptyBucket() == m_pHashTable->bucket(m_Index).entry ||
          hash_table::Tombstone() == m_pHashTable->bucket(m_Index).entry);
}

template<typename HT>
bool EntryIterBase<HT>::operator==(const EntryIterBase& pCopy) const
{
  return ((m_pHashTable == pCopy.m_pHashTable) && (m_Index == pCopy.m_Index));
}

template<typename HT>
bool EntryIterBase<HT>::operator!=(const EntryIterBase& pCopy) const
{
  return !(*this == pCopy);
}
