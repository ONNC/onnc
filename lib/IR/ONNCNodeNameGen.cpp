
#include <algorithm>
#include <map>
#include <onnc/IR/Module.h>
#include <onnc/IR/ONNCNodeNameGen.h>
#include <onnx/common/ir.h>
#include <onnx/onnx_pb.h>
#include <string>

class NodeNameGenerator
{
private:
  std::map<std::string, std::string> m_NameTable = { { "MaxPool", "pool" } };
  std::map<std::string, uint32_t> m_CTable;

  std::string str_tolower(std::string pS) const
  {
    std::transform(pS.begin(), pS.end(), pS.begin(),
                   [](unsigned char pC) { return std::tolower(pC); });
    return pS;
  }
  std::string re_name(const std::string &pType) const
  {
    for (auto &nt : m_NameTable) {
      if (pType == nt.first)
        return nt.second;
    }
    return str_tolower(pType);
  }

public:
  std::string gen_name(const std::string &pType)
  {
    auto n = re_name(pType);
    m_CTable[n]++;
    return n + std::to_string(m_CTable[n]);
  }
};

using namespace onnc;

namespace {

class ONNCNodeNameGen : public ModulePass
{
public:
  static char ID;
  ONNCNodeNameGen() : ModulePass(ID) {}

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    for (auto *n : pModule.getGraphIR()->nodes()) {
      if (n->outputs().size() == 0)
        continue;
      auto b = n->outputs()[0]->uniqueName();
      if (b.empty())
        continue;
      n->setName(b);
    }
    return Pass::kModuleNoChanged;
  }
}; // namespace

} // anonymous namespace

char ONNCNodeNameGen::ID = 0;
ModulePass *onnc::createONNCNodeNameGenPass() { return new ONNCNodeNameGen(); }
