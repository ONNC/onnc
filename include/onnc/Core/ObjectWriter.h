//===- ObjectWriter.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_OBJECT_WRITER_H
#define ONNC_CORE_OBJECT_WRITER_H
#include <onnc/Support/OStream.h>

namespace onnc {

/** \class onnc::ObjectWriter
 *  \brief provides interfaces for dumping compiler output.
 *
 *  Deep learning accelerators don't have common standard for the output
 *  file format. Some of them define their own format and some of them use
 *  just memory image as the output format. ObjectWriter tries to provide
 *  a common interface to encapsulate various formats in writing steps.
 */
class ObjectWriter
{
public:
  ObjectWriter(OStream& pOS);

  virtual ~ObjectWriter() = 0;

  OStream& getStream() { return *m_pOS; }

  void setStream(OStream& pOS) { m_pOS = &pOS; }

private:
  OStream* m_pOS;
};

} // namespace of onnc

#endif
