//===- NVDLAMeta.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CLangMeta.h"

#include <onnc/Diagnostic/MsgHandling.h>

#include <algorithm>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>

namespace onnc {

const std::string CLangMeta::weightFileExt{".weight"};
const std::string CLangMeta::serviceLibraryFileExt{".c"};

} // namespace onnc
