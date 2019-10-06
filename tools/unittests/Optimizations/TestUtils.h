//===- TestUtils.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPT_TEST_UTILS
#define ONNC_OPT_TEST_UTILS
#include <onnc/Core/PassSupport.h>
#include <skypat/skypat.h>

#include <sstream>
#include <string>
#include <unordered_map>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Testing methods
//===----------------------------------------------------------------------===//
template <typename OptPass, typename F, typename... PassCtorParams>
          // typename std::enable_if<std::is_function<F>::value>::type * = nullptr>
void testOptPassOnNetwork(F createNetworkFunc,
                          Pass::ReturnType ansRet,
                          const std::string &ansStr,
                          PassCtorParams&& ... PassParams) {
  Module module;
  createNetworkFunc(module);

  OptPass op(std::forward<PassCtorParams>(PassParams)...);
  Pass::ReturnType passRet = op.runOnModule(module);

  std::ostringstream oss;
  module.getRootComputeGraph()->print(oss);

  EXPECT_EQ(passRet, ansRet);
  EXPECT_EQ(oss.str(), ansStr);
}

template <typename F,
          typename std::enable_if<std::is_function<F>::value>::type * = nullptr>
std::string getNetworkString(const F& createNetworkFunc) {
  static std::unordered_map<intptr_t, std::string> strCache;
  intptr_t funcKey = reinterpret_cast<intptr_t>(createNetworkFunc);
  auto it = strCache.find(funcKey);
  if (it != strCache.end()) return it->second;

  Module module;
  createNetworkFunc(module);
  std::ostringstream oss;
  module.getRootComputeGraph()->print(oss);
  auto retPair = strCache.emplace(funcKey, oss.str());

  return (retPair.first)->second;
}

#endif // ONNC_OPT_TEST_UTILS
