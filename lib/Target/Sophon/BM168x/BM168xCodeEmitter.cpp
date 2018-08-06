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
#include "BM168xCodeEmitter.h"
#include <cstdint>
#include <fstream>

#define DEBUG_TYPE "bm168x_code_emitter"
#include <onnc/Support/Debug.h>
using namespace onnc;

BM168xCodeEmitter::BM168xCodeEmitter(TGBackend *pBackend)
    : TGCodeEmitter(), m_pBackend(pBackend)
{
}

void BM168xCodeEmitter::encodeInstructions(std::ostream &pOS)
{
  // TODO refactor
  if (dynamic_cast<BM1680Backend *>(m_pBackend)) {
    DEBUG(dbgs() << "BM1680Backend BM168xCodeEmitter::encodeInstructions\n");

    // XXX: we use auto only when the type is keeping changing.
    // onnc part
    auto &instList = m_pBackend->getInsts();
    for (auto const &i : instList)
      i->emit();
    instList.clear();
    return;
  }
  // TODO BM1682
  assert(0);
}

void BM168xCodeEmitter::genRuntimeInfo(const xGraph *pOnnxGraph,
                                       std::ostream &pOS)
{
  // TODO: Generate runtime info for BM168x.
  return;
}
