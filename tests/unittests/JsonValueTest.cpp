//===- JsonValueTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/Rope.h>
#include <onnc/JSON/Value.h>
#include <onnc/JSON/Type.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/String.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/IOStream.h>
#include <cstring>
#include <string>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// JsonValue Test
//===----------------------------------------------------------------------===//
SKYPAT_F(JsonValueTest, default_constructor)
{
  json::Value val;
  EXPECT_TRUE(val.isUndefined());
}

SKYPAT_F(JsonValueTest, copy_undefined)
{
  json::Value val;
  json::Value cpy = val;
  EXPECT_TRUE(cpy.isUndefined());
}

SKYPAT_F(JsonValueTest, clear)
{
  json::Value val(0.0);
  EXPECT_EQ(val.type(), json::Type::FLOAT);
  val.clear();
  EXPECT_EQ(val.type(), json::Type::UNDEF);
}

SKYPAT_F(JsonValueTest, check_type)
{
  ASSERT_TRUE(is_integral<long long int>::value);
  ASSERT_TRUE(is_integral<long int>::value);
  ASSERT_TRUE(is_integral<int>::value);
  ASSERT_FALSE(is_floating<long long int>::value);
  ASSERT_FALSE(is_floating<long int>::value);
  ASSERT_FALSE(is_floating<int>::value);

  ASSERT_FALSE(is_floating<long long int>::value);
  ASSERT_FALSE(is_floating<long int>::value);
  ASSERT_FALSE(is_floating<int>::value);
  ASSERT_TRUE(is_integral<long long int>::value);
  ASSERT_TRUE(is_integral<long int>::value);
  ASSERT_TRUE(is_integral<int>::value);

  ASSERT_TRUE(is_bool<bool>::value);

  ASSERT_TRUE(is_string<const char*>::value);
  ASSERT_TRUE(is_string<StringRef>::value);
  ASSERT_TRUE(is_string<std::string>::value);
  ASSERT_FALSE(is_string<bool>::value);

  ASSERT_TRUE(is_object<Object>::value);
  ASSERT_FALSE(is_object<std::string>::value);

  ASSERT_TRUE(is_array<Array>::value);
  ASSERT_FALSE(is_array<const char*>::value);
}

SKYPAT_F(JsonValueTest, value_int)
{
  long long int N = 10;
  long int O = 10;
  int P = 10;
  json::Value n(N);
  json::Value o(O);
  json::Value p(P);

  long double X = 1.0;
  double Y = 1.0;
  float  Z = 1.0;

  json::Value x(X);
  json::Value y(Y);
  json::Value z(Z);

  ASSERT_EQ(json::INT, n.type());
  ASSERT_EQ(json::INT, o.type());
  ASSERT_EQ(json::INT, p.type());
  ASSERT_EQ(json::FLOAT, x.type());
  ASSERT_EQ(json::FLOAT, y.type());
  ASSERT_EQ(json::FLOAT, z.type());
}

SKYPAT_F(JsonValueTest, read_from_string)
{
  const char* content = "[ \"a\", \"b\"]";

  json::Reader reader;
  json::Value value;
  reader.read(content, value);

  ASSERT_TRUE(0 == ::strcmp(value.toArray().at(0).toString(), "a"));
  ASSERT_TRUE(0 == ::strcmp(value.toArray().at(1).toString(), "b"));
}

SKYPAT_F(JsonValueTest, notation_check)
{
  json::Value v;
  json::Object o;
  json::Array a;
  ASSERT_EQ(v.type(), json::UNDEF);
  ASSERT_EQ(o.type(), json::OBJECT);
  ASSERT_EQ(a.type(), json::ARRAY);
}

SKYPAT_F(JsonValueTest, assignments)
{
  json::Value value;
  value.assign(5);
  ASSERT_EQ(value.toInteger(), 5);

  int a = 10;
  value.assign(a);
  ASSERT_EQ(value.toInteger(), 10);

  const char* s = "test string";
  json::Value v2;
  v2.assign(s);
  ASSERT_EQ(::strlen(v2.toString()), 11);
}

SKYPAT_F(JsonValueTest, object_test)
{
  json::Object obj;
  obj["name"] = "test";

  ASSERT_EQ(obj.type(), json::OBJECT);
  ASSERT_EQ(obj["name"].type(), json::STRING);
  ASSERT_TRUE(0 == ::strcmp(obj["name"].toString(), "test"));
  EXPECT_EQ(obj.size(), 1);

  obj["other"] = "test";
  EXPECT_EQ(obj.size(), 2);

  EXPECT_TRUE(obj.hasValue("other"));

  json::Object::iterator pair, pEnd = obj.end();
  for (pair = obj.begin(); pair != pEnd; ++pair) {
    EXPECT_TRUE(0 == ::strcmp(pair->value().toString(), "test"));
  }
}

// This test case is used to examine control charactor
SKYPAT_F(JsonValueTest, string_test)
{
  std::string content((Rope("{\n") +
                       Rope("\"option\" : \"D\",\n") +
                       Rope("\"values\" : [ \"") +
                       Rope("FLAGS_STR=\\\\\\\"\\\" -DPERFORMANCE_RUN=1   \\\"\\\\\\\"") +
                       Rope("\" ]\n") +
                       Rope("}")).str());

  json::Reader reader;
  json::Value value;
  ASSERT_TRUE(reader.read(content.c_str(), value));

  ASSERT_TRUE(value.isObject());

  json::Object& obj = value.asObject();
  EXPECT_TRUE(obj["option"].isString());
  EXPECT_TRUE(0 == ::strcmp(obj["option"].toString(), "D"));

  ASSERT_TRUE(obj["values"].isArray());
  json::Array& arr = obj["values"].asArray();
  EXPECT_EQ(arr.size(), 1);

  ASSERT_TRUE(arr.front().isString());
}

SKYPAT_F(JsonValueTest, string_trim)
{
  const char* s1 = "FLAGS_STR=\\\\\\\"\\\" -DPERFORMANCE_RUN=1   \\\"\\\\\\\"";
  ASSERT_TRUE(0 == json::trim(s1).compare("FLAGS_STR=\\\"\" -DPERFORMANCE_RUN=1   \"\\\""));

  const char* s2 = "\\ttab";
  ASSERT_TRUE(0 == json::trim(s2).compare("	tab"));

  const char* s3 = "backspace\\b";
  ASSERT_TRUE(0 == json::trim(s3).compare("backspace\b"));

  const char* s4 = "new\\nline";
  ASSERT_TRUE(0 == json::trim(s4).compare("new\nline"));
}
