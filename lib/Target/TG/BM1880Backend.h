//===- BM1880Backend.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef BM1880_BACKEND_H
#define BM1880_BACKEND_H
#include "TGBackend.h"
#include <memory>
#include <onnx/common/ir.h>
#include <string>

namespace onnc {

class TGCodeEmitter;

using MemTable = std::map<std::string, uint64_t>;

class BM1880Backend : public TGBackend
{
public:
  BM1880Backend(const TargetOptions &pOptions);
  virtual ~BM1880Backend() = default;
  bool isNativeTensorType(::onnx::TensorProto_DataType pType) override;
  std::string getBackendName() override { return "BM1880Backend"; };
  std::string getCtableName() override { return "bm1880_ctable"; }
};

} // namespace onnc

#endif // BM1880_BACKEND_H
