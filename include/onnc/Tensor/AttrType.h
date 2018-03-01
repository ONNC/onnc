//===- AttrType.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TENSOR_ATTRTYPE_H
#define ONNC_TENSOR_ATTRTYPE_H
#include <cstdint>
#include <string>
#include <vector>

namespace onnc {
namespace tensor {
namespace AttrType {

// TODO: Tensor type & Graph type
typedef int64_t INT;
typedef float FLOAT;
typedef std::string STRING;
class TENSOR {};
class GRAPH {};
typedef std::vector<INT> INTS;
typedef std::vector<FLOAT> FLOATS;
typedef std::vector<STRING> STRINGS;

} // namespace of AttrType
} // namespace of tensor
} // namespace of onnc

#endif
