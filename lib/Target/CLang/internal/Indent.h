#ifndef TARGET_CLANG_INTERNAL_INDENT_H_INCLUDED
#define TARGET_CLANG_INTERNAL_INDENT_H_INCLUDED

#include <cstdint>
#include <ostream>

namespace onnc {
namespace internal {

struct Indent
{
public:
  using level_type = unsigned short;
  using width_type = unsigned short;

  friend constexpr Indent operator+(Indent, Indent::level_type);
  friend std::ostream&    operator<<(std::ostream&, Indent);

  constexpr Indent(level_type level, width_type width = 2) noexcept
    : level{level}
    , width{width}
  {}

private:
  const level_type level;
  const width_type width;
};

inline constexpr Indent operator+(Indent lhs, Indent::level_type rhs)
{
  return Indent{static_cast<Indent::level_type>(lhs.level + rhs), lhs.width};
}

inline std::ostream& operator<<(std::ostream& stream, Indent indent)
{
  for (auto spaceCount = indent.level * indent.width; 0 < spaceCount; --spaceCount) {
    stream.put(' ');
  }

  return stream;
}

} // namespace internal
} // namespace onnc

#endif
