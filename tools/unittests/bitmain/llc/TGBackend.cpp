#include <bm_kernel.h>
#include <bmkernel_api.h>
#include <cstring>
#include <cassert>
#include <fstream>
#include <host_util.h>
#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include "TGBackend.h"
#include "reader_helper.h"
#include "onnx/common/ir_pb_converter.h"
#include "onnx/optimizer/optimize.h"

static void dumpONNXIR(const onnx::ModelProto &model) {

  // dump model info
  if (model.has_ir_version()) {
    std::cout << "model.ir_version: " << model.ir_version() << std::endl;
  }
  if (model.has_producer_name()) {
    std::cout << "model.producer_name: " << model.producer_name() << std::endl;
  }
  if (model.has_producer_version()) {
    std::cout << "model.producer_version: " << model.producer_version()
              << std::endl;
  }
  if (model.has_domain()) {
    std::cout << "model.domain: " << model.domain() << std::endl;
  }
  if (model.has_model_version()) {
    std::cout << "model.model_version: " << model.model_version() << std::endl;
  }
  if (model.has_doc_string()) {
    std::cout << "model.doc_string: " << model.doc_string() << std::endl;
  }

  if (!model.has_graph()) {
    return;
  }

  // dump graph
  const onnx::GraphProto &graph = model.graph();

  std::cout << "graph " << graph.name() << " (";

  for (int i = 0; i < graph.input_size(); i++) {
    if (i != 0) {
      std::cout << ", ";
    }
    std::cout << '%' << graph.input(i).name();
  }
  std::cout << ") {" << std::endl;

  for (int i = 0; i < graph.initializer_size(); i++) {
    const onnx::TensorProto &initializer = graph.initializer(i);
    std::cout << "  initialize input %" << initializer.name()
              << " = data tensor<";
    for (int j = 0; j < initializer.dims_size(); j++) {
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << initializer.dims(j);
    }
    std::cout << '>' << std::endl;
  }

  for (int i = 0; i < graph.node_size(); i++) {
    const onnx::NodeProto &node = graph.node(i);
    std::cout << "  ";
    for (int j = 0; j < node.output_size(); j++) {
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << '%' << node.output(j);
    }
    std::cout << " = ";
    std::cout << node.op_type();
    if (node.attribute_size() != 0) {
      std::cout << '<';
    }
    for (int j = 0; j < node.attribute_size(); j++) {
      const onnx::AttributeProto &attribute = node.attribute(j);
      if (j != 0) {
        std::cout << ", ";
      }
      switch (attribute.type()) {
      case onnx::AttributeProto::FLOAT:
        std::cout << attribute.f();
        break;
      case onnx::AttributeProto::INT:
        std::cout << attribute.i();
        break;
      case onnx::AttributeProto::STRING:
        std::cout << attribute.s();
        break;
      case onnx::AttributeProto::TENSOR:
      case onnx::AttributeProto::GRAPH:
        std::cout << attribute.name();
        break;
      case onnx::AttributeProto::FLOATS:
        std::cout << '[';
        for (int k = 0; k < attribute.floats_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.floats(k);
        }
        std::cout << ']';
        break;
      case onnx::AttributeProto::INTS:
        std::cout << '[';
        for (int k = 0; k < attribute.ints_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.ints(k);
        }
        std::cout << ']';
        break;
      case onnx::AttributeProto::STRINGS:
        std::cout << '[';
        for (int k = 0; k < attribute.strings_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.strings(k);
        }
        std::cout << ']';
        break;
      case onnx::AttributeProto::TENSORS:
      case onnx::AttributeProto::GRAPHS:
        std::cout << attribute.name();
        break;
      default:
        std::cout << "unknown";
      }
    }
    if (node.attribute_size() != 0) {
      std::cout << '>';
    }
    std::cout << '(';
    for (int j = 0; j < node.input_size(); j++) {
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << '%' << node.input(j);
    }
    std::cout << ')' << std::endl;
  }

  std::cout << "  return ";
  // unordered_map<std::string, std::string> output_type;
  for (int i = 0; i < graph.output_size(); i++) {
    if (i != 0) {
      std::cout << ", ";
    }
    std::cout << ConvertTypeToString(graph.output(i).type()) << " %"
              << graph.output(i).name();
  }
  std::cout << std::endl;
  std::cout << '}' << std::endl;
}

