//===- Color.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Color.h>
#include <onnc/Support/OStream.h>
#include <ostream>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Color
//===----------------------------------------------------------------------===//
Color::Color(kColor pColor, kType pType)
  : m_Color(pColor), m_Type(pType) {
}

Color Color::Bold(kColor pColor)
{
  return Color(pColor, BOLD);
}

Color Color::BackGround(kColor pColor)
{
  return Color(pColor, BACKGROUND);
}

void Color::setColor(std::ostream& pOS)
{
  pOS << "\033[";
  if (m_Color != RESET) {
    if (m_Type == BOLD) {
      pOS << "1;";
    }
    // TODO: Turn off bold mode;
    int base_color = m_Type == BACKGROUND ? 40 : 30;
    pOS << (base_color + m_Color);
  }
  pOS << 'm';
}

OStream& onnc::operator<<(OStream& pOS, Color pColor)
{
  if (pOS.isDisplayed())
    pColor.setColor(pOS);
  return pOS;
}

OStream& onnc::operator<<(OStream& pOS, Color::kColor pColor)
{
  if (pOS.isDisplayed())
    Color(pColor).setColor(pOS);
  return pOS;
}

std::ostream& std::operator<<(std::ostream& pOS, Color pColor)
{
  pColor.setColor(pOS);
  return pOS;
}

std::ostream& std::operator<<(std::ostream& pOS, Color::kColor pColor)
{
  Color(pColor).setColor(pOS);
  return pOS;
}
