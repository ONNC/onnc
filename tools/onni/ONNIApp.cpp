//===- ONNIApp.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNIApp.h"
#include "Interpreter.h"
#include <cstdlib>
#include <onnc/Target/TargetSelect.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/IR/Module.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Core/PassManager.h>
#include <onnc/ADT/Color.h>
#include <onnc/Support/IOStream.h>
#include <string>
#include <fstream>

extern "C" {
#include "onnc-runtime.h"
}

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNIApp
//===----------------------------------------------------------------------===//
ONNIApp::ONNIApp(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv),
    m_Options() {
  InitializeAllPlatforms();
  InitializeAllBackends();
}

ONNIApp::~ONNIApp()
{
}

int ONNIApp::run()
{
  onnc::onnx::Reader reader;
  Module module;
  SystemError err = reader.parse(options().model(), module);
  if (!err.isGood()) {
    // TODO: show error message
    return EXIT_FAILURE;
  }

  std::string error;
  std::string quadruple;
  options().quadruple().canonical(quadruple);
  const onnc::Target* target = TargetRegistry::Lookup(quadruple, error);
  if (nullptr == target) {
    errs() << Color::RED << "Error" << Color::RESET
           << ": can not found target `" << quadruple << "`: " << error
           << std::endl;
    return EXIT_FAILURE;
  }

  PassManager pm;
  TargetBackend* backend = target->createBackend(options().target());
  backend->addTensorSel(pm);
  //backend->addTensorSched(pm);
  // TODO: Fix this after the MemAlloc finished.
  // backend->addMemAlloc(pm);

  pm.run(module);

  // TODO: Refactor
  void *runtime_context = ONNC_RUNTIME_init_runtime();

  Interpreter interpreter(runtime_context);

  // XXX
  Interpreter::AddressTable &atable = interpreter.m_ATable;

  ::onnx::Graph *graph = module.getRootTensorGraph();
  // FIXME: Use onnc-runtime to handle input
  {
    ::onnx::TensorProto tensor;
    std::ifstream input_fin(options().input().native());
    tensor.ParseFromIstream(&input_fin);
    const std::string &raw_data_str = tensor.raw_data();
    ::onnx::Value *input_onnx_value = graph->inputs()[0];
    // XXX
    void *mem = new char[raw_data_str.size()];
    memcpy(mem, raw_data_str.data(), raw_data_str.size());
    atable[input_onnx_value] = mem;
  }
  // FIXME: Use onnc-runtime to handle weight
  {
    for(::onnx::Value *v : graph->inputs()) {
      if (atable.find(v) != atable.end()) continue;
      const ::onnx::Tensor &tensor = *graph->getInitializer(v->uniqueName());
      const std::string &raw_data_str = tensor.raw();
      // XXX
      void *mem = new char[raw_data_str.size()];
      memcpy(mem, raw_data_str.data(), raw_data_str.size());
      atable[v] = mem;
    }
  }

  // TODO: Use Compute IR.

  for (xNode *n : module.getRootTensorGraph()->nodes()) {
		errs() << "GOOOOOOOOOO: <" << n->kind().toString() << ">" << std::endl;
    interpreter.visit(n);
  }

  // FIXME: Use onnc-runtime to handle weight
  {
    for(::onnx::Value *v : graph->outputs()) {
      if (atable.find(v) == atable.end()) {
			  errs() << "error1 !!!!!" << v << "!!!!!" << std::endl;
			}
      const float *output = reinterpret_cast<const float *>(atable[v]);
      size_t size = 1;
      for (auto i: v->sizes()) {
        size *= i.dim;
      }
      errs() << '[';
      for (size_t i = 0; i < size; ++i) {
        errs() << output[i] << ", ";
      }
      errs() << ']' << std::endl;
    }
  }


  ONNC_RUNTIME_shutdown_runtime(runtime_context);

  //ComputeGraph *g = module.getRootComputeGraph();
  //errs() << g << std::endl;
  //NodeList::iterator node, nEnd = m_NodeList.end();
  //for (node = m_NodeList.begin(); node != nEnd; ++node)
  //  delete *node;
  //for (ComputeOperator &n : *module.getRootComputeGraph()) {
  //  errs() << &n << std::endl;
  //}
  //pm.run(interpreter);
  return EXIT_SUCCESS;
}
