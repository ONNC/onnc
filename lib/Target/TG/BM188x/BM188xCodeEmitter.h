#ifndef BM188X_CODE_EMITTER_H
#define BM188X_CODE_EMITTER_H
#include "BM188xBackend.h"
#include "TGCodeEmitter.h"
#include <memory>
#include <onnc/JSON/Object.h>
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
  static void prepare8bitWeight(const MemOperand *pMemOp,
                                std::vector<int8_t> &pWeight);
  static void prepare16bitWeight(const MemOperand *pMemOp,
                                 std::vector<int8_t> &pWeight);
  BM188xCodeEmitter(BM1880Backend *pBackend);

  ~BM188xCodeEmitter() override = default;

  void encodeInstructions(std::ostream &pOS) override;

  void prepareWeight(std::vector<int8_t> &pWeight);

  void genWeightBin(const std::string &pOutputFilename) override;

  void genRuntimeInfo(const onnx::Graph *pOnnxGraph,
                      std::ostream &pOS) override;

private:
  json::Object genOutputLayer(std::string &pDefaultOnncLayerName,
                              std::string &pDefaultOnnxLayerName,
                              const ::onnx::Graph *pOnnxGraph);
  float getThreshold(const std::string &pOnncLayerName);

  std::string findOnncLayerName(const onnx::Graph *pOnnxGraph,
                                const onnx::Value *pValue);

private:
  // void *m_BmkernelHandle;
  BM1880Backend *m_Backend;
  std::vector<int8_t> m_WeightData;
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
