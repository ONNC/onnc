//===- OperatorCodeGen.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "OperatorCodeGen.h"
#include <fstream>

using namespace onnc;

using std::string;

string OperatorCodeGen::generate(const string OperatorName) {
  std::ifstream ifs;
  string content = "";

  if(OperatorName == "Abs") {
    ifs.open("../../Runtime/operator/abs.inc");
  }
  // TODO: remaining operators

  // TODO: if find nothing, should be error

  content.assign(
    (std::istreambuf_iterator<char>(ifs)),
    (std::istreambuf_iterator<char>())
  );

  ifs.close();

  return content;
}
