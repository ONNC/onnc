//===- ObjectPool.tcc -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OBJECT_POOL_TCC
#define ONNC_SUPPORT_OBJECT_POOL_TCC

namespace onnc {

template<typename DataType, unsigned int Amount>
ObjectPool<DataType, Amount>::ObjectPool()
  : IListBase(), m_Size(0) {
}

template<typename DataType, unsigned int Amount>
ObjectPool<DataType, Amount>::~ObjectPool()
{
  clear();
}

template<typename DataType, unsigned int Amount>
void ObjectPool<DataType, Amount>::construct(pointer pPtr) const
{
  new (pPtr) DataType();
}

template<typename DataType, unsigned int Amount>
void ObjectPool<DataType, Amount>::destroy(pointer pPtr) const
{
  pPtr->~DataType();
}

template<typename DataType, unsigned int Amount>
typename ObjectPool<DataType, Amount>::pointer
ObjectPool<DataType, Amount>::allocate()
{
  IListNodeBase* cur = head();
  IListNodeBase* sentinel = getSentinel();

  while (cur != sentinel) {
    slab_type* slab = static_cast<slab_type*>(cur);
    for (unsigned int i = 0; i < Amount; ++i) {
      if (!slab->data[i].used) {
        slab->data[i].used = true;
        ++m_Size;
        return &slab->data[i].value;
      }
    }
    cur = cur->getNext();
  }

  /// All slabs are full; Append a new slab.
  slab_type* new_slab = new slab_type();
  doInsert(*sentinel, *new_slab);
  new_slab->data[0].used = true;
  ++m_Size;
  return &new_slab->data[0].value;
}

template<typename DataType, unsigned int Amount> bool
ObjectPool<DataType, Amount>::InSlab(slab_type* pSlab, pointer pPtr)
{
  return (((void*) pPtr > (void*) pSlab) &&
          ((void*) pPtr < (void*)(pSlab + sizeof(slab_type))));
}

template<typename DataType, unsigned int Amount> void
ObjectPool<DataType, Amount>::deallocate(pointer pPtr)
{
  IListNodeBase* cur = head();
  IListNodeBase* sentinel = getSentinel();

  while (cur != sentinel) {
    slab_type* slab = static_cast<slab_type*>(cur);
    if (InSlab(slab, pPtr)) {
      for (unsigned int i = 0; i < Amount; ++i) {
        if (&slab->data[i].value == pPtr && slab->data[i].used) {
          slab->data[i].used = false;
          --m_Size;
          return;
        }
      }
    } // end of skip condition
    cur = cur->getNext();
  } // end of for
}

template<typename DataType, unsigned int Amount>
void ObjectPool<DataType, Amount>::clear()
{
  IListNodeBase* cur = head();
  IListNodeBase* sentinel = getSentinel();

  while (cur != sentinel) {
    IListNodeBase* next = cur->getNext();
    remove(*cur);
    delete cur;
    cur = next;
  }

  // The last node is sentinel. Do not delete it.
  doReset();
  m_Size = 0;
}

} // namespace of onnc

#endif
