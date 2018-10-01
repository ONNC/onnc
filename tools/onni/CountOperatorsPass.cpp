//===- CountOperatorsPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CountOperatorsPass.h"

#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Module.h>
#include <onnc/Support/IOStream.h>

#include <algorithm>
#include <iomanip>
#include <unordered_map>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CountOperatorsPass
//===----------------------------------------------------------------------===//
Pass::ReturnType CountOperatorsPass::runOnModule(Module &pModule)
{
  std::unordered_map<std::string, int> count;
  size_t op_len = 8;
  uint64_t total = 0;

  for (ComputeOperator &cm : *pModule.getRootComputeGraph()) {
    onnc::StringRef name = cm.name(); 
    count[name] += 1;
    op_len = std::max(op_len, name.size());
    ++total;
  }

  const std::string sep{" |"};
  size_t count_len = (total > 99999) ? 10 : 5;

  count_len += 1;

  outs() << m_Prefix << std::setw(op_len) << "Operator" << sep
         << std::setw(count_len) << "Count" << std::endl;
  outs() << m_Prefix
         << std::setfill('-')
         << std::setw(op_len) << '-' << "-+" << std::setw(count_len) << '-'
         << std::setfill(' ')
         << std::endl;
  for (auto c : count) {
    outs() << m_Prefix << std::setw(op_len) << c.first << sep
           << std::setw(count_len) << c.second << std::endl;
  }
  outs() << m_Prefix
         << std::setfill('-')
         << std::setw(op_len) << '-' << "-+" << std::setw(count_len) << '-'
         << std::setfill(' ')
         << std::endl;
  outs() << m_Prefix << std::setw(op_len) << "Total" << sep
         << std::setw(count_len) << total << std::endl;

  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char CountOperatorsPass::ID = 0;

CountOperatorsPass *onnc::CreateCountOperatorsPass(const std::string &pPrefix) {
  return new CountOperatorsPass(pPrefix);
}
