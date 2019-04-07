#include "CLangGenServiceLibraryPass.h"

#include <onnc/ADT/StringRef.h>
#include <onnc/IR/Module.h>

#include <cassert>
#include <fstream>
#include <iostream>

namespace onnc {

namespace internal {
template <unsigned Width>
struct indent_t
{
  const unsigned level;
};

using indent_type = internal::indent_t<4>;

template <unsigned Width>
inline indent_t<Width> operator+(indent_t<Width> lhs, unsigned rhs)
{
  return indent_t<Width>{lhs.level + rhs};
}

template <unsigned Width>
inline std::ostream& operator<<(std::ostream& stream, indent_t<Width> indent)
{
  for (auto spaceCount = indent.level * Width; 0 < spaceCount; --spaceCount) {
    stream.put(' ');
  }

  return stream;
}

inline void addIncludeDirectives(std::ostream& stream)
{
  stream << "#include <assert.h>\n";
  stream << "#include <float.h>\n";
  stream << "#include <math.h>\n";
  stream << "#include <stddef.h>\n";
  stream << "#include <stdlib.h>\n";
  stream << "#include <stdint.h>\n";
  stream << "#include <string.h>\n";
  stream << "#include <onnc-runtime.h>\n";
}

inline void addContentFromFile(std::ostream& stream, const Path& file)
{
  assert(is_regular(file));

  stream << std::ifstream{file.native()}.rdbuf();
}

template <typename StringNames>
inline void addOperatorFunctionDefinitions(std::ostream& stream, const Path& resourceDirectory,
                                           const StringNames& names)
{
  const Path implFilesDirectory = resourceDirectory + "include" + "internal";
  assert(is_directory(implFilesDirectory));

  addContentFromFile(stream, implFilesDirectory + "util.inc");

  for (const auto& name : names) {
    const Path implFile = implFilesDirectory + (StringRef{name}.lower() + ".inc");

    addContentFromFile(stream, implFile);
  }
}

inline void addModelMainDefinition(std::ostream& stream)
{
  stream << "int model_main(const struct ONNC_RUNTIME_inference_context* context)\n";
  stream << "{\n";
  stream << "}\n";
}
} // namespace internal

CLangGenServiceLibraryPass::ReturnType CLangGenServiceLibraryPass::runOnModule(Module& module)
{
  using namespace internal;

  std::ofstream file{outputFile.native()};

  addIncludeDirectives(file);
  addOperatorFunctionDefinitions(file, resourceDirectory, meta.usedOperatorNames);
  addModelMainDefinition(file);

  return kModuleNoChanged;
}

} // namespace onnc
