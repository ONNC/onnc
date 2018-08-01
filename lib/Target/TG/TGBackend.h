//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_BACKEND_H
#define TARGET_TG_TG_BACKEND_H
#include "ComputeOperator.h"
#include "TGCodeEmitter.h"
#include "TGFuseOptimizer.h"
#include "GlobalMemAllocPass.h"
#include <memory>
#include <onnc/Support/Path.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnx/common/ir.h>
#include <string>
#include <unordered_map>
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
  typedef std::vector<std::unique_ptr<ComputeOperator2> > Instructions;
  typedef std::vector<MemOperand *> MemOperands;
  typedef std::unordered_map<const onnc::Value *, onnc::ComputeMemOperand *>
    ValMemOpndMap;

public:
  TGBackend(TargetLowering *pTLI, TGCodeEmitter *pCE, Instructions& pInsns,
            const TargetOptions &pOptions);

  ~TGBackend() override;

  void addTensorSel(PassManager &pPM) override;

  void addCodeEmit(PassManager &pPM, const Path &pOutputFile) override;

  void addMemAlloc(PassManager &pPM) override;

  std::vector<MemOperand *> &getMemOperands() { return m_MemOperands; }

  // get or create a MemOperand by onnx::Value. user can specify name because
  // different ONNX value can map to the same MemOperand
  MemOperand *getMemOperand(const ::onnx::Value *pValue, MemType pMemType,
                            const std::string &pName = std::string());

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

  Instructions& getInsts() { return m_Instructions; }

  ValMemOpndMap& getValMemOpndMap() { return m_ValMemOpndMap; }

  onnc::ComputeMemOperand* getMemOpndByValue(const onnc::Value* pVal);

protected:
  Instructions& m_Instructions;
  MemOperands m_MemOperands;
  ValMemOpndMap m_ValMemOpndMap;

private:
  TargetLowering *m_pTLI; // NOLINT
  TGCodeEmitter *m_pCE;   // NOLINT
  Path m_OutputPath;
  LowerPass_t m_ReplaceTargetLower;
};

} // namespace onnc

#endif // TARGET_TG_TG_BACKEND_H
