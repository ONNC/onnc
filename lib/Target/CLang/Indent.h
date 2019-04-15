#ifndef TARGET_CLANG_INTERNAL_INDENT_H_INCLUDED
#define TARGET_CLANG_INTERNAL_INDENT_H_INCLUDED

#include <cstdint>
#include <ostream>

namespace onnc {
namespace internal {

struct Indent
{
public:
  using level_type = unsigned;
  using width_type = unsigned;

  Indent(level_type level, width_type width = 2)
    : level_{level}
    , width_{width}
  {}

  level_type level() const { return level_; }

  width_type width() const { return width_; }

private:
  const level_type level_;
  const width_type width_;
};

inline Indent operator+(Indent lhs, Indent::level_type rhs) { return Indent{lhs.level() + rhs}; }

inline std::ostream& operator<<(std::ostream& stream, Indent indent)
{
  for (auto spaceCount = indent.level() * indent.width(); 0 < spaceCount; --spaceCount) {
    stream.put(' ');
  }

  return stream;
}

} // namespace internal
} // namespace onnc

#endif
