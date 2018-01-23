//===- Slab.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_SLAB_H
#define ONNC_SUPPORT_SLAB_H
#include <onnc/ADT/IListNode.h>

namespace onnc {

/** \class Slab
 *  \brief Slab is a continuous memory space that amounts to `Amount`
 *  `DataType` elements.
 *
 *  Slab is a basic allocated unit of BumpAllocator.
 *
 *  @tparam DataType the type of elements
 *  @tparam Amount   the amount of elements in a slab
 */
template<typename DataType, unsigned int Amount = 64>
struct Slab : public IListNodeBase
{
public:
  enum {
    max_size = Amount
  };

public:
  static void construct(DataType* pPtr) { new (pPtr) DataType(); }

  static void
  construct(DataType* pPtr, const DataType& pValue) { new (pPtr) DataType(pValue); }

  static unsigned int size() { return Amount; }

  static void destroy(DataType* pPtr) { /** XXX: do nothing **/ }

public:
  DataType data[max_size];
};

} // namespace onnc

#endif
