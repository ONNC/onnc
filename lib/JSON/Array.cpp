//===- Array.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/Array.h>
#include <onnc/JSON/Value.h>
#include <ostream>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// Array
//===----------------------------------------------------------------------===//
Array::Array()
  : std::vector<Value>(), Notation(ARRAY) {
}

Array::Array(const Array& pArray)
  : std::vector<Value>(pArray), Notation(ARRAY) {
}

Array& Array::operator=(const Array& pArray)
{
  std::vector<Value>::operator=(pArray);
  Notation::operator=(pArray);
  return *this;
}

void Array::print(IndentOStream& pOS) const
{
  pOS << "[ ";
  onnc::json::Array::const_iterator e = begin(), eEnd = end();
  while (e != eEnd) {
    e->print(pOS);
    if (++e != eEnd)
      pOS << ",";
    pOS << ' ';
  }
  pOS << ']';
}
