//===- MallocAllocator.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_MALLOC_ALLOCATOR_H
#define ONNC_SUPPORT_MALLOC_ALLOCATOR_H

#include <cstdlib>

namespace onnc {

/**
 *  \brief MallocAllocator allocate objects in dynamic storage.
 *  \tparam DataType the type of allocated objects
 */
template<typename DataType>
class MallocAllocator
{
public:
  typedef size_t          size_type;
  typedef ptrdiff_t       difference_type;
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
  MallocAllocator() throw() { }

  MallocAllocator(const MallocAllocator&) throw() { }

  MallocAllocator(MallocAllocator&&) throw() { }

  ~MallocAllocator() throw() { }

  MallocAllocator& operator=(const MallocAllocator&) throw() { return *this; }

  MallocAllocator& operator=(MallocAllocator&&) throw() { return *this; }

  const_pointer address(const_reference X) const { return &X; }
  pointer       address(reference       X) const { return &X; }

  pointer allocate(size_type pNumOfElements = 1, const void* pU = NULL) {
    return static_cast<pointer>(std::malloc(pNumOfElements*sizeof(DataType)));
  }

  void deallocate(pointer pObject, size_type pN = 0)
  { std::free(static_cast<void*>(pObject)); }

  size_type max_size() const throw()
  { return size_t(-1) / sizeof(DataType); }

  void construct(pointer pObject)
  { ::new((void *)pObject) value_type(); }

  void construct(pointer pObject, const DataType& pValue)
  { ::new((void *)pObject) value_type(pValue); }

  void destroy(pointer pObject)
  { pObject->~DataType(); }
};

template<>
class MallocAllocator<void>
{
public:
  typedef size_t      size_type;
  typedef ptrdiff_t   difference_type;
  typedef void*       pointer;
  typedef const void* const_pointer;
  typedef void*       reference;
  typedef const void* const_reference;
  typedef void*       value_type;

  template<typename OtherDataType>
  struct rebind
  {
    typedef MallocAllocator<OtherDataType> other;
  };

public:
  MallocAllocator() throw() { }

  MallocAllocator(const MallocAllocator&) throw() { }

  MallocAllocator(MallocAllocator&&) throw() { }

  ~MallocAllocator() throw() { }

  MallocAllocator& operator=(const MallocAllocator&) throw() { return *this; }

  MallocAllocator& operator=(MallocAllocator&&) throw() { return *this; }

  size_type max_size() const throw()
  { return size_t(-1) / sizeof(void*); }

  const_pointer address(const_reference X) const { return X; }
  pointer       address(reference       X) const { return X; }

  pointer allocate(size_type pNumOfElements, const void* pU = NULL)
  { return std::malloc(pNumOfElements); }

  void deallocate(pointer pObject, size_type pN = 0)
  { std::free(pObject); }
};

} // namespace onnc

#endif
