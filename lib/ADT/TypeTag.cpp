//===-- TypeTag.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/TypeTag.h>

//===----------------------------------------------------------------------===//
// Type Tags
//===----------------------------------------------------------------------===//
template<> onnc::TypeTag onnc::type_tag<bool>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<int>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<long>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<unsigned int>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<unsigned long>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<float>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<double>()
{
  static bool instance;
  return &instance;
}

template<> onnc::TypeTag onnc::type_tag<const char*>()
{
  static bool instance;
  return &instance;
}
