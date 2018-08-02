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
#ifndef BM168X_CODE_EMITTER_H
#define BM168X_CODE_EMITTER_H
#include "BM168xBackend.h"
#include "TGCodeEmitter.h"
#include <memory>
#include <onnc/Support/Path.h>
#include <vector>

namespace bmnet {

class BM168xBackendContext;

} // namespace bmnet

namespace onnc {

class TGBackend;

class BM168xCodeEmitter : public TGCodeEmitter
{
public:
  BM168xCodeEmitter(TGBackend *pBackend);

  ~BM168xCodeEmitter() override = default;

  void encodeInstructions(std::ostream &pOS) override;

  void genRuntimeInfo(const onnx::Graph *pOnnxGraph,
                      std::ostream &pOS) override;

private:
  // void *m_BmkernelHandle; // NOLINT
  TGBackend *m_pBackend; // NOLINT
};

class bm168x_kernel
{
public:
  static bm168x_kernel &getInstance()
  {
    static bm168x_kernel instance; // Guaranteed to be destroyed.
                                   // Instantiated on first use.
    return instance;
  }

public:
  bmnet::BM168xBackendContext *m_CTX;

  bm168x_kernel(bm168x_kernel const &) = delete;

  void operator=(bm168x_kernel const &) = delete;

private:
  bm168x_kernel() = default;

  ~bm168x_kernel() = default;
};

} // namespace onnc

#endif