void TGBackend::sendCmdBuf(void *userData, const void *cmdBuf, uint32_t len) {
  std::cout << __func__ << std::endl;
  std::cout << "save to " << CMD_BUF_NAME << std::endl;
  std::fstream output(CMD_BUF_NAME,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  output.write((char *)cmdBuf, len);
}

void TGBackend::emitCmdBuf(void *userData, void *cmdBuf, uint32_t len) {
  sendCmdBuf(userData, cmdBuf, len);
  /* free implicitly */
  free(cmdBuf);
}

void TGBackend::freeCmdBuf(void *userData, void *cmdBuf) {
  /* free without emit */
  emitCmdBuf(userData, cmdBuf, 0);
}

void *TGBackend::allocCmdBuf(void *userData, uint32_t size) {
  void *cmdBuf = malloc(size);
  return cmdBuf;
}

void TGBackend::hostSync(void) {}

void TGBackend::emitDebugInfo(void *userData, char const *info, int nodeId,
                              long long unsigned int fwAddr, bool isFloat) {}

void TGBackend::bmkernelContextPrepare(void) {
  bmkernel_info_t bmkernelInfo;
  hw_info_t hwInfo;

  memcpy(&hwInfo, get_hw_info(), sizeof(hw_info_t));
  std::cout << "default hw_info:" << std::endl;
  std::cout << "  chip_version " << hwInfo.chip_version << std::endl
            << "  nodechip_shift " << hwInfo.nodechip_shift << std::endl
            << "  npu_shift " << hwInfo.npu_shift << std::endl << "  eu_shift "
            << hwInfo.eu_shift << std::endl << "  local_mem_shift "
            << hwInfo.local_mem_shift << std::endl << "  local_mem_banks "
            << hwInfo.local_mem_banks << std::endl << "  global_mem_size 0x"
            << std::hex << hwInfo.global_mem_size << std::endl
            << "  nodechip_num " << hwInfo.nodechip_num << std::endl
            << "  npu_num " << hwInfo.npu_num << std::endl << "  eu_num "
            << hwInfo.eu_num << std::endl << "  local_mem_size 0x" << std::hex
            << hwInfo.local_mem_size << std::endl;

  bmkernelInfo.chip_version = hwInfo.chip_version;
  bmkernelInfo.eu_shift = hwInfo.eu_shift;
  bmkernelInfo.npu_shift = hwInfo.npu_shift;
  bmkernelInfo.node_shift = hwInfo.nodechip_shift;
  bmkernelInfo.local_mem_shift = hwInfo.local_mem_shift;
  bmkernelInfo.local_mem_banks = hwInfo.local_mem_banks;
  bmkernelInfo.cmd_buf_size = 0x10000000;
  bmkernelInfo.emit_on_update = false;
  bmkernelInfo.alloc_cmd_buf = TGBackend::allocCmdBuf;
  bmkernelInfo.emit_cmd_buf = TGBackend::emitCmdBuf;
  bmkernelInfo.free_cmd_buf = TGBackend::freeCmdBuf;
  bmkernelInfo.sync = TGBackend::hostSync;
  bmkernelInfo.debug = TGBackend::emitDebugInfo;

  bmkernel_register(&bmkernelInfo, BMKERNEL_NODE_ID_HOST, (void *)0,
                    &m_bmkernelHandle);
  std::cout << "bmkernel_register done" << std::endl;
}

TGBackend::TGBackend(const onnx::ModelProto &model) : m_bmkernelHandle(nullptr) {

 // test onnx opt passes
 std::cout << "before onnx IR optimization" << std::endl;
 onnx::optimization::Optimizer onnxOptimizer;
 auto mirrorModel = std::make_unique<onnx::ModelProto>(model);
 dumpONNXIR(*mirrorModel);
 std::vector<std::string> passNames{"eliminate_nop_transpose", "fuse_consecutive_transposes", "fuse_transpose_into_gemm"};
 auto optModel = onnxOptimizer.optimize(std::move(mirrorModel), passNames);
 std::cout << "after onnx IR optimization" << std::endl;
 dumpONNXIR(*optModel);

  // transfer pb to onnx ir
  m_onnxGraph = std::move(onnx::ImportModelProto(*optModel));
}

TGBackend::~TGBackend() { kernel_exit(); }

void TGBackend::codeEmit(void) {
  // init bm1680 context
  bmkernelContextPrepare();
  kernel_enter(m_bmkernelHandle);
  for (auto const &i : m_instructions) {
    i->emit();
  }
  m_instructions.clear();
  kernel_submit();
}

TGBackend &TGBackend::lowering(void) {
  uint64_t offset = 0;
  for (auto it = m_onnxGraph->begin(), ie = m_onnxGraph->end(); it != ie;
       ++it) {
    const onnx::Node *const node = *it;
    std::unique_ptr<TGOperator> tgOp(TGOperator::makeTGOperator(*node, offset));
    // FIXME walkaound for Dropout node
    if (nullptr == tgOp)
      continue;
    assert(nullptr != tgOp);
    std::cout << "lowering: " << tgOp->getName() << std::endl;
    offset += tgOp->getTotalSize();
    m_instructions.push_back(std::move(tgOp));
  }
  return *this;
}
