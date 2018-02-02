//===- ObjectWriter.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/ObjectWriter.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ObjectWriter
//===----------------------------------------------------------------------===//
ObjectWriter::ObjectWriter(OStream& pOS)
  : m_pOS(&pOS) {
}

ObjectWriter::~ObjectWriter()
{
  /* To date, do nothing */
}
