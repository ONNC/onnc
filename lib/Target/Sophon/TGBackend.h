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
#include "TGFuseOptimizer.h"
#include <memory>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Support/Path.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace onnc {

class TGBackend : public DLATargetBackend
{
public:
  typedef std::unordered_map<const onnc::Value *, onnc::ComputeMemOperand *>
    ValMemOpndMap;

public:
  TGBackend(const TargetOptions &pOptions);

  ~TGBackend() override = default;

  void addTensorSel(PassManager &pPM) override;

  void addCodeEmit(PassManager &pPM, const Path &pOutputFile) override;

  void addMemAlloc(PassManager &pPM) override;

  virtual std::unique_ptr<TGFuseOptimizer> getFuseOptimizr() = 0;

  // default sizeof function
  virtual size_t sizeOfTensorType(xTensorProtoDataType pType);

  // backend can descript which tensor types are supported
  virtual bool isNativeTensorType(xTensorProtoDataType pType);

  // calibration table name
  virtual std::string getCtableName() { return std::string(); }

  // load ctable from onnx meta data
  std::string getCtable(const Module &pModule);

  // parse textString to proto format and store in Backend
  virtual void setCtableProto(const std::string &pTextString);

  // for debug usage
  virtual std::string getBackendName() { return "TGBackend"; }

  using LowerPass_t = Pass *(*)(TGBackend *);
  void replaceTargetLower(LowerPass_t pFunc)
  {
    m_ReplaceTargetLower = pFunc;
    return;
  }

  ValMemOpndMap& getValMemOpndMap() { return m_ValMemOpndMap; }

  onnc::ComputeMemOperand* getMemOpndByValue(const onnc::Value* pVal);

protected:
  ValMemOpndMap m_ValMemOpndMap;

private:
  Path m_OutputPath;
  LowerPass_t m_ReplaceTargetLower;
};

} // namespace onnc

#endif // TARGET_TG_TG_BACKEND_H
