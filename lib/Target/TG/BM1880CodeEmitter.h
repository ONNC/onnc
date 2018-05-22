#ifndef BM1880_CODE_EMITTER_H
#define BM1880_CODE_EMITTER_H
#include "BM1880Backend.h"
#include "TGCodeEmitter.h"
#include <memory>
#include <onnc/Support/Path.h>
#include <vector>

namespace bmnet {

class BM188xBackendContext;

} // namespace bmnet

namespace onnc {

class BM1880Backend;

class BM1880CodeEmitter : public TGCodeEmitter
{
public:
  BM1880CodeEmitter(BM1880Backend *pBackend);

  virtual ~BM1880CodeEmitter() = default;

  void encodeInstructions(const Path &pOutputPath) override;

private:
  void *m_bmkernelHandle;
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
  bmnet::BM188xBackendContext *m_Ctx;

private:
  bm1880_kernel(bm1880_kernel const &) = delete;

  void operator=(bm1880_kernel const &) = delete;

  bm1880_kernel() = default;

  ~bm1880_kernel() = default;
};

} // namespace onnc

#endif
