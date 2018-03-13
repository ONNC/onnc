#include <cstring>
#include <cassert>
#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include "TGBackend.h"
#include "TGISelLowering.h"
#include "TGCodeEmitter.h"
#include "reader_helper.h"
#include "onnx/common/ir_pb_converter.h"
#include "onnx/optimizer/optimize.h"

TGBackend::TGBackend(const onnx::ModelProto &model)
    : m_TLI(new TGTargetLowering(this)), m_CE(new TGCodeEmitter(this)) {

  // test onnx opt passes
  std::cout << "before onnx IR optimization" << std::endl;
  onnx::optimization::Optimizer onnxOptimizer;
  std::vector<std::string> passNames{"eliminate_nop_transpose", "fuse_consecutive_transposes", "fuse_transpose_into_gemm"};
  auto optModel = onnxOptimizer.optimize(model, passNames);
  std::cout << "after onnx IR optimization" << std::endl;
  dumpONNXProto(optModel);

  // transfer pb to onnx ir
  m_onnxGraph = std::move(onnx::ImportModelProto(optModel));

  ::removeUnusedNodePass::removeUnusedNode(*const_cast<onnx::Graph *>(m_onnxGraph.get()));
  ::updateOutputInfoPass::updateOutputInfo(*const_cast<onnx::Graph *>(m_onnxGraph.get()));

  std::cout << "after ours IR passes" << std::endl;
  // TODO, add IR dumpper
  onnx::ModelProto lastModel;
  onnx::ExportModelProto(&lastModel, m_onnxGraph);
  dumpONNXProto(lastModel);

}

TGBackend::~TGBackend() {
  delete m_TLI;
  delete m_CE;
}

TGBackend &TGBackend::ddrAllocInfo(void) {
  ddrAllocInfo(*m_onnxGraph.get(), m_globalMemLayout);
  return *this;
}

void TGBackend::ddrAllocInfo(onnx::Graph &graph, MemTable &memTable) {
  // Definition fom BM168xBackendContext.hpp
  // TAG will be masked by runtime while processing cmdbuf.
  const int GLOBAL_NEURON_TAG = 0x1;
  const int GLOBAL_WEIGHT_TAG = 0x2;
  const int GLOBAL_ARM_TAG = 0x3;

  // allocate spaces for weight
  unsigned int weight_offset = 0;
  // BMKernel only supports DATA_FMT_F32 & DATA_FMT_I1
  int F32_SIZE = 4;
  std::string tab = "\t";

  std::cout << __func__ << " dump global memory layout:" << std::endl;

  for (auto i : graph.initializers()) {

    memTable[i.name()] = weight_offset + GLOBAL_WEIGHT_TAG;
    std::cout << tab << i.name() << " = " << weight_offset;

    assert(i.elem_type() == onnx::TensorProto_DataType_FLOAT);
    if (i.sizes().size() > 0) {
      int tensor_size = F32_SIZE;
      std::cout << " <";
      for (auto dim : i.sizes()) {
        std::cout << dim << ",";
        tensor_size *= dim;
      }
      std::cout << ">" << std::endl;
      weight_offset += tensor_size;
    } else {
      std::cout << std::endl;
    }
  }

  unsigned int neuron_offset = 0;
  std::unordered_set<std::string> initNames(graph.initializer_names().begin(),
                                            graph.initializer_names().end());
  // allocate space for inputs
  for (auto i : graph.inputs()) {
    if (0 == initNames.count(i->uniqueName())) {

      memTable[i->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      std::cout << tab << i->uniqueName() << " = " << neuron_offset;

      assert(i->elemType() == onnx::TensorProto_DataType_FLOAT);
      if (i->sizes().size() > 0) {
        int tensor_size = F32_SIZE;
        std::cout << " <";
        for (auto &dim : i->sizes()) {
          std::cout << dim.dim << ",";
          tensor_size *= dim.dim;
        }
        std::cout << ">" << std::endl;
        neuron_offset += tensor_size;
      } else {
        std::cout << std::endl;
      }
    }
  }
  // allocate space for outputs
  for (auto i : graph.nodes()) {
    if (i->kind() == onnx::Symbol("Undefined"))
      continue;

    for (auto o : i->outputs()) {

      memTable[o->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      std::cout << tab << o->uniqueName() << " = " << neuron_offset;

      // FIXME: remove this after output dimension is fixed
      assert(o->elemType() == onnx::TensorProto_DataType_FLOAT);
      if (o->sizes().size() > 0) {
        int tensor_size = F32_SIZE;
        std::cout << " <";
        for (auto dim : o->sizes()) {
          std::cout << dim.dim << ",";
          tensor_size *= dim.dim;
        }
        std::cout << ">" << std::endl;
        neuron_offset += tensor_size;
      } else {
        std::cout << std::endl;
      }
    }
  }
  std::cout << tab << "weight size: " << weight_offset << std::endl;
  std::cout << tab << "neuron size: " << neuron_offset << std::endl;
}

void TGBackend::codeEmit(void) {
  m_CE->encodeInstructions();
}

TGBackend &TGBackend::lowering(void) {
  m_TLI->CodeGenAndEmitInst(m_onnxGraph.get(), m_instructions);
  return *this;
}

