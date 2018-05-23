#include <memory>
#include <onnc/IR/Dump.h>
#include <onnc/IR/ONNCModulePrinter.h>
#include <string>

using namespace onnc;

namespace {

class ONNCModulePrinter : public ModulePass
{
public:
  static char ID;
  ONNCModulePrinter() : ModulePass(ID) {}

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    ::onnc::DumpModule(pModule);
    return Pass::kModuleNoChanged;
  }
};

} // anonymous namespace

char ONNCModulePrinter::ID = 0;
ModulePass *onnc::createONNCModulePrinterPass()
{
  return new ONNCModulePrinter();
}
