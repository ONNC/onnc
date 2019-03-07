//===- ONNIApp.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNIApp.h"

#include "CountOperatorsPass.h"
#include "InterpreterPass.h"

#include <onnc/ADT/Color.h>
#include <onnc/Config/ONNX.h>
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
#include <onnc/Analysis/Counter.h>
#include <onnc/Transforms/OnnxOptPass.h>

#include <fstream>
#include <iomanip>
#include <memory>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNIApp
//===----------------------------------------------------------------------===//
namespace onnc {
namespace internal {
  class TensorReadProxy
  {
  public:
    TensorReadProxy() noexcept
      : m_Success{false}
      , m_Data{}
      , m_Length{0}
    { }

    TensorReadProxy(std::unique_ptr<char[]> data, std::size_t length) noexcept
      : m_Success{true}
      , m_Data{std::move(data)}
      , m_Length{length}
    { }

    TensorReadProxy(TensorReadProxy&&) = default;

    operator bool() const noexcept { return m_Success; }

    const bool m_Success;
    std::unique_ptr<char[]> m_Data;
    const std::size_t m_Length;
  };

  void enableOnnxOptmization(PassManager& passManager)
  {
    using Option = OnnxOptPass::Option;

    OnnxOptPass pass;
    pass.add(Option::extract_constant_to_initializer)
        .add(Option::fuse_add_bias_into_conv)
        .add(Option::fuse_bn_into_conv)
        .add(Option::fuse_consecutive_squeezes)
        .add(Option::fuse_consecutive_transposes)
        .add(Option::fuse_transpose_into_gemm)
        .add(Option::eliminate_identity)
        .add(Option::eliminate_nop_pad)
        .add(Option::eliminate_nop_transpose)
        .add(Option::eliminate_unused_initializer)
      ;

    passManager.add<OnnxOptPass>(std::move(pass));
  }

  TensorReadProxy readTensor(const Path& filePath)
  {
    if (!exists(filePath)) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": input file not found: " << filePath
             << std::endl;
      return TensorReadProxy{};
    }

    if (!is_regular(filePath)) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": input file is not a regular file: " << filePath
             << std::endl;
      return TensorReadProxy{};
    }

    std::ifstream stream(filePath.native());
    if (!stream.is_open()) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": cannot open file file: " << filePath
             << std::endl;
      return TensorReadProxy{};
    }

    xTensorProto tensor;
    tensor.ParseFromIstream(&stream);

    const auto& rawData = tensor.raw_data();
    const auto  length  = rawData.length();

    auto data = std::make_unique<char[]>(length);
    std::memcpy(data.get(), rawData.data(), length);

    return TensorReadProxy{std::move(data), length};
  }
} // namespace internal
} // namespace onnc

ONNIApp::ONNIApp(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv),
    m_Options() {
  InitializeAllPlatforms();
  InitializeAllBackends();
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

  if (options().onnxOpt()) {
    internal::enableOnnxOptmization(pm);
  }

  const auto backend = std::unique_ptr<TargetBackend>{target->createBackend(options().target())};
  backend->addTensorSel(pm);
  backend->addTensorSched(pm);
  backend->addMemAlloc(pm);
  if (options().verbose() >= 3) {
    pm.add<CountOperatorsPass>("[Statistics] ");
  }

  // FIXME: Use onnc-runtime to handle input
  std::unique_ptr<char[]> input;
  if (!options().dryRun()) {
    auto result = internal::readTensor(options().input());
    if (result) {
      input = std::move(result.m_Data);
    }
  }
  pm.add<InterpreterPass>(backend.get(), input.get(),
                          options().verbose(), options().dryRun());

  pm.run(module);

  if (options().verbose() >= 3) {
    errs() << "==== print CountOperatorsPass result again ====\n";
    global::stats().print();
    errs() << "==== end again of printing CountOperatorsPass ====\n";
  }
  return EXIT_SUCCESS;
}
