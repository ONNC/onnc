//===- StringSwitchTest.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/StringSwitch.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Testcases
//===----------------------------------------------------------------------===//
namespace {

enum Color {
  Red,
  Blue,
  Yello,
  Unknown
};

} // anonymous namespace

SKYPAT_F( StringSwitchTest, simple_test)
{
  const char* color_str = "Red";
  Color color = StringSwitch<Color>(color_str)
    .Case("Red", Red)
    .Case("Blue", Blue)
    .Case("Yello", Yello)
    .Default(Unknown);

  ASSERT_TRUE(Red == color);
}

SKYPAT_F( StringSwitchTest, default_test) {
  const char* color_str = "NoColor";
  Color color = StringSwitch<Color>(color_str)
    .Case("Red", Red)
    .Case("Blue", Blue)
    .Case("Yello", Yello)
    .Default(Unknown);

  ASSERT_TRUE(Unknown == color);
}

SKYPAT_F( StringSwitchTest, start_with_test)
{
  const char* color_str = "BlueSky";
  Color color = StringSwitch<Color>(color_str)
    .StartsWith("Red", Red)
    .StartsWith("Blue", Blue)
    .StartsWith("Yello", Yello)
    .Default(Unknown);

  ASSERT_TRUE(Blue == color);
}

SKYPAT_F( StringSwitchTest, end_with_test)
{
  const char* color_str = "TheLightIsYello";
  Color color = StringSwitch<Color>(color_str)
    .EndsWith("Red", Red)
    .Case("Blue", Blue)
    .EndsWith("Yello", Yello)
    .Default(Unknown);

  ASSERT_TRUE(Yello == color);
}
