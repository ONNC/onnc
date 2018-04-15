#include <onnc/Target/TargetBackend.h>

namespace onnc {

bool TargetBackend::addPassesToEmitFile(PassManager &pPM, Path output,
                                 CodeGenFileType fileType) {
  // add common codegen passes
  addTensorSel(pPM);
  addMemAlloc(pPM);

  // add code emitter
  addCodeEmit(pPM, output, fileType);
  return true;
}

} // namespace onnc
