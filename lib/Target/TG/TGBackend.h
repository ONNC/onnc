//===- TGBackend.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_BACKEND_H
#define TARGET_TG_TG_BACKEND_H
#include "ComputeOperator.h"
#include "TGCodeEmitter.h"
#include "TGFuseOptimizer.h"
#include "TargetLowering.h"
#include "GlobalMemAllocPass.h"
#include <memory>
#include <onnc/Support/Path.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <onnx/common/ir.h>
#include <string>
#include <vector>

namespace onnc {

class TargetLowering;
class TGCodeEmitter;

using MemTable = std::map<std::string, uint64_t>;

// FIXME: Move to an appropriate file.
const int GLOBAL_NEURON_TAG = 0x1;
const int GLOBAL_WEIGHT_TAG = 0x2;

class TGBackend : public DLATargetBackend
{
public:
  TGBackend(TargetLowering *pTLI, TGCodeEmitter *pCE,
            const TargetOptions &pOptions);

  ~TGBackend() override;

  void addTensorSel(PassManager &pPM) override;

  void addCodeEmit(PassManager &pPM, const Path &pOutputFile) override;

  void addMemAlloc(PassManager &pPM) override;

  std::vector<std::unique_ptr<ComputeOperator2> > &getInsts()
  {
    return m_Instructions;
  }

  std::vector<MemOperand *> &getMemOperands() { return m_MemOperands; }

  // get or create a MemOperand by onnx::Value. user can specify name because
  // different ONNX value can map to the same MemOperand
  MemOperand *getMemOperand(const ::onnx::Value *pValue, MemType pMemType,
                            const std::string &pName = std::string());

  const TargetOptions &getOption() { return m_Options; }

  TargetLowering *getTargetLowering() { return m_pTLI; }

  TGCodeEmitter *getTargetCodeEmitter() { return m_pCE; }

  virtual std::unique_ptr<TGFuseOptimizer> getFuseOptimizr() = 0;

  // default sizeof function
  virtual size_t sizeOfTensorType(::onnx::TensorProto_DataType pType);

  // backend can descript which tensor types are supported
  virtual bool isNativeTensorType(::onnx::TensorProto_DataType pType);

  // calibration table name
  virtual std::string getCtableName() { return std::string(); }

  // load ctable from onnx meta data
  std::string getCtable(const Module &pModule);

  // parse textString to proto format and store in Backend
  virtual void setCtableProto(const std::string &pTextString);

  // for debug usage
  virtual std::string getBackendName() { return std::string("TGBackend"); };

  using LowerPass_t = Pass *(*)(TGBackend *);
  void replaceTargetLower(LowerPass_t pFunc)
  {
    m_ReplaceTargetLower = pFunc;
    return;
  }

  LowerPass_t getTargetLower() const { return m_ReplaceTargetLower; }

private:
  std::vector<std::unique_ptr<ComputeOperator2> > m_Instructions;
  std::vector<MemOperand *> m_MemOperands;
  TargetLowering *m_pTLI; // NOLINT
  TGCodeEmitter *m_pCE;   // NOLINT
  Path m_OutputPath;
  TargetOptions m_Options;
  LowerPass_t m_ReplaceTargetLower;
};

} // namespace onnc

#endif // TARGET_TG_TG_BACKEND_H
