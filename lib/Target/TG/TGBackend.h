//===- TGBackend.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_BACKEND_H
#define TARGET_TG_TG_BACKEND_H
#include "ComputeOperand.h"
#include "TGCodeEmitter.h"
#include "TargetLowering.h"
#include <memory>
#include <onnc/Support/Path.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnx/common/ir.h>
#include <string>
#include <vector>

namespace onnc {

class TargetLowering;
class TGCodeEmitter;

using MemTable = std::map<std::string, uint64_t>;

class TGBackend : public DLATargetBackend
{
public:
  TGBackend(TargetLowering *pTLI, TGCodeEmitter *pCE,
            const TargetOptions &pOptions);

  virtual ~TGBackend();

  void codeEmit();

  void addTensorSel(PassManager &pPM) override;

  void addCodeEmit(PassManager &pPM, const Path &pOutputFile) override;

  void addMemAlloc(PassManager &pPM) override;

  std::vector<std::unique_ptr<ComputeOperand2> > &getInsts()
  {
    return m_instructions;
  }

  TargetLowering *getTargetLowering() { return m_pTLI; }

  // default sizeof function
  virtual size_t sizeOfTensorType(::onnx::TensorProto_DataType pType);

  // backend can descript which tensor types are supported
  virtual bool isNativeTensorType(::onnx::TensorProto_DataType pType);

  // calibration table name
  virtual std::string getCtableName() { return ""; }

  // load ctable from onx meta data
  const std::string &getCtable(const Module &pModule);

  // parse textString to proto format and store in Backend
  virtual void setCtableProto(const std::string &pTextString);

  // for debug usage
  virtual std::string getBackendName() {
    return "TGBackend";
  };

private:
  std::vector<std::unique_ptr<ComputeOperand2> > m_instructions;
  TargetLowering *m_pTLI;
  TGCodeEmitter *m_pCE;
  Path m_outputPath;
};

}  // namespace onnc

#endif  // TARGET_TG_TG_BACKEND_H
