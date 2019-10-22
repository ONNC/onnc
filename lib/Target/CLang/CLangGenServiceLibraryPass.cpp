#include "CLangGenServiceLibraryPass.h"

#include "CLangOperatorInvokeVisitor.h"
#include "internal/Indent.h"

#include <onnc/ADT/StringRef.h>
#include <onnc/IR/Module.h>

#include <cassert>
#include <fstream>
#include <iostream>

namespace onnc {
namespace internal {
inline void addIncludeDirectives(std::ostream& stream)
{
  stream << "#include <onnc-runtime.h>\n";
  stream << "#include <stdlib.h>\n";
}

inline void addMacroDefinitions(std::ostream& stream)
{
  stream << R"(
#ifndef restrict
# define restrict
#endif
)"
         << "\n";
}

inline void removeMacroDefinitions(std::ostream& stream)
{
  stream << R"(
#undef restrict
)"
         << "\n";
}

} // namespace internal

CLangGenServiceLibraryPass::ReturnType CLangGenServiceLibraryPass::runOnModule(Module& module)
{
  using namespace internal;

  std::ofstream file{outputFile.native()};
  addIncludeDirectives(file);
  addMacroDefinitions(file);
  removeMacroDefinitions(file);
  addModelMainDefinition(file, module);

  return kModuleNoChanged;
}

void CLangGenServiceLibraryPass::addModelMainDefinition(std::ostream& stream, const Module& module)
{
  using namespace internal;
  using identifier_type = CLangOperatorInvokeVisitor::identifier_type;

  const identifier_type context = "context";
  stream << "int model_main(const struct ONNC_RUNTIME_inference_context* " << context << ")\n";
  stream << "{\n";

  constexpr const Indent indent{1};

  // allocate internal memory
  const identifier_type memory = "memory";
  stream << indent << "char * const " << memory << " = calloc(" << getInternalMemorySize() << ", 1);\n";

  CLangOperatorInvokeVisitor visitor{meta, stream, indent, memory, context};
  visitor.visit(module);

  // release internal memory
  stream << indent << "free(" << memory << ");\n";
  stream << indent << "return 0;\n"
         << "}\n";
}

std::size_t CLangGenServiceLibraryPass::getInternalMemorySize() const
{
  if (meta.packedInternalMemoryBlocks.empty()) {
    return 0;
  }

  const auto& lastMemoryBlock = std::rbegin(meta.packedInternalMemoryBlocks)->second;
  return static_cast<std::size_t>(lastMemoryBlock.offset + lastMemoryBlock.length);
}
} // namespace onnc
