//===- LegalizePadPass.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LegalizePadPass.h"

#include "NvDlaDefine.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/LpPool.h>
#include <onnc/IR/Compute/Pad.h>

namespace onnc {
namespace nvdla {
//===----------------------------------------------------------------------===//
// LegalizePadPass
//===----------------------------------------------------------------------===//

Pass::ReturnType LegalizePadPass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = kModuleNoChanged;

  ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType LegalizePadPass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  // Save all Pad operators into vector
  std::vector<Pad*> pads_vector;
  for (auto& op : pCG) {
    if (!isa<Pad>(op)) {
      continue;
    }

    Pad* pad = cast<Pad>(&op);
    
    if (!is_removable_pad(pad)) {
      continue;
    }

    // Only these four operators have pads attribute
    if (is_followed_by_averagepool(pad) || is_followed_by_conv(pad) || is_followed_by_maxpool(pad) || is_followed_by_lppool(pad)) {
      pads_vector.push_back(pad);
    }
  }

  for (Pad* op : pads_vector) {
    remove_pad(op, pCG);
    ret |= Pass::kModuleChanged;
  }

  if (ret != kModuleNoChanged) {
    pCG.topologicalSort();
  }

  return ret;
}

template<typename OperatorType>
void LegalizePadPass::set_pads_impl(OperatorType* op, Pad* pad) {
  IntsAttr::VectorType op_pads = op->getPads().vector();
  IntsAttr::VectorType pad_pads = pad->getPads().vector();
  op->setPads({op_pads[0] + pad_pads[2], op_pads[1] + pad_pads[3], op_pads[2] + pad_pads[6], op_pads[3] + pad_pads[7]});
}

void LegalizePadPass::remove_pad(Pad* pad, ComputeGraph& pCG)
{
  Tensor* output_tensor = pad->getOutput(0);

  // Set input tensor as the next operator's input
  assert(output_tensor->getUses().size() == 1);

  ComputeOperator* next_op = output_tensor->getUses()[0].getUser();

  // Set next operator's pads with Pad's value
  if (AveragePool* averagepool = dyn_cast<AveragePool>(next_op)) {
    set_pads_impl(averagepool, pad);
  }
  else if (Conv* conv = dyn_cast<Conv>(next_op)) {
    set_pads_impl(conv, pad);
  }
  else if (MaxPool* maxpool = dyn_cast<MaxPool>(next_op)) {
    set_pads_impl(maxpool, pad);
  }
  else if (LpPool* lppool = dyn_cast<LpPool>(next_op)) {
    set_pads_impl(lppool, pad);
  }

  next_op->replaceInput(0, *pad->getInput(0));
  
  // Delete edges
  pad->removeAllInputs();
  pad->removeAllOutputs();

  // Delete unused tensor
  pCG.erase(*output_tensor);

  // Delete node
  pCG.erase(*pad);
}

template<typename OperatorType>
bool LegalizePadPass::is_valid_pads(OperatorType* op, Pad* pad, PadsType upper_bound) {
  IntsAttr::VectorType old_pads = op->getPads().vector();
  IntsAttr::VectorType new_pads = pad->getPads().vector();

  // Check pads size
  if (old_pads.size() != 4 || new_pads.size() != 8) {
    return false;
  }

  // Check pads values
  if ((old_pads[0] + new_pads[2] <= upper_bound) && 
      (old_pads[1] + new_pads[3] <= upper_bound) && 
      (old_pads[2] + new_pads[6] <= upper_bound) && 
      (old_pads[3] + new_pads[7] <= upper_bound)) {
    return true;
  }
  else {
    return false;
  }
}

bool LegalizePadPass::is_removable_pad(Pad* pad) {
  // Input dimension should in the format of NxCxHxW
  if (!(pad->getInput(0)->getNumOfDimensions() == 4)) {
    return false;
  }

  // Output dimension should in the format of NxCxHxW
  if (!(pad->getOutput(0)->getNumOfDimensions() == 4)) {
    return false;
  }

  // Pad's Attributes: 'mode' should be "constant"
  if (!(pad->getMode().value() == StringAttr::ValueType("constant"))) {
    return false;
  }

  // Pad's Attributes: 'pads' should be in the format of [0, 0, pad_top, pad_left, 0, 0, pad_bottom, pad_right]
  IntsAttr::VectorType pads = pad->getPads().vector();
  if (!(pads.size() == 8)) {
    return false;
  }
  if (!(pads[0] == 0 && pads[1] == 0 && pads[4] == 0 && pads[5] == 0)) {
    return false;
  }

  // Pad's Attributes: 'value' should be 0
  if (!(pad->getValue().value() == FloatAttr::ValueType(0))) {
    return false;
  }

  // The pattern matches
  return true;
}

bool LegalizePadPass::is_followed_by_averagepool(Pad* pad)
{
  // The output tensor of the Pad has only one user.
  if (!(pad->getOutput(0)->getUses().size() == 1)) {
    return false;
  }

  // This user is AveragePool
  ComputeOperator* next_op = pad->getOutput(0)->getUses()[0].getUser();
  AveragePool* averagepool = dyn_cast<AveragePool>(next_op);
  if (!(averagepool)) {
    return false;
  }

  // Find setting of upper bound
  auto find_p = std::find_if(m_backend_setting.pads_limit.begin(), m_backend_setting.pads_limit.end(), 
    [](const std::pair<OperatorWithPads, PadsType>& p) {
      if (p.first == OperatorWithPads::AveragePool) {
        return true;
      }
    }
  );

  // Cannot find setting for this operator, so we cannot handle it.
  if (find_p == m_backend_setting.pads_limit.end()){
    return false;
  }

  // The original padding of this AveragePool plus the additional padding from Pad must be smaller than its upper bound.
  if (!(is_valid_pads(averagepool, pad, (*find_p).second))) {
    return false;
  }
  
  // The pattern matches, so we need to remove this pad
  return true;
}

bool LegalizePadPass::is_followed_by_conv(Pad* pad)
{
  // The output tensor of the Pad has only one user.
  if (!(pad->getOutput(0)->getUses().size() == 1)) {
    return false;
  }

  // This user is Conv
  ComputeOperator* next_op = pad->getOutput(0)->getUses()[0].getUser();
  Conv* conv = dyn_cast<Conv>(next_op);
  if (!(conv)) {
    return false;
  }

  // Find setting of upper bound
  auto find_p = std::find_if(m_backend_setting.pads_limit.begin(), m_backend_setting.pads_limit.end(), 
    [](const std::pair<OperatorWithPads, PadsType>& p) {
      if (p.first == OperatorWithPads::Conv) {
        return true;
      }
    }
  );

  // Cannot find setting for this operator, so we cannot handle it.
  if (find_p == m_backend_setting.pads_limit.end()){
    return false;
  }

  // The original padding of this Conv plus the additional padding from Pad must be smaller than its upper bound.
  if (!(is_valid_pads(conv, pad, (*find_p).second))) {
    return false;
  }

  // Pad's output should match Conv's X
  if (!(pad->getOutput(0) == conv->getX())) {
    return false;
  }

  // The pattern matches, so we need to remove this pad
  return true;
}

bool LegalizePadPass::is_followed_by_maxpool(Pad* pad)
{
  // The output tensor of the Pad has only one user.
  if (!(pad->getOutput(0)->getUses().size() == 1)) {
    return false;
  }

  // This user is MaxPool
  ComputeOperator* next_op = pad->getOutput(0)->getUses()[0].getUser();
  MaxPool* maxpool = dyn_cast<MaxPool>(next_op);
  if (!(maxpool)) {
    return false;
  }

  // Find setting of upper bound
  auto find_p = std::find_if(m_backend_setting.pads_limit.begin(), m_backend_setting.pads_limit.end(), 
    [](const std::pair<OperatorWithPads, PadsType>& p) {
      if (p.first == OperatorWithPads::MaxPool) {
        return true;
      }
    }
  );

  // Cannot find setting for this operator, so we cannot handle it.
  if (find_p == m_backend_setting.pads_limit.end()){
    return false;
  }

  // The original padding of this MaxPool plus the additional padding from Pad must be smaller than its upper bound.
  if (!(is_valid_pads(maxpool, pad, (*find_p).second))) {
    return false;
  }

  // The pattern matches, so we need to remove this pad
  return true;
}

bool LegalizePadPass::is_followed_by_lppool(Pad* pad)
{
  // The output tensor of the Pad has only one user.
  if (!(pad->getOutput(0)->getUses().size() == 1)) {
    return false;
  }

  // This user is LpPool
  ComputeOperator* next_op = pad->getOutput(0)->getUses()[0].getUser();
  LpPool* lppool = dyn_cast<LpPool>(next_op);
  if (!(lppool)) {
    return false;
  }

  // Find setting of upper bound
  auto find_p = std::find_if(m_backend_setting.pads_limit.begin(), m_backend_setting.pads_limit.end(), 
    [](const std::pair<OperatorWithPads, PadsType>& p) {
      if (p.first == OperatorWithPads::LpPool) {
        return true;
      }
    }
  );

  // Cannot find setting for this operator, so we cannot handle it.
  if (find_p == m_backend_setting.pads_limit.end()){
    return false;
  }

  // The original padding of this LpPool plus the additional padding from Pad must be smaller than its upper bound.
  if (!(is_valid_pads(lppool, pad, (*find_p).second))) {
    return false;
  }

  // The pattern matches, so we need to remove this pad
  return true;
}

} // namespace nvdla
} // namespace onnc
