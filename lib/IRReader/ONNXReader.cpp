//===-- ONNXReader.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IRReader/ONNXReader.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// onnx::Reader
//===----------------------------------------------------------------------===//
onnx::Reader::Reader()
{
}

onnx::Reader::~Reader()
{
}

SystemError onnx::Reader::parse(const Path& pFileName, Module& pModule)
{
}

SystemError onnx::Reader::parse(StringRef pContent, Module& pModule)
{
}
