//===- Fallback.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Fallback.h"

using namespace onnc;
using namespace caffe2;

namespace onnc {

bool Fallback::executeLayers()
{
  onnc::json::Object j_cpu_fallback = m_JRoot["cpu fallback"].asObject();

  for (size_t i = 0; i < j_cpu_fallback.size(); i++) {
    Workspace ws;
    OperatorDef def;
    onnc::json::Object j_layer_info =
        j_cpu_fallback[std::to_string(i)].asObject();
    std::string op_type = j_layer_info["type"].toString();
    if (op_type == "Softmax") {
      // Input info
      onnc::json::Object j_input = j_layer_info["input0"].asObject();
      onnc::json::Array j_dim = j_input["dim"].asArray();
      std::string input_name = j_input["name"].toString();
      std::vector<int64_t> dims;
      for (size_t j = 0; j < j_dim.size(); j++) {
        dims.push_back(j_dim[j].toInteger());
      }
      std::vector<float> input_data;
      for (size_t j = 0; j < m_InputSize; j++) {
        input_data.emplace_back(m_pData[j]);
      }
      // Output info
      onnc::json::Object j_output = j_layer_info["output0"].asObject();
      std::string output_name = j_output["name"].toString();
      // Prepare Caffe2 OP info.
      def.set_type("Softmax");
      def.add_input(input_name);
      def.add_output(output_name);
      TensorCPU tensor(dims, input_data, nullptr);
      auto *t_input = ws.CreateBlob(input_name)->GetMutable<TensorCPU>();
      t_input->ResizeLike(tensor);
      t_input->ShareData(tensor);

      ws.RunOperatorOnce(def);

      auto output = ws.GetBlob(output_name)->Get<TensorCPU>();
      const auto &probs = output.data<float>();
      // FIXME: Store the results in m_BlobData.
      for (auto i = 0; i < output.size(); i++) {
        std::cout << "probs_" << i << " = " << probs[i] << "\n";
      }
    } else {
      // TODO: Implement more ops.
      std::cout << "Fallback op:" << op_type << " is not implemented\n";
      return false;
    }
  }

  return true;
}

Pass::ReturnType Fallback::runOnModule(::onnc::Module &pModule)
{
  assert(m_pData != nullptr);

  // Execute fallback layers by the execution plan.
  executeLayers();

  return Pass::kModuleNoChanged;
}
} // namespace onnc

char Fallback::ID = 0;
ModulePass *onnc::createFallbackPass(float *pData, size_t pSize,
                                     json::Object pJRoot)
{
  return new Fallback(pData, pSize, pJRoot);
}
