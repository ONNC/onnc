//===- StringMap.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_STRING_MAP_H
#define ONNC_ADT_STRING_MAP_H
#include <onnc/ADT/StringHashTable.h>

namespace onnc {

template<class ValueType>
using StringMap = StringHashTable<ValueType,
                                  internal::StringHasher<internal::SDBM> >;

} // namespace of onnc

#endif
