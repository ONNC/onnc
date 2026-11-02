//===- NvDlaCollectConcatInfoPass.cpp -------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaCollectConcatInfoPass.h"

#include "NvDlaUtil.h"

#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>

#include <cstdlib>
#include <sstream>
#include <vector>

namespace onnc {

namespace {

// The operators whose NVDLA code asks for every feature-data address through
// CodeEmitVisitor::issueDlaAddr(tensor, ...), which adds a placed tensor's offset. Other operators
// (Softmax, Split, Transpose, Reshape, the RUBIK Concat, the network's input and output) take the
// start of a buffer, so they must not produce or read a placed tensor.
bool knowsPlacedTensors(const ComputeOperator* op)
{
  return op != nullptr && (isa<Conv>(op) || isa<Relu>(op) || isa<MaxPool>(op) || isa<AveragePool>(op) ||
                           isa<LRN>(op) || isa<Add>(op) || isa<Sum>(op) || isa<Mul>(op));
}

std::string describe(const ComputeOperator* op)
{
  return (op == nullptr) ? std::string("nothing") : op->name().str();
}

} // anonymous namespace

NvDlaCollectConcatInfoPass::NvDlaCollectConcatInfoPass(const NvDlaConstants& constants, NvDlaBackendMeta& meta) noexcept
  : NvDlaConstants{constants}
  , m_Meta(meta)
{}

Pass::ReturnType NvDlaCollectConcatInfoPass::runOnModule(Module& pModule)
{
  const bool hasRubik     = (CONFIG_SET != nvdla::ConfigSet::nv_small);
  bool       usePlacement = !hasRubik;
  if (const char* const mode = std::getenv("ONNC_NVDLA_CONCAT")) {
    if (std::string(mode) == "placement") {
      usePlacement = true;
    } else if (std::string(mode) == "rubik") {
      usePlacement = false;
    } else {
      errs() << "[NvDla] ONNC_NVDLA_CONCAT=" << mode << " is not 'rubik' or 'placement': ignored\n";
    }
  }
  if (!usePlacement) {
    return Pass::kModuleNoChanged;
  }
  // Placement relies on every producer and the Concat being emitted as DLA ops (their addresses go
  // through issueDlaAddr). With --op-mapper cpu_only/ping_pong some of them become EMU (CPU) ops,
  // which know nothing about placement, so leave the Concats to the engine mapping instead.
  if (m_Meta.m_OpEngineMapper.getMappingMethod() != NvDlaBackendMeta::OpEngineMapper::MappingMethod::dla_only) {
    outs() << "[NvDla] concat by placement disabled: --op-mapper is not dla_only\n";
    return Pass::kModuleNoChanged;
  }

  for (ComputeOperator& cm : *pModule.getRootComputeGraph()) {
    const Concat* const concat = dyn_cast<Concat>(&cm);
    if (concat == nullptr) {
      continue;
    }

    std::string reason;
    if (tryPlace(*concat, reason)) {
      continue;
    }

    const std::string name = concat->getOutput(0)->getName();
    if (hasRubik) {
      outs() << "[NvDla] concat " << name << " is not done by placement (" << reason << "): using RUBIK\n";
      continue;
    }

    std::ostringstream os;
    os << "Concat " << name << " cannot be compiled for nv_small: " << reason
       << ". nv_small has no RUBIK unit, so a Concat is only possible by placing its inputs side by side "
          "in one buffer";
    fatal(nvdla_exceed_hardware_limit) << os.str();
  }

  return Pass::kModuleNoChanged;
}

bool NvDlaCollectConcatInfoPass::tryPlace(const Concat& concat, std::string& reason)
{
  std::ostringstream why;

  const Tensor* const output    = concat.getOutput(0);
  const unsigned      numInputs = concat.getNumOfInputs();

  if (concat.getAxis().value() != 1 || output->getNumOfDimensions() != 4) {
    why << "it does not join 4-D tensors along the channel axis (axis=" << concat.getAxis().value() << ")";
    reason = why.str();
    return false;
  }

  // channels that one group ("surface") of the feature layout holds
  const unsigned channelsPerGroup = FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE;

  const NvDlaCubeInfo outputCube(*this, NVDLA_CUBE_FEATURE, output->dimension(0), output->dimension(1),
                                 output->dimension(2), output->dimension(3), 0, 0);

  std::vector<NvDlaBackendMeta::Offset> offsets;
  NvDlaBackendMeta::Offset              offset = 0;
  for (unsigned idx = 0; idx < numInputs; ++idx) {
    const Tensor* const input = concat.getInput(idx);

    if (input->getNumOfDimensions() != 4) {
      why << "input " << idx << " (" << input->getName() << ") is not a 4-D tensor";
      reason = why.str();
      return false;
    }

    // A layer writes whole groups of channels starting at a group boundary, so the next input can
    // only follow directly when this one ends on a group boundary.
    if (idx + 1 < numInputs && static_cast<unsigned>(input->dimension(1)) % channelsPerGroup != 0) {
      why << "input " << idx << " (" << input->getName() << ") has " << input->dimension(1)
          << " channels, and every input except the last needs a multiple of " << channelsPerGroup;
      reason = why.str();
      return false;
    }

    for (unsigned other = 0; other < idx; ++other) {
      if (concat.getInput(other) == input) {
        why << "input " << idx << " (" << input->getName() << ") is used twice";
        reason = why.str();
        return false;
      }
    }

    if (m_Meta.isPlaced(*input) || m_Meta.isReshaped(*input)) {
      why << "input " << idx << " (" << input->getName() << ") already shares the buffer of another tensor";
      reason = why.str();
      return false;
    }

    const ComputeOperator* const producer = getProducer(*input);
    if (!knowsPlacedTensors(producer)) {
      why << "input " << idx << " (" << input->getName() << ") is produced by " << describe(producer)
          << ", which cannot write into a shared buffer";
      reason = why.str();
      return false;
    }

    for (const auto& use : input->getUses()) {
      const ComputeOperator* const user = use.getUser();
      if (user != &concat && !knowsPlacedTensors(user)) {
        why << "input " << idx << " (" << input->getName() << ") is also read by " << describe(user)
            << ", which cannot read from a shared buffer";
        reason = why.str();
        return false;
      }
    }

    const NvDlaCubeInfo inputCube(*this, NVDLA_CUBE_FEATURE, input->dimension(0), input->dimension(1),
                                  input->dimension(2), input->dimension(3), 0, 0);
    if (inputCube.stride_line != outputCube.stride_line || inputCube.stride_surface != outputCube.stride_surface) {
      why << "input " << idx << " (" << input->getName() << ") does not have the output's width and height";
      reason = why.str();
      return false;
    }

    offsets.push_back(offset);
    offset += inputCube.size;
  }

  if (offset != outputCube.size) {
    why << "its inputs take " << offset << " bytes side by side, but the output buffer has " << outputCube.size;
    reason = why.str();
    return false;
  }

  for (unsigned idx = 0; idx < numInputs; ++idx) {
    m_Meta.markAsPlaced(*concat.getInput(idx), *output, offsets[idx]);
  }

  return true;
}

} // namespace onnc
