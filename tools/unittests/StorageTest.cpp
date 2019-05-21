//===- StorageTest.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/JSON/Storage.h>
#include <onnc/JSON/Group.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Storage Test
//===----------------------------------------------------------------------===//
SKYPAT_F(GroupTest, constructor)
{
  json::Group group;
  EXPECT_TRUE(group.isNull());

  json::Storage storage;
  json::Group g1 = storage.addGroup("group 1");
  outs() << storage << std::endl;
//  group = g1;
//  EXPECT_FALSE(group.isNull());
//  EXPECT_TRUE(group.isEmpty());
}
