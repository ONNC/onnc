//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <cstdlib>

int main(int argc, char* argv[])
{
  skypat::Test::Initialize(&argc, argv);
  skypat::Test::RunAll();

  return (skypat::testing::UnitTest::self()->getNumOfFails() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
