#include <memory>
#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Dump.h>
#include <onnc/IR/ONNCModulePrinter.h>
#include <string>

using namespace onnc;

namespace {

class ONNCModulePrinter : public CustomPass<ONNCModulePrinter>
{
public:
  ONNCModulePrinter() = default;

  StringRef getPassName() const override { return "ONNCModulePrinter"; }
  
  Pass::ReturnType runOnModule(Module &pModule) override
  {
    ::onnc::DumpModule(pModule);
    return Pass::kModuleNoChanged;
  }
};

} // anonymous namespace

ModulePass *onnc::createONNCModulePrinterPass()
{
  return new ONNCModulePrinter();
}
