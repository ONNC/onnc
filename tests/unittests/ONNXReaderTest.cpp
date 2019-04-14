//===- ONNXReaderTest.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/IRReader/ONNXReader.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNXReader test
//===----------------------------------------------------------------------===//
SKYPAT_F(ONNXReaderTest, parse_file)
{
  Path path(TOPDIR);
  path.append("tools")
      .append("unittests")
      .append("data")
      .append("bvlc_alexnet")
      .append("model.onnx");
  onnc::Module module;
  onnc::onnx::Reader reader;
  SystemError err = reader.parse(path, module);
  ASSERT_TRUE(err.isGood());
}
