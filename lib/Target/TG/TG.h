//===- TG.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_H
#define TARGET_TG_TG_H
#include "TGBackend.h"
#include <onnc/Target/Target.h>
#include <string>

namespace onnc {

class ModulePass;
extern onnc::Target TheTGTarget;

unsigned int TGQuadrupleMatchFn(const Quadruple &pQuadruple);
ModulePass *createONNXFuseOptPass(TGBackend *pTarget);
ModulePass *createTGCodeEmitPass(TGBackend *pTarget,
                                 const std::string &pOutputFilename);
ModulePass *createTargetLoweringPass(TGBackend *pTarget);

} // namespace onnc

#endif // TARGET_TG_TG_H
