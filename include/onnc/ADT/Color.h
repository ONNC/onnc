//===- Color.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_COLOR_H
#define ONNC_ADT_COLOR_H
#include <iosfwd>
#include <iomanip>
#include <onnc/Support/IOSFwd.h>

namespace onnc {

/** \class Color
 *  \brief Color is a std::ostream manipulator for changing the color.
 */
class Color
{
public:
  // color order matches ANSI escape sequence, don't change
  enum kColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,

    RESET
  };

  enum kType {
    NORMAL,
    BOLD,
    BACKGROUND
  };

public:
  Color(kColor pColor = BLACK, kType pType = NORMAL);

  static Color Bold(kColor pColor);

  static Color BackGround(kColor pColor);

  void setColor(std::ostream& pOS);

private:
  kColor m_Color : 24;
  kType m_Type : 8;
};

OStream& operator<<(OStream& pOS, Color pColor);

OStream& operator<<(OStream& pOS, Color::kColor pColor);

} // namespace of onnc

namespace std {

ostream& operator<<(ostream& pOS, onnc::Color pColor);

ostream& operator<<(ostream& pOS, onnc::Color::kColor pColor);

} // namespace of std

#endif
