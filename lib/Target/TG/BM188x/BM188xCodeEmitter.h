#ifndef BM188X_CODE_EMITTER_H
#define BM188X_CODE_EMITTER_H
#include "BM188xBackend.h"
#include "TGCodeEmitter.h"
#include "plat-bm188x/BM188xBackendContext.hpp"
#include <memory>
#include <onnc/Support/Path.h>
#include <vector>

namespace bmnet {

class BM1880BackendContext;

} // namespace bmnet

namespace onnc {

class BM1880Backend;

class BM188xCodeEmitter : public TGCodeEmitter
{
public:
  BM188xCodeEmitter(BM1880Backend *pBackend);

  ~BM188xCodeEmitter() override = default;

  void encodeInstructions(const Path &pOutputPath) override;

  void prepareWeight(std::vector<int8_t> &pWeight);

private:
  void *m_BmkernelHandle;
  BM1880Backend *m_Backend;
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
  bmnet::BM188xBackendContext *m_CTX;

private:
  bm1880_kernel(bm1880_kernel const &) = delete;

  void operator=(bm1880_kernel const &) = delete;

  bm1880_kernel() = default;

  ~bm1880_kernel() = default;
};

} // namespace onnc

#endif
