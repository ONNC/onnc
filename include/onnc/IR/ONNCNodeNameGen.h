//===- ONNCNodeNameGen.h --------------------------------------------------===//

//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NODE_NAME_GEN_H
#define ONNC_NODE_NAME_GEN_H

#include <onnc/Core/ModulePass.h>

namespace onnc {
ModulePass *createONNCNodeNameGenPass();
}

#endif // ONNC_NODE_NAME_GEN_H
