//===- WeightTest.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include "../Weight.h"
#include <onnx/common/ir.h>
#include "../io.hpp"
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// WeightTest
//===----------------------------------------------------------------------===//
SKYPAT_F(BM188xTest, weight_append)
{
  onnx::Tensor tensor;
  std::string raw_data("abcdefghijklmnopqr");
  tensor.set_raw_data(raw_data);
  tensor.sizes().push_back(1);
  tensor.sizes().push_back(3);
  tensor.sizes().push_back(3);
  tensor.sizes().push_back(2);

  BM188X::Weight::WeightType weight;
  BM188X::Weight::append16bit(weight, tensor.raw());

  errs() << onnc::getTotalCount(tensor.sizes()) << std::endl;
  errs() << raw_data.size() << std::endl;
  errs() << weight.size() << std::endl;

  errs() << "12345678901234567890" << std::endl;
  for (unsigned i = 0; i < weight.size(); ++i) {
    if ('a' > weight[i] || 'z' < weight[i])
      errs() << '*';
    else
      errs() << weight[i];
  }
  errs() << std::endl;
  ASSERT_EQ(weight.size(), 36);
  ASSERT_TRUE(weight[0]  == 'a');
  ASSERT_TRUE(weight[18] == 'b');
  ASSERT_TRUE(weight[1]  == 'c');
  ASSERT_TRUE(weight[19] == 'd');
  ASSERT_TRUE(weight[2]  == 'e');
  ASSERT_TRUE(weight[20] == 'f');
  ASSERT_TRUE(weight[3]  == 'g');
  ASSERT_TRUE(weight[21] == 'h');
  ASSERT_TRUE(weight[4]  == 'i');
  ASSERT_TRUE(weight[22] == 'j');
  ASSERT_TRUE(weight[5]  == 'k');
  ASSERT_TRUE(weight[23] == 'l');
  ASSERT_TRUE(weight[6]  == 'm');
  ASSERT_TRUE(weight[24] == 'n');
  ASSERT_TRUE(weight[7]  == 'o');
  ASSERT_TRUE(weight[25] == 'p');
  ASSERT_TRUE(weight[8]  == 'q');
  ASSERT_TRUE(weight[26] == 'r');
}
