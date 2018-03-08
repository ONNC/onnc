#pragma once

#include <cstdint>
#include <memory>
#include <onnx/common/ir.h>
#include <onnx/onnx_pb.h>
#include <vector>
#include "TGOperator.h"

#define CMD_BUF_NAME "cmdbuf.bin"

class TGBackend {
public:
  TGBackend(const onnx::ModelProto &model);
  ~TGBackend();
  TGBackend &lowering(void);
  void codeEmit(void);

private:
  static void sendCmdBuf(void *userData, const void *cmdBuf, uint32_t len);
  static void emitCmdBuf(void *userData, void *cmdBuf, uint32_t len);
  static void freeCmdBuf(void *userData, void *cmdBuf);
  static void *allocCmdBuf(void *userData, uint32_t size);
  static void hostSync(void);
  static void emitDebugInfo(void *userData, char const *info, int nodeId,
                            long long unsigned int fwAddr, bool isFloat);
  void bmkernelContextPrepare(void);

  void *m_bmkernelHandle;
  std::unique_ptr<onnx::Graph> m_onnxGraph;
  std::vector<std::unique_ptr<TGOperator>> m_instructions;
};
