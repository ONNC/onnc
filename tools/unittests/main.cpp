//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <pat/pat.h>
#include <cstdlib>

int main(int argc, char* argv[])
{
  pat::Test::Initialize(argc, argv);
  pat::Test::RunAll();

  return (pat::testing::UnitTest::self()->getNumOfFails() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
