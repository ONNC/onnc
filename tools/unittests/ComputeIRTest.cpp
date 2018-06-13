//===- ComputeIRTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Scalar.h>
#include <onnx/common/tensor.h>
#include <ostream>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(ComputeIRTest, attribute_test)
{
  FloatAttr alpha;
  errs() << alpha.kind() << std::endl;

  Scalar<int64_t, Value::kInt64> a;

  ASSERT_EQ(a.kind(), Value::kInt64);

  ::onnx::Tensor tensor;
  Scalar<int64_t, Value::kInt64> b(tensor);

  b.setValue(10);
  ASSERT_EQ(b.getValue(), 10);
}
