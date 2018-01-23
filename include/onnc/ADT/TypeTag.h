//===- TypeTag.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_TYPE_TAG_H
#define ONNC_ADT_TYPE_TAG_H

#include <cstdlib>

namespace onnc {

typedef void* TypeTag;

enum MySQLTypeTag
{
  kTypeBool,
  kTypeInt,
  kTypeLong,
  kTypeUnsignedInt,
  kTypeUnsignedLong,
  kTypeFloat,
  kTypeDouble,
  kTypeConstCharP,
  kTypeStringRef,
};

template<typename ValueType> TypeTag type_tag() { return NULL; }

template<> TypeTag type_tag<bool>();
template<> TypeTag type_tag<int>();
template<> TypeTag type_tag<long>();
template<> TypeTag type_tag<unsigned int>();
template<> TypeTag type_tag<unsigned long>();
template<> TypeTag type_tag<float>();
template<> TypeTag type_tag<double>();
template<> TypeTag type_tag<const char*>();

} // namespace of onnc

#endif
