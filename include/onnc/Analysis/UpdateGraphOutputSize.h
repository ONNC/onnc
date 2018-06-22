//===- UpdateGraphOutputSize.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_UPDATE_GRAPH_OUTPUT_SIZE_H
#define ONNC_UPDATE_GRAPH_OUTPUT_SIZE_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

namespace onnc {

/** \class UpdateGraphOutputSize
 *  onnx framework didn't update output value's dimension, so this pass is
 *  used to fill correct output dimension for each output value.
 */
class UpdateGraphOutputSize : public ModulePass
{
public:
  static char ID;

public:
  UpdateGraphOutputSize();

  ReturnType runOnModule(Module &pModule) override;

  void setBatchSize(unsigned pBatchSize) { m_BatchSize = pBatchSize; }

private:
  void updateInputBatchSize(onnx::Graph *pGraph);
  void resetOutputValueInfo(onnx::Graph *pGraph);
  void updateReshapeOutputInfo(::onnx::Node *pNode);

private:
  unsigned m_BatchSize;
};

UpdateGraphOutputSize* CreateUpdateGraphOutputSizePass();

} // namespace of onnc

#endif
