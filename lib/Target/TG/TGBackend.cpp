//===- TGBackend.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TG.h"
#include "TGBackend.h"
#include "TargetInfo/TGTargetInfo.h"
#include <onnc/Transforms/removeUnusedNodes.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TGBackend
//===----------------------------------------------------------------------===//
TGBackend::TGBackend(const TargetOptions &pOptions)
  : DLATargetBackend(pOptions),
    m_pTLI(new TGTargetLowering(this)),
    m_pCE(new TGCodeEmitter(this)) {
}

TGBackend::~TGBackend()
{
  delete m_pTLI;
  delete m_pCE;
}

void TGBackend::addTensorSel(PassManager &pPM)
{
  // IR level pass
  pPM.add(createRemoveUnusedNodesPass());
  pPM.add(createUpdateOutputInfoPass());
  // TGbackend require memory allocation before TensorSel (lowering)
  pPM.add(createTGMemAllocInfoPass(this));
  pPM.add(createTargetLoweringPass(this));
  return;
}

void TGBackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  m_outputPath = pOutput;
  pPM.add(createTGCodeEmitPass(this));
}

void TGBackend::codeEmit(void)
{
  m_pCE->encodeInstructions(m_outputPath);
}

// BM1680
BM1680Backend::BM1680Backend(const TargetOptions &pOptions)
    : TGBackend(pOptions)
{
}

BM1680Backend::~BM1680Backend()
{
}

// BM1682
BM1682Backend::BM1682Backend(const TargetOptions &pOptions)
    : TGBackend(pOptions)
{
}

BM1682Backend::~BM1682Backend()
{
}

// BM1880
BM1880Backend::BM1880Backend(const TargetOptions &pOptions)
    : TGBackend(pOptions)
{
}

BM1880Backend::~BM1880Backend() {}

//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateTGBM1680Backend(const TargetOptions& pOptions)
{
  return new BM1680Backend(pOptions);
}

TargetBackend* CreateTGBM1682Backend(const TargetOptions& pOptions)
{
  return new BM1682Backend(pOptions);
}

TargetBackend* CreateTGBM1880Backend(const TargetOptions& pOptions)
{
  return new BM1880Backend(pOptions);
}

extern "C" void InitializeTGONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1680Target(),
      CreateTGBM1680Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1682Target(),
      CreateTGBM1682Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1880Target(),
      CreateTGBM1880Backend);
}
