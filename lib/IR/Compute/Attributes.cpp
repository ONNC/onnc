//===- Attributes.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Attributes.h>

using namespace onnc;

// ScalarAttribute
template<> void BoolAttr::print(std::ostream& pOS) const {
  pOS << value();
}
template<> void FloatAttr::print(std::ostream& pOS) const {
  pOS << value();
}
template<> void IntAttr::print(std::ostream& pOS) const {
  pOS << value();
}
template<> void StringAttr::print(std::ostream& pOS) const {
  pOS << '"' << value() << '"';
}
template<> void TensorAttr::print(std::ostream& pOS) const {
  value().print(pOS);
}
template<> void GraphAttr::print(std::ostream& pOS) const {
  pOS << "Graph " << value().name();
}

// VectorAttribute
template<> void BoolsAttr::print(std::ostream& pOS) const {
  bool first = true;
  pOS << '[';
  for (bool v : vector()) {
    if (!first) pOS << ", ";
    pOS << v;
    first = false;
  }
  pOS << ']';
}
template<> void FloatsAttr::print(std::ostream& pOS) const {
  bool first = true;
  pOS << '[';
  for (double v : vector()) {
    if (!first) pOS << ", ";
    pOS << v;
    first = false;
  }
  pOS << ']';
}
template<> void IntsAttr::print(std::ostream& pOS) const {
  bool first = true;
  pOS << '[';
  for (int64_t v : vector()) {
    if (!first) pOS << ", ";
    pOS << v;
    first = false;
  }
  pOS << ']';
}
template<> void StringsAttr::print(std::ostream& pOS) const {
  bool first = true;
  pOS << '[';
  for (const auto &v : vector()) {
    if (!first) pOS << ", ";
    pOS << '"' << v << '"';
    first = false;
  }
  pOS << ']';
}
template<> void TensorsAttr::print(std::ostream& pOS) const {
  bool first = true;
  pOS << '[';
  for (const auto &v : vector()) {
    if (!first) pOS << ", ";
    v.print(pOS);
    first = false;
  }
  pOS << ']';
}
template<> void GraphsAttr::print(std::ostream& pOS) const {
  bool first = true;
  pOS << '[';
  for (const auto &v : vector()) {
    if (!first) pOS << ", ";
    pOS << v.name();
    first = false;
  }
  pOS << ']';
}
