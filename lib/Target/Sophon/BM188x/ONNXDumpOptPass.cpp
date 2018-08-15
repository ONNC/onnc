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
#include <onnc/Core/ModulePass.h>
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Config/ONNX.h>

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

} // namespace

char ONNXDumpOpt::ID = 0;

ModulePass *onnc::createONNXDumpOptPass(BM1880Backend *pBackend)
{
  return new ONNXDumpOpt(pBackend);
}

Pass::ReturnType ONNXDumpOpt::runOnModule(Module &pModule)
{
  // update ctable in module meta data
  pModule.getMetaData()["bm1880_ctable"] =
      m_pBackend->getBackendCtable().DebugString();

  std::string output_file = m_pBackend->options().optOnnxModel();

  if (output_file.empty()) {
    errs() << "error:" << __func__ << "\n";
    exit(1);
  }

  // read ctable from module and dump for debug
  if (output_file == std::string("-")) {
    // get ctable from module
    std::string ctable = m_pBackend->getCtable(pModule);
    // transfer ctable string into protobuf
    m_pBackend->setCtableProto(ctable);
    std::cout << m_pBackend->getBackendCtable().DebugString() << std::endl;
  } else {
    xProto modelProto;
    onnc::ExportModelProto(modelProto, pModule);
    std::fstream output(output_file,
                        std::ios::out | std::ios::trunc | std::ios::binary);
    modelProto.SerializeToOstream(&output);
  }
  return Pass::kModuleNoChanged;
}
