//===- TGBackend.h ----------------------------------------------------===//
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
  TGBackend(const CompilerConfig &pConfig);

  virtual ~TGBackend();

  void codeEmit();

  void addCodeEmit(PassManager& pPM);

  MemTable &getMemLayout() { return m_globalMemLayout; }

  std::vector<std::unique_ptr<Operator> > &getInsts() { return m_instructions; }

  TargetLowering *getTargetLowering() { return m_pTLI; }

private:
  std::vector<std::unique_ptr<Operator> > m_instructions;
  MemTable m_globalMemLayout;
  TargetLowering *m_pTLI;
  TGCodeEmitter *m_pCE;

};

class BM1680Backend : public TGBackend
{
public:
  BM1680Backend(const CompilerConfig& pConfig);

  virtual ~BM1680Backend();
};

class BM1682Backend : public TGBackend
{
public:
  BM1682Backend(const CompilerConfig& pConfig);

  virtual ~BM1682Backend();
};

}  // namespace onnc

#endif  // TARGET_TG_TG_BACKEND_H
