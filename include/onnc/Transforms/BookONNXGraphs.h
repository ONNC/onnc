//===- BookONNXGraphs.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_BOOK_ONNX_GRAPHS_H
#define ONNC_BOOK_ONNX_GRAPHS_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class BookONNXGraphs
 *  \brief BookONNXGraphs
 */
class BookONNXGraphs : public CustomPass<BookONNXGraphs>
{
public:
  BookONNXGraphs() = default;

  virtual ~BookONNXGraphs() = default;

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  StringRef getPassName() const override { return "BookONNXGraphs"; }
};

ModulePass *CreateBookONNXGraphs();

} // namespace of onnc

#endif
