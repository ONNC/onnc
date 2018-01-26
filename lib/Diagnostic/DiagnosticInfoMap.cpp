//===- DiagnosticInfoMap.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/DiagnosticInfoMap.h>
#include <onnc/Diagnostic/EngineFwd.h>
#include <onnc/Support/DataTypes.h>
#include <cassert>

using namespace onnc;

namespace onnc {
namespace diagnostic {

struct DiagStaticInfo
{
  unsigned int     id;
  Severity severity;
  uint16_t         desc_len;
  const char*      desc_str;
};

static bool operator<(const DiagStaticInfo& pLHS, const DiagStaticInfo& pRHS)
{
  return (pLHS.id < pRHS.id);
}

static const DiagStaticInfo DiagInfo[] = {
#define DIAG(ENUM, CLASS, MSG) { ::ENUM, CLASS, sizeof(MSG)-1, MSG },
#include "onnc/Diagnostic/DiagCommonKinds.inc"
#undef DIAG
  { 0, None, 0, 0 }
};

// DiagStaticInfoSize - the number of entries in the DiagInfo
static const unsigned int DiagInfoSize = sizeof(DiagInfo)/sizeof(DiagInfo[0])-1;

} // namespace of diagnostic
} // namespace of onnc

//===----------------------------------------------------------------------===//
// DiagnosticInfoMap
//===----------------------------------------------------------------------===//
StringRef diagnostic::InfoMap::description(unsigned int pID) const
{
  DiagStaticInfo key = { pID, None, 0, 0 };
  const DiagStaticInfo* info =
                       std::lower_bound(DiagInfo, DiagInfo + DiagInfoSize, key);

  assert((DiagInfo + DiagInfoSize) != info &&
         info->id == pID &&
         "cannot get correct information by id!");

  return StringRef(info->desc_str, info->desc_len);
}
