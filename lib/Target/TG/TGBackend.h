//===- TGBackend.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_BACKEND_H
#define TARGET_TG_TG_BACKEND_H
#include <string>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Support/Path.h>
#include <memory>
#include <onnx/common/ir.h>
#include <vector>
#include "Operator.h"
#include "TGISelLowering.h"
#include "TargetLowering.h"
#include "TGCodeEmitter.h"

namespace onnc {

class TGCodeEmitter;

using MemTable = std::map<std::string, uint64_t>;

class TGBackend : public DLATargetBackend
{
public:
  TGBackend(const TargetOptions &pOptions);

  virtual ~TGBackend();

  void codeEmit();

  void addTensorSel(PassManager &pPM);

  void addCodeEmit(PassManager& pPM, const Path& pOutputFile);

  MemTable &getMemLayout() { return m_globalMemLayout; }

  std::vector<std::unique_ptr<Operator> > &getInsts() { return m_instructions; }

  TargetLowering *getTargetLowering() { return m_pTLI; }

  // default sizeof function
  virtual size_t sizeOfTensorType(::onnx::TensorProto_DataType type);

  // backend can descript which tensor types are supported
  virtual bool isNativeTensorType(::onnx::TensorProto_DataType type);

  // for debug usage
  virtual std::string getBackendName() {
    return "TGBackend";
  };

private:
  std::vector<std::unique_ptr<Operator> > m_instructions;
  MemTable m_globalMemLayout;
  TargetLowering *m_pTLI;
  TGCodeEmitter *m_pCE;
  Path m_outputPath;
};

class BM1680Backend : public TGBackend
{
public:
  BM1680Backend(const TargetOptions& pOptions);

  virtual ~BM1680Backend();
  bool isNativeTensorType(::onnx::TensorProto_DataType type) override;
  std::string getBackendName() override{
    return "BM1680Backend";
  };
};

class BM1682Backend : public TGBackend
{
public:
  BM1682Backend(const TargetOptions& pOptions);

  virtual ~BM1682Backend();
  bool isNativeTensorType(::onnx::TensorProto_DataType type) override;
  std::string getBackendName() override{
    return "BM1682Backend";
  };
};

class BM1880Backend : public TGBackend
{
public:
  BM1880Backend(const TargetOptions& pOptions);

  virtual ~BM1880Backend();
  bool isNativeTensorType(::onnx::TensorProto_DataType type) override;
  std::string getBackendName() override{
    return "BM1880Backend";
  };
};

}  // namespace onnc

#endif  // TARGET_TG_TG_BACKEND_H
