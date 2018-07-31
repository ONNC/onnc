//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
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
ModulePass *CreateTGCodeEmitPass(TGBackend *pTarget,
                                 const std::string &pOutputFilename);
ModulePass *createTargetLoweringPass(TGBackend *pTarget);

} // namespace onnc

#endif // TARGET_TG_TG_H
