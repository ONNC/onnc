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
#include "BM188xBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Config/ONNX.h>

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
  std::string getDummyCtable(xGraph *pGraph);

private:
  BM1880Backend *m_pBackend; // NOLINT
};

} // namespace

Pass::ReturnType PrepareCtable::runOnModule(Module &pModule)
{
  auto ctable = m_pBackend->getCtable(pModule);
  if (m_pBackend->options().shouldUseDummyCTable()) {
    if (!ctable.empty()) {
      std::cerr << "error: ctable exist!" << std::endl;
      exit(1);
    }
    xGraph *graph = pModule.getGraphIR().get();
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

std::string PrepareCtable::getDummyCtable(xGraph *pGraph)
{
  tg::bm1880::NetCalibrationParameter net_ctable_param;
  net_ctable_param.set_name(pGraph->name());

  // add "data layer" threshold into ctable
  std::unordered_set<std::string> initializer_names(
      pGraph->initializer_names().begin(), pGraph->initializer_names().end());
  for (size_t i = 0; i < pGraph->inputs().size(); ++i) {
    xValue *v = pGraph->inputs()[i];
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
    if (symbol == xSymbol("Conv")) {
      layer_cal_param->set_right_shift_width(0);
      tg::bm1880::ConvolutionCalibrationCalibrationParameter *conv_cal_param =
          layer_cal_param->mutable_convolution_param();
      conv_cal_param->set_scale_right_shift_width(0);
      // TODO add prelu_param
    } else if (symbol == xSymbol("Gemm") ||
               symbol == xSymbol("Scale")) {
      layer_cal_param->set_right_shift_width(0);
    } else if (symbol == xSymbol("MaxPool") ||
               symbol == xSymbol("AveragePool") ||
               symbol == xSymbol("GlobalAveragePool")) {
      layer_cal_param->set_right_shift_width(0);
      layer_cal_param->add_threshold_x_quantized(0);
    } else if (symbol == xSymbol("Sum") || symbol == xSymbol("Max") ||
               symbol == xSymbol("Mul") || symbol == xSymbol("Add")) {
      layer_cal_param->set_right_shift_width(0);
      for (size_t i = 0; i < node->inputs().size(); i++)
        layer_cal_param->add_threshold_x_quantized(0);
    } else if (symbol == xSymbol("PRelu")) {
      layer_cal_param->set_right_shift_width(0);
      tg::bm1880::PReLUCalibrationParameter *prelu_cal_param =
          layer_cal_param->mutable_prelu_param();
      prelu_cal_param->set_gt_scale(0);
      prelu_cal_param->set_gt_right_shift_width(0);
      prelu_cal_param->set_le_right_shift_width(0);
    } else if (symbol == xSymbol("Concat")) {
      tg::bm1880::ConcatCalibrationParameter *concat_param =
          layer_cal_param->mutable_concat_param();
      concat_param->set_need_quantize_num(0);
    } else if (symbol == xSymbol("Relu") ||
               symbol == xSymbol("Flatten") ||
               symbol == xSymbol("Reshape") ||
               symbol == xSymbol("Unsqueeze") ||
               symbol == xSymbol("Transpose")) {
      // Do nothing.
    } else if (symbol == xSymbol("BatchNormalization")) {
      layer_cal_param->set_right_shift_width(0);
      // Do nothing.
    } else if (symbol == xSymbol("LRN")) {
      auto add_blob = [&](std::string pN) {
        tg::bm1880::BlobParameter *out_blob_param =
            layer_cal_param->add_blob_param();
        out_blob_param->set_name(pN);
        out_blob_param->set_threshold_y(1);
      };
      add_blob("sq");
      add_blob("sum_sq");
      add_blob("scale");
      layer_cal_param->add_threshold_x_quantized(0);
      layer_cal_param->add_threshold_x_quantized(0);
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
