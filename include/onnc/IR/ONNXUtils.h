//===- ONNXUtils.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_ONNX_UTILS_H
#define ONNC_IR_ONNX_UTILS_H
#include <onnc/IR/Module.h>
#include <onnc/Config/ONNX.h>

namespace onnc {

typedef std::vector<xDimension> TensorSizes;
typedef std::vector<int64_t> LongInts;

void SerializeToString(std::string &pOutput, const Module &pModule);

void ExportModelProto(xProto &pModelProto, const Module &pModule);

/// Factory of Module
/// @param [in] pModuleProto The prototex of the module.
Module* CreateModule(const xProto &pModelProto);

/// Factory of Module. destroy the module.
void DestroyModule(Module*& pModule);

size_t getTotalCount(const std::vector<int64_t> &pDim);

const xTensor &getTensor(std::string pName, const xGraph &pGraph);

bool OutputSizeIsInputSize(xNode& pNode);

void GetAttrVals(xNode& pNode, xBuiltinSymbol pAttr, LongInts& pVal);

void GetPads(xNode& pNode, LongInts& pPadsB, LongInts& pPadsE);

void GetConvKernelShape(xNode& pNode, LongInts& pKShape);

/// XXX: do we lost a reference at the last parameter?
bool IsTranspose(const xNode& pNode, const xBuiltinSymbol pAttr);

LongInts GetValueSizes(const xValue& pVal);

} // namespace onnc

#endif
