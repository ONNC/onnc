//===- CLangMemInfoPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CLANGMEMINFO_PASS_H
#define ONNC_CLANGMEMINFO_PASS_H

#include "CLangMeta.h"
#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CLangMemInfoPass : public CustomPass<CLangMemInfoPass>
{
public:
  CLangMemInfoPass(CLangMeta& pMeta) noexcept;

  ReturnType runOnModule(Module& pModule) override;

private:
  CLangMeta& m_pMeta;
};

} // namespace onnc

#endif
