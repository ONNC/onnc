//===- MemoryAllocator.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_MEMORY_ALLOCATOR_H
#define ONNC_ADT_MEMORY_ALLOCATOR_H
#include <onnc/ADT/IList.h>
#include <onnc/Support/Slab.h>

namespace onnc {

/** \class MemoryAllocator
 *  \brief MemoryAllocator allocates numerous small objects.
 *
 *  MemoryAllocator is a memory pool allocator. It preallocates a number
 *  of memory blocks to avoid the system from memory fragmentation.
 *
 *  MemoryAllocator doesn't provide deallocator. Users should design his own
 *  deallocators. That is because the implementation of deallocators need
 *  careful consideration of trade-offs between speed and memory usage, and
 *  we think users should design one for themselves.
 */
template<typename DataType, unsigned int Amount = 64>
class MemoryAllocator : public IListBase
{
public:
  typedef DataType value_type;
  typedef const DataType* const_pointer;
  typedef const DataType& const_reference;
  typedef DataType* pointer;
  typedef DataType& reference;

  typedef Slab<DataType, Amount> slab_type;

  template<typename NewDataType>
  struct rebind {
    typedef MemoryAllocator<NewDataType, Amount> other;
  };

public:
  MemoryAllocator();

  virtual ~MemoryAllocator() { }

  /// allocate - to allocate one datum.
  pointer allocate();

  /// construct - to construct an element object on the location of pPtr
  void construct(pointer pPtr) const;

  /// construct - to construct an element object on the location of pointed by
  /// pPtr
  void construct(pointer pPtr, const_reference pVal) const;

  unsigned int size() const { return m_Size; }

protected:
  void doCountIn(unsigned int pN = 1) { m_Size += pN; }

  void doCountOut(unsigned int pN = 1) { m_Size -= pN; }

  void doReset() {
    IListBase::doReset();
    m_Size = 0;
  }

  void doConcatenate(MemoryAllocator& pClient) {
    m_Size += pClient.size();
    IListBase::doConcatenate(pClient);
  }

private:
  using IListBase::size;

protected:
  unsigned int m_Size;
};

//===----------------------------------------------------------------------===//
// MemoryAllocator
//===----------------------------------------------------------------------===//
template<typename DataType, unsigned int Amount>
MemoryAllocator<DataType, Amount>::MemoryAllocator()
  : IListBase(), m_Size(0) {
}

template<typename DataType, unsigned int Amount>
typename MemoryAllocator<DataType, Amount>::pointer
MemoryAllocator<DataType, Amount>::allocate()
{
  unsigned int index = this->size() % Amount;
  doCountIn();
  if (0 == index) { // slab_type full
    slab_type* new_slab = new slab_type();
    doInsert(*getSentinel(), *new_slab);
    return new_slab->data;
  }

  return static_cast<slab_type*>(IListBase::getSentinel()->getPrev())->data + index;
}

template<typename DataType, unsigned int Amount> void
MemoryAllocator<DataType, Amount>::construct(pointer pPtr) const
{
  slab_type::construct(pPtr);
}

template<typename DataType, unsigned int Amount> void
MemoryAllocator<DataType, Amount>::construct(pointer pPtr,
                                             const_reference pVal) const
{
  slab_type::construct(pPtr, pVal);
}

} // namespace of onnc

#endif
