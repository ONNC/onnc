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

  virtual ~BM168xCodeEmitter() = default;

  void encodeInstructions(const Path &pOutputPath) override;

private:
  void *m_bmkernelHandle;
  TGBackend *m_pBackend;
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
  bmnet::BM168xBackendContext *ctx;

private:
  bm168x_kernel(bm168x_kernel const &) = delete;

  void operator=(bm168x_kernel const &) = delete;

  bm168x_kernel() = default;

  ~bm168x_kernel() = default;
};

} // namespace onnc

#endif
