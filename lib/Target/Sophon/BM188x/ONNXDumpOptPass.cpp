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
#include "BM188xBackend.h"
#include <fstream>
#include <onnc/Config/ONNX.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>

using namespace onnc;
namespace {

class ONNXDumpOpt : public ModulePass
{
public:
  static char ID;

public:
  ONNXDumpOpt(BM1880Backend *pBackend) : ModulePass(ID), m_pBackend(pBackend) {}

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  BM1880Backend *m_pBackend; // NOLINT
};

class ONNXDumpQuantized : public ModulePass
{
public:
  static char ID;

public:
  ONNXDumpQuantized(BM1880Backend *pBackend)
      : ModulePass(ID), m_pBackend(pBackend)
  {
  }

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  BM1880Backend *m_pBackend; // NOLINT
};

} // namespace

char ONNXDumpOpt::ID = 0;
char ONNXDumpQuantized::ID = 0;

ModulePass *onnc::createONNXDumpOptPass(BM1880Backend *pBackend)
{
  return new ONNXDumpOpt(pBackend);
}

ModulePass *onnc::createONNXDumpQuantizedPass(BM1880Backend *pBackend)
{
  return new ONNXDumpQuantized(pBackend);
}

static void ExportONNX(const std::string &outputFileName, const Module &pModule)
{
  xProto modelProto;
  onnc::ExportModelProto(modelProto, pModule);
  std::fstream output(outputFileName,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  modelProto.SerializeToOstream(&output);
}

Pass::ReturnType ONNXDumpOpt::runOnModule(Module &pModule)
{
  std::string outputFile = m_pBackend->options().optOnnxModel();

  if (outputFile.empty()) {
    errs() << "error:" << __func__ << "\n";
    exit(1);
  }

  if (outputFile != std::string("-")) {
    pModule.getMetaData().erase("bm1880_ctable");

    ExportONNX(outputFile, pModule);
  }

  return Pass::kModuleNoChanged;
}

Pass::ReturnType ONNXDumpQuantized::runOnModule(Module &pModule)
{
  std::string outputFile = m_pBackend->options().optOnnxModel();

  if (outputFile.empty()) {
    errs() << "error:" << __func__ << "\n";
    exit(1);
  }

  if (outputFile == std::string("-")) {
    // export ctable for stdout.
    std::cout << m_pBackend->getBackendCtable().DebugString() << std::endl;
  } else {
    // export latest ctable
    pModule.getMetaData()["bm1880_ctable"] =
        m_pBackend->getBackendCtable().DebugString();

    ExportONNX("quantized-" + outputFile, pModule);
  }

  return Pass::kModuleNoChanged;
}
