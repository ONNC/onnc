//===- JsonObjectTest.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/JSON/Object.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// JsonObjectTest
//===----------------------------------------------------------------------===//
SKYPAT_F(JsonObjectTest, object_insert)
{
  json::Object obj;
  obj.insert("key1", 1);

  obj.insert("key2", 2.2);

  obj.insert("key3", "3strings");

  obj.insert("key4", true);

  int       value1 = obj["key1"].toInteger();
  double    value2 = obj["key2"].toFloating();
  StringRef value3 = obj["key3"].toString();
  bool      value4 = obj["key4"].toBool();

  EXPECT_EQ(value1, 1);
  EXPECT_EQ(value2, 2.2);
  EXPECT_TRUE(value3.equals("3strings"));
  EXPECT_TRUE(value4);

  obj.clear();
  EXPECT_TRUE(obj.empty());
  EXPECT_EQ(obj.size(), 0);
}

SKYPAT_F(JsonObjectTest, object_insert_cant_override)
{
  json::Object obj;
  bool rst1 = obj.insert("key", 1);
  bool rst2 = obj.insert("key", 2.2);
  bool rst3 = obj.insert("key", "3strings");
  bool rst4 = obj.insert("key", true);

  EXPECT_TRUE(rst1);
  EXPECT_FALSE(rst2);
  EXPECT_FALSE(rst3);
  EXPECT_FALSE(rst4);

  int value1 = obj["key"].toInteger();
  EXPECT_EQ(value1, 1);
}

SKYPAT_F(JsonObjectTest, object_write_override)
{
  json::Object obj;
  bool rst1 = obj.write("key", 1);
  bool rst2 = obj.write("key", 2.2);
  bool rst4 = obj.write("key", true);
  bool rst3 = obj.write("key", "3strings");

  EXPECT_TRUE(rst1);
  EXPECT_FALSE(rst2);
  EXPECT_FALSE(rst3);
  EXPECT_FALSE(rst4);

  StringRef value = obj["key"].toString();
  EXPECT_TRUE(value.equals("3strings"));
}
