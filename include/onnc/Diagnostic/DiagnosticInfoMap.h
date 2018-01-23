//===- DiagnosticInfoMap.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_DIAGNOSTIC_INFOMAP_H
#define ONNC_DIAGNOSTIC_DIAGNOSTIC_INFOMAP_H
#include <onnc/ADT/StringRef.h>

namespace onnc {

enum ID {
#define DIAG(ENUM, CLASS, MSG) ENUM,
#include "onnc/Diagnostic/DiagCommonKinds.inc"
#undef DIAG
  NUM_OF_BUILDIN_DIAGNOSTIC_INFO
};

namespace diagnostic {

/** \class InfoMap
 *  \brief InfoMap caches run-time information of Diagnostic.
 */
class InfoMap
{
public:
  StringRef description(unsigned int pID) const;
};

} // namespace of diagnostic
} // namespace of onnc

#endif
