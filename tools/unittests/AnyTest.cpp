//===- AnyTest.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/Any.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <ostream>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(AnyTest, value_constructors)
{
  // assign int
  std::string data;
  OStrStream os(data);
  Any triseven(777);
  os << triseven;
  ASSERT_TRUE(0 == data.compare("777"));

  Any astring("aaa");
  os.clearbuf();
  os << astring;
  ASSERT_TRUE(0 == data.compare("aaa"));

  triseven = std::string("8888");
  os.clearbuf();
  os << triseven;
  ASSERT_TRUE(0 == data.compare("8888"));
}

SKYPAT_F(AnyTest, copy_constructors)
{
  Any triseven(777);
  Any other(triseven);

  std::string data;
  OStrStream os(data);
  os << other;
  ASSERT_TRUE(0 == data.compare("777"));

  const Any astring("aaa");
  Any other_string(astring);
  os.clearbuf();
  os << other_string;
  ASSERT_TRUE(0 == data.compare("aaa"));
}

SKYPAT_F(AnyTest, assign_constructor)
{
  Any triseven = 777;
  std::string data;
  OStrStream os(data);
  os << triseven;
  ASSERT_TRUE(0 == data.compare("777"));
}

namespace {
  struct Complex {
    int a;
    int b;

    Complex(int pA, int pB) : a(pA), b(pB) { }
  };
} // anonymous namespace

namespace std {
ostream& operator<<(ostream& pOS, const Complex& pC)
{
  pOS << (pC.a + pC.b);
  return pOS;
}

} // namespace of std

SKYPAT_F(AnyTest, complex_value_constructor)
{

  Complex com(1, 2);
  // assign int
  std::string data;
  OStrStream os(data);
  Any comp_any(com);
  os << comp_any;

  ASSERT_TRUE(0 == data.compare("3"));
}
