//===- OStream.tcc --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OSTREAM_TCC
#define ONNC_SUPPORT_OSTREAM_TCC

namespace onnc {

template<typename CharT, typename Traits> BasicOStream<CharT, Traits>&
BasicOStream<CharT, Traits>::changeColor(enum Colors pColor,
                                         bool pBold,
                                         bool pBG)
{
  if (hasColors()) {
    *this << "\033[";
    if (pColor == Colors::SAVEDCOLOR) {
      // TODO: Turn off bold mode;
      if (pBold) {
        *this << '1';
      }
    } else {
      // TODO: Turn off bold mode;
      int base_color = pBG ? 40 : 30;
      if (pBold) {
        *this << "1;";
      }
      *this << (base_color + pColor);
    }
    *this << 'm';
  }
  return *this;
}

template<typename CharT, typename Traits> BasicOStream<CharT, Traits>&
BasicOStream<CharT, Traits>::resetColor()
{
  if (hasColors()) {
    *this << "\033[m";
  }
  return *this;
}

/// reverseColor - Revers the foreground and background colors.
template<typename CharT, typename Traits> BasicOStream<CharT, Traits>&
BasicOStream<CharT, Traits>::reverseColor()
{
  if (hasColors()) {
    *this << "\033[7m";
  }
  return *this;
}

/// isDisplayed - Can the stream connected to a terminal that can
/// display ANSI color, such as "tty" or "console" window.
template<typename CharT, typename Traits>
bool BasicOStream<CharT, Traits>::isDisplayed() const
{
  // TODO
  return false;
}

} // namespace of onnc

#endif
