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
SKYPAT_F(ComputeIRTest, attr_test)
{
  FloatAttr alpha;
  ASSERT_EQ(alpha.kind(), Attribute::kFloat);
}

SKYPAT_F(ComputeIRTest, scalar_test)
{
  Scalar a;

  ASSERT_EQ(a.kind(), Value::kUndefined);

  Int32Scalar b;

  ASSERT_EQ(b.kind(), Value::kInt32);
  ASSERT_EQ(b.getValue(), 0);

  b.setValue(10);
  ASSERT_EQ(b.getValue(), 10);

  Int64Scalar c;
  ASSERT_EQ(c.kind(), Value::kInt64);

  // string
  ::onnx::Tensor tensor2;
  StringScalar e(tensor2);
  ASSERT_TRUE(e.empty());
  ASSERT_EQ(e.kind(), Value::kString);
  e.setValue("test");
  ASSERT_TRUE(e.getValue() == "test");
}

SKYPAT_F(ComputeIRTest, tensor_test)
{
}
