//===- StringHashTable.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_STRING_HASH_TABLE_H
#define ONNC_ADT_STRING_HASH_TABLE_H
#include <onnc/ADT/HashTable.h>
#include <onnc/ADT/StringHasher.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/DataTypes.h>
#include <onnc/Support/MallocAllocator.h>
#include <cassert>
#include <cctype>
#include <functional>

namespace onnc {
namespace internal {

/**
 * Partial specification version of struct Entry for StringRef.
 */
template<class ValueType>
struct Entry<StringRef, ValueType>
{
public:
  typedef StringRef key_type;
  typedef ValueType value_type;

public:
  Entry(const key_type& pKey) : m_KeyLen(pKey.size()) {
    std::memcpy(m_Key, pKey.data(), pKey.size());
    m_Key[pKey.size()] = '\0';
  }

  key_type key() const { return StringRef(m_Key, m_KeyLen); }

  void setKey(const key_type& pKey) { m_Key = pKey; }

  const value_type& value() const { return m_Value; }

  value_type&       value()       { return m_Value; }

  void setValue(const value_type& pValue) { m_Value = pValue; }

protected:
  value_type m_Value;

  uint16_t m_KeyLen;

  char m_Key[];
};

} // namespace hash

/**
 * Partial specification MallocAllocator for Entry<StringRef, ValueType>
 */
template<class ValueType>
class MallocAllocator<internal::Entry<StringRef, ValueType> >
{
public:
  typedef size_t          size_type;
  typedef ptrdiff_t       difference_type;
  typedef internal::Entry<StringRef, ValueType> DataType;

  typedef DataType*       pointer;
  typedef const DataType* const_pointer;
  typedef DataType&       reference;
  typedef const DataType& const_reference;
  typedef DataType        value_type;

  template<typename OtherDataType>
  struct rebind
  {
    typedef MallocAllocator<OtherDataType> other;
  };

public:
  MallocAllocator() throw()
  { }

  MallocAllocator(const MallocAllocator&) throw()
  { }

  ~MallocAllocator() throw()
  { }

  const_pointer address(const_reference X) const { return &X; }
  pointer       address(reference       X) const { return &X; }

  pointer allocate(size_type pNumOfElements, const StringRef* pKey) {
    return static_cast<pointer>(
      std::malloc(sizeof(internal::Entry<StringRef, ValueType>) + pKey->size() + 1));
  }

  void deallocate(pointer pObject, size_type pN = 0)
  { std::free(static_cast<void*>(pObject)); }

  size_type max_size() const throw()
  { return size_t(-1) / sizeof(DataType); }

  void destroy(pointer pObject)
  { pObject->~DataType(); }
};

template<class ValueType,
         class HasherType = internal::StringHasher<internal::ELF> >
using StringHashTable = HashTable<StringRef, ValueType, HasherType>;

} // namespace of onnc

#endif
