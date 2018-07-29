#include "BM188xBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class PrepareCtable : public ModulePass
{
public:
  static char ID;

public:
  PrepareCtable(BM1880Backend *pBackend) : ModulePass(ID), m_pBackend(pBackend)
  {
  }

  StringRef getPassName() const override { return "PrepareCtable"; }
  
  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  // insert ctable into backend
  std::string getDummyCtable(onnx::Graph *pGraph);

private:
  BM1880Backend *m_pBackend; // NOLINT
};

} // namespace

Pass::ReturnType PrepareCtable::runOnModule(Module &pModule)
{
  auto ctable = m_pBackend->getCtable(pModule);
  /// XXX: this makes the pass logic difficult to trace.
  /// The condition should move to backend.
  if (m_pBackend->options().shouldUseDummyCTable()) {
    if (not ctable.empty()) {
      std::cerr << "error: ctable exist!" << std::endl;
      exit(1);
    }
    onnx::Graph *graph = pModule.getGraphIR().get();
    ctable = getDummyCtable(graph);
  } else {
    if (ctable.empty()) {
      std::cerr << "error: ctable not found!" << std::endl;
      exit(1);
    }
  }
  m_pBackend->setCtableProto(ctable);
  return Pass::kModuleNoChanged;
}

std::string PrepareCtable::getDummyCtable(onnx::Graph *pGraph)
{
  tg::bm1880::NetCalibrationParameter net_ctable_param;
  net_ctable_param.set_name(pGraph->name());

  // add "data layer" threshold into ctable
  std::unordered_set<std::string> initializer_names(
      pGraph->initializer_names().begin(), pGraph->initializer_names().end());
  for (size_t i = 0; i < pGraph->inputs().size(); ++i) {
    onnx::Value *v = pGraph->inputs()[i];
    if (0 != initializer_names.count(v->uniqueName()))
      continue;
    tg::bm1880::LayerCalibrationParameter *layer_cal_param =
        net_ctable_param.add_layer();
    layer_cal_param->set_name(v->uniqueName());
    tg::bm1880::BlobParameter *out_blob_param =
        layer_cal_param->add_blob_param();
    out_blob_param->set_name(v->uniqueName());
    out_blob_param->set_threshold_y(1);
  }

  // iterator each node
  for (auto *node : pGraph->nodes()) {
    const std::string &layer_name = node->outputs()[0]->uniqueName();
    tg::bm1880::LayerCalibrationParameter *layer_cal_param =
        net_ctable_param.add_layer();
    layer_cal_param->set_name(layer_name);
    // add BlobParameter
    for (auto *v : node->outputs()) {
      if (0 != initializer_names.count(v->uniqueName()))
        continue;
      tg::bm1880::BlobParameter *out_blob_param =
          layer_cal_param->add_blob_param();
      out_blob_param->set_name(v->uniqueName());
      out_blob_param->set_threshold_y(1);
    }

    // add differnet layer param into ctable
    uint32_t symbol = node->kind();
    // sync to LayerImpl.h
    if (symbol == onnx::Symbol("Conv")) {
      layer_cal_param->set_right_shift_width(0);
      tg::bm1880::ConvolutionCalibrationCalibrationParameter *conv_cal_param =
          layer_cal_param->mutable_convolution_param();
      conv_cal_param->set_scale_right_shift_width(0);
      // TODO add prelu_param
    } else if (symbol == onnx::Symbol("Gemm") ||
               symbol == onnx::Symbol("Scale")) {
      layer_cal_param->set_right_shift_width(0);
    } else if (symbol == onnx::Symbol("MaxPool") ||
               symbol == onnx::Symbol("AveragePool")) {
      layer_cal_param->set_right_shift_width(0);
      layer_cal_param->add_threshold_x_quantized(0);
    } else if (symbol == onnx::Symbol("Sum") || symbol == onnx::Symbol("Max") ||
               symbol == onnx::Symbol("Mul") || symbol == onnx::Symbol("Add")) {
      layer_cal_param->set_right_shift_width(0);
      for (auto *v : node->inputs()) {
        if (0 != initializer_names.count(v->uniqueName()))
          continue;
        layer_cal_param->add_threshold_x_quantized(0);
      }
    } else if (symbol == onnx::Symbol("PRelu")) {
      layer_cal_param->set_right_shift_width(0);
      tg::bm1880::PReLUCalibrationParameter *prelu_cal_param =
          layer_cal_param->mutable_prelu_param();
      prelu_cal_param->set_gt_scale(0);
      prelu_cal_param->set_gt_right_shift_width(0);
      prelu_cal_param->set_le_right_shift_width(0);
    } else if (symbol == onnx::Symbol("Concat")) {
      tg::bm1880::ConcatCalibrationParameter *concat_param =
          layer_cal_param->mutable_concat_param();
      concat_param->set_need_quantize_num(0);
    } else if (symbol == onnx::Symbol("Relu") ||
               symbol == onnx::Symbol("Flatten") ||
               symbol == onnx::Symbol("Reshape") ||
               symbol == onnx::Symbol("Unsqueeze") ||
               symbol == onnx::Symbol("Transpose")) {
      // Do nothing.
    } else if (symbol == onnx::Symbol("BatchNormalization")) {
      layer_cal_param->set_right_shift_width(0);
      // Do nothing.
    } else if (symbol == onnx::Symbol("LRN")) {
      // FIXME: fp.tsai@iclink.tw
    } else {
      // FIXME: Add assert in the future.
      errs() << "Error: Unsupport op type " << node->kind().toString()
             << std::endl;
    }
  }

  return net_ctable_param.DebugString();
}

char PrepareCtable::ID = 0;

ModulePass *onnc::createPrepareCtablePass(BM1880Backend *pBackend)
{
  return new PrepareCtable(pBackend);
}
