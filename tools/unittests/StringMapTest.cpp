//===- StringMapTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringMap.h>
#include <skypat/skypat.h>

using namespace skypat;
using namespace onnc;

SKYPAT_F(StringMapTest, insert)
{
  StringMap<int> m;
  ASSERT_EQ(m.numOfEntries(), 0);
  bool exist = true;
  m.insert("abc", exist)->value() = 123;
  ASSERT_FALSE(exist);
  ASSERT_EQ(m.numOfEntries(), 1);

  ASSERT_EQ(m.insert("abc", exist)->value(), 123);
  ASSERT_TRUE(exist);
}

SKYPAT_F(StringMapTest, find)
{
  StringMap<int> m;
  bool exist = true;
  m.insert("abc", exist)->value() = 123;
  {
    auto it = m.find("abc");
    ASSERT_TRUE(it != m.end());
    ASSERT_EQ(it->value(), 123);
  }
  {
    auto it = m.find("def");
    ASSERT_TRUE(it == m.end());
  }
}

SKYPAT_F(StringMapTest, count)
{
  StringMap<int> m;
  bool exist = true;
  m.insert("abc", exist)->value() = 123;
  m.count("abc");
  ASSERT_EQ(m.count("abc"), 1);
  ASSERT_EQ(m.count("def"), 0);
}
