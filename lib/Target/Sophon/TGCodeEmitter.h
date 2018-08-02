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
#ifndef ONNC_TARGET_TG_CODE_EMITTER_H
#define ONNC_TARGET_TG_CODE_EMITTER_H
#include <onnx/common/ir.h>
#include <vector>
#include <string>
#include <ostream>

namespace onnc {

class TGCodeEmitter
{
public:
  virtual ~TGCodeEmitter() = 0; //< pure interface

  virtual void encodeInstructions(::std::ostream &pOS) = 0;

  virtual void genWeightBin(const ::std::string &pOutputFilename) { return; }

  virtual void genRuntimeInfo(const ::onnx::Graph *pOnnxGraph,
                              std::ostream &pOS) = 0;
};

} // namespace onnc

#endif
