//===- StringRefTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/StringRef.h>
#include <cstring>
#include <onnc/Support/IOStream.h>

using namespace skypat;
using namespace onnc;

//===----------------------------------------------------------------------===//
// StringRef Test
//===----------------------------------------------------------------------===//
SKYPAT_F(StringRefTest, default_constructor)
{
  StringRef str;
  ASSERT_TRUE(NULL == str.data());
  ASSERT_TRUE(0 == str.size());
  ASSERT_TRUE(0 == str.length());
  ASSERT_TRUE(str.str().empty());
  ASSERT_TRUE(str.begin() == str.end());
}

SKYPAT_F(StringRefTest, const_constructor)
{
  StringRef str("test string");
  ASSERT_TRUE(11 == str.size());
  ASSERT_TRUE(str.length() == str.size());
  ASSERT_FALSE(str.empty());
  ASSERT_TRUE(2 == str.count('s'));
  ASSERT_TRUE(2 == str.count("st"));
  ASSERT_TRUE(0 == str.count('z'));
  ASSERT_TRUE(0 == str.count("ts"));
  ASSERT_TRUE(0 == str.count("test string is larger than original string"));
  ASSERT_TRUE(0 == strcmp("test string", str.data()));
  int counter = 0;
  for (StringRef::iterator i = str.begin(), e = str.end(); i != e; ++i)
    ++counter;
  ASSERT_TRUE(11 == counter);
  ASSERT_TRUE('t' == str.front());
  ASSERT_TRUE('g' == str.back());

  StringRef str_ne("another test string");
  StringRef str_eq("test string");
  ASSERT_FALSE(str.equals(str_ne));
  ASSERT_FALSE(str.equals("the other one"));
  ASSERT_TRUE(str.equals(str_eq));
  ASSERT_TRUE(str.equals("test string"));

  StringRef complex_str("Test StrinG");
  ASSERT_TRUE(complex_str.equals_lower(str));
  ASSERT_TRUE(0   == complex_str.compare_lower(str));
  ASSERT_TRUE(-1  == complex_str.compare(str));
  ASSERT_TRUE(1   == str.compare(complex_str));
  ASSERT_TRUE('s' == str[2]);
  ASSERT_TRUE(0   == strcmp("test string", complex_str.lower().data()));

  ASSERT_TRUE(complex_str.startswith("Test"));
  ASSERT_TRUE(str.startswith("test"));
  ASSERT_TRUE(complex_str.endswith(" StrinG"));
  ASSERT_TRUE(str.endswith("ing"));
}

SKYPAT_F(StringRefTest, string_search)
{
  StringRef str("TeSt sTrInG");
  ASSERT_TRUE(0 == str.find('T'));
  ASSERT_TRUE(1 == str.find('e'));
  ASSERT_TRUE(2 == str.find('S'));
  ASSERT_TRUE(3 == str.find('t'));
  ASSERT_TRUE(6 == str.find('T', 1));

  ASSERT_TRUE(2 == str.find("St"));
  ASSERT_TRUE(StringRef::npos == str.find('z'));

  ASSERT_TRUE(6 == str.rfind('T'));

  ASSERT_TRUE(5 == str.find_first_of('s'));
  ASSERT_TRUE(0 == str.find_first_of('T'));
  ASSERT_TRUE(6 == str.find_last_of('T'));
  ASSERT_TRUE(7 == str.find_last_of("sTr"));

  ASSERT_TRUE(1 == str.find_first_not_of('T'));
}

SKYPAT_F(StringRefTest, substring_op)
{
  StringRef str("tEsT StRiNg");

  ASSERT_TRUE(str.substr(1, 3).equals("EsT"));
  ASSERT_TRUE(str.slice(1, 6).equals("EsT S"));

  std::pair<StringRef, StringRef> res1 = str.split(' ');
  ASSERT_TRUE(res1.first.equals("tEsT"));
  ASSERT_TRUE(res1.second.equals("StRiNg"));

  std::pair<StringRef, StringRef> res2 = str.split("T S");
  ASSERT_TRUE(res2.first.equals("tEs"));
  ASSERT_TRUE(res2.second.equals("tRiNg"));
}

SKYPAT_F(StringRefTest, last_char)
{
  StringRef str("-Wa,");
  char delimiter = ',';
  StringRef::size_type pos = str.find(delimiter);
  ASSERT_TRUE(pos == (str.size()-1));
}

SKYPAT_F(StringRefTest, valid_checking)
{
  StringRef invalid;
  StringRef sub = invalid.substr(0, 10);
  ASSERT_FALSE(sub.isValid());
  ASSERT_TRUE(sub.empty());
}

SKYPAT_F(StringRefTest, valid_checking2)
{
  StringRef invalid;
  StringRef sub = invalid.substr(2, 10);
  ASSERT_FALSE(sub.isValid());
  ASSERT_TRUE(sub.empty());
}

SKYPAT_F(StringRefTest, substr_equals)
{
  StringRef str("A long long string");
  StringRef sub = str.substr(2, 4);
  ASSERT_TRUE(sub.equals("long"));
}

SKYPAT_F(StringRefTest, str_to_ulonglong)
{
  StringRef str("16180339");
  unsigned long long value;
  bool ret = StrToULongLong(str, 10, value);
  ASSERT_TRUE(ret);
  ASSERT_EQ(value, 16180339ULL);
}

SKYPAT_F(StringRefTest, str_to_ulonglong_max)
{
  StringRef str("18446744073709551616");
  unsigned long long value;
  bool ret = StrToULongLong(str, 10, value);
  ASSERT_FALSE(ret);
}

SKYPAT_F(StringRefTest, str_to_longlong_positive)
{
  StringRef str("16180339");
  long long value;
  bool ret = StrToLongLong(str, 10, value);
  ASSERT_TRUE(ret);
  ASSERT_EQ(value, 16180339LL);
}

SKYPAT_F(StringRefTest, str_to_longlong_negative)
{
  StringRef str("-16180339");
  long long value;
  bool ret = StrToLongLong(str, 10, value);
  ASSERT_TRUE(ret);
  ASSERT_EQ(value, -16180339LL);
}
