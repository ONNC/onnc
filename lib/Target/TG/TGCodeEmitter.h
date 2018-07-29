//===- TGCodeEmitter.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_CODE_EMITTER_H
#define ONNC_TARGET_TG_CODE_EMITTER_H
#include <onnx/common/ir.h>
#include <vector>
#include <string>
#include <ostream>

namespace onnc {

class TGBackend;

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
