//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <cstdlib>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

int main(int argc, char* argv[])
{
  ::testing::GTEST_FLAG(throw_on_failure) = true;
  ::testing::InitGoogleMock(&argc, argv);
  skypat::Test::Initialize(argc, argv);
  skypat::Test::RunAll();

  return (skypat::testing::UnitTest::self()->getNumOfFails() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
