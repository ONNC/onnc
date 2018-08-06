//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#ifndef BM188X_CODE_EMITTER_H
#define BM188X_CODE_EMITTER_H
#include "BM188xBackend.h"
#include "TGCodeEmitter.h"
#include "Weight.h"
#include <onnc/JSON/Object.h>
#include <onnc/Support/Path.h>
#include <onnc/Config/ONNX.h>
#include <memory>
#include <vector>

namespace bmnet {

class BM1880BackendContext;

} // namespace bmnet

namespace onnc {

class BM1880Backend;

class BM188xCodeEmitter : public TGCodeEmitter
{
public:
  BM188xCodeEmitter(BM1880Backend *pBackend, BM1880Backend::Instructions& pInsns);

  ~BM188xCodeEmitter() override = default;

  void genRuntimeInfo(const xGraph *pOnnxGraph,
                      std::ostream &pOS) override;

  void encodeInstructions(std::ostream &pOS) override;

  void genWeightBin(const std::string &pOutputFilename) override;

private:
  json::Object genOutputLayer(const std::string &pDefaultOnncLayerName,
                              const std::string &pDefaultOnnxLayerName,
                              const xGraph *pOnnxGraph);

  float getThreshold(const std::string &pOnncLayerName);

  std::string findOnncLayerName(const xGraph *pOnnxGraph,
                                const xValue *pValue);

private:
  // void *m_BmkernelHandle;
  BM1880Backend *m_Backend;
  BM1880Backend::Instructions& m_Instructions;
};

class bm1880_kernel
{
public:
  static bm1880_kernel &getInstance()
  {
    static bm1880_kernel instance; // Guaranteed to be destroyed.
                                   // Instantiated on first use.
    return instance;
  }

public:
  bm1880_kernel(bm1880_kernel const &) = delete;

  void operator=(bm1880_kernel const &) = delete;

private:
  bm1880_kernel() = default;

  ~bm1880_kernel() = default;
};

} // namespace onnc

#endif
