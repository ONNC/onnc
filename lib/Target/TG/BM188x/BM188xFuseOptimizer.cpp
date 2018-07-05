#define DEBUG_TYPE "bm188x_fuse_opt"
#include "BM188xFuseOptimizer.h"
#include "BM188xBackend.h"
#include <onnc/Support/Debug.h>

namespace onnc {

static bool
getTensorThreshold(const tg::bm1880::LayerCalibrationParameter *pLayerCtable,
                   std::string pName, float *pThres)
{
  bool is_find_thres = false;
  for (int i = 0; i < pLayerCtable->blob_param_size(); i++) {
    DEBUG(dbgs() << "pName = " << pName << ", blob_name = "
                 << pLayerCtable->blob_param(i).name() << "\n";);
    if (pName == pLayerCtable->blob_param(i).name()) {
      *pThres = pLayerCtable->blob_param(i).threshold_y();
      is_find_thres = true;
      break;
    }
  }
  return is_find_thres;
}

onnx::Node *BM188xFuseOptimizer::FuseConvScale(onnx::Graph *pGraph,
                                               onnx::Node *pConvNode,
                                               onnx::Node *pScaleNode)
{
  // keep conv layer's output threshold because FuseConvScale will change output
  // name
  const std::string conv_output_name = pConvNode->output()->uniqueName();
  const auto *conv_output_ctable =
      m_p1880backend->getLayerCtable(conv_output_name);
  float conv_output_threshold;
  if (!getTensorThreshold(conv_output_ctable, conv_output_name,
                          &conv_output_threshold)) {
    errs() << "count not find threshold in: " << conv_output_name << "\n";
    assert(0);
  }
  onnx::Node *new_conv =
      TGFuseOptimizer::FuseConvScale(pGraph, pConvNode, pScaleNode);
  // append conv layer's output threshold as attribute
  new_conv->f_(onnx::Symbol("conv_output_threshold"), conv_output_threshold);
  return new_conv;
}

} // namespace onnc
