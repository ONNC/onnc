#ifndef ONNC_TARGET_TG_CODE_EMITTER_H
#define ONNC_TARGET_TG_CODE_EMITTER_H
#include <onnc/Support/Path.h>
#include <vector>
#include <memory>

namespace bmnet {

class BM168xBackendContext;

} // namespace of bmnet

namespace onnc {

class TGBackend;

class TGCodeEmitter
{
public:
  TGCodeEmitter(TGBackend *tgBackend);

  virtual ~TGCodeEmitter() = default;

  virtual void encodeInstructions(const Path &pOutputPath);

private:
  void *m_bmkernelHandle;
  TGBackend *m_pTGBackend;
};

class tg_kernel
{
public:
  static tg_kernel &getInstance() {
    static tg_kernel instance; // Guaranteed to be destroyed.
                               // Instantiated on first use.
    return instance;
  }

public:
  bmnet::BM168xBackendContext *ctx;

private:
  tg_kernel(tg_kernel const &) = delete;

  void operator=(tg_kernel const &) = delete;

  tg_kernel() {} // Constructor? (the {} brackets) are needed here.

  ~tg_kernel() {}
};

} // namespace onnc

#endif
