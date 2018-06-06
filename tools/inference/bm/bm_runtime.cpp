//===- bm_runtime.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include <onnc/JSON/Object.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/Value.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Path.h>

#include <bm_debug.h>
#include <bmruntime.h>
#include <bmruntime_bmnet.h>

namespace po = boost::program_options;

static int getFileLength(std::fstream &pF)
{
  pF.seekg(0, pF.end);
  int length = pF.tellg();
  pF.seekg(0, pF.beg);
  return length;
}

static void parseOutputInfo(onnc::json::Object &pRoot,
                            std::string &pOutputLayerName, uint64_t *pOffset,
                            size_t *pSize)
{
  onnc::json::Object jMemLayout = pRoot["memory layout"].asObject();
  std::cout << pOutputLayerName << "\n";
  onnc::json::Object jOutputLayer = jMemLayout[pOutputLayerName].asObject();
  onnc::json::Object jOutputNode = jOutputLayer[pOutputLayerName].asObject();
  *pOffset = jOutputNode["addr"].toInteger();
  *pSize = jOutputNode["size"].toInteger();
  return;
}

static void quantize(float *pData, int8_t *pQData, size_t pSize,
                     float pThreshold)
{
  for (size_t i = 0; i < pSize; i++) {
    float data = pData[i];
    int fixedData = (int)(data * (128.0 / pThreshold) + 0.5);
    pQData[i] =
        (fixedData < -128) ? -128 : ((fixedData > 127) ? 127 : fixedData);
  }
  return;
}

int main(int pArgc, char *pArgv[])
{
  onnc::Path inputPath;
  onnc::Path weightPath;
  onnc::Path cmdbufPath;
  onnc::Path descPath;
  std::string outputFileName;
  {
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help,h", "produce help message")
        ("input,i", po::value(&inputPath)->required(), "inferenced file (.bin)")
        ("weight,w", po::value(&weightPath)->required(), "weight file (.bin)")
        ("cmdbuf,c", po::value(&cmdbufPath)->required(), "bm command buffer (.bin)")
        ("desc,d", po::value(&descPath)->required(), "bm runtime description file (.json)")
        ("output,o", po::value(&outputFileName)->default_value("output.bin"), "output .bin file");
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(pArgc, pArgv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
    }
    po::notify(vm);
  }

  std::fstream fInput(inputPath.c_str(), std::ios::in | std::ios::binary);
  size_t inputSize = getFileLength(fInput);
  float *inputFp32 = new float[inputSize];
  fInput.read((char *)inputFp32, inputSize);

  std::fstream fWeight(weightPath.c_str(), std::ios::in | std::ios::binary);
  size_t weightSize = getFileLength(fWeight);
  uint8_t *weight = new uint8_t[weightSize];
  fWeight.read((char *)weight, weightSize);

  std::fstream fCmdbuf(cmdbufPath.c_str(), std::ios::in | std::ios::binary);
  size_t cmdbufSize = getFileLength(fCmdbuf);
  uint8_t *cmdbuf = new uint8_t[cmdbufSize];
  fCmdbuf.read((char *)cmdbuf, cmdbufSize);

  onnc::json::Reader jR;
  onnc::json::Value runtimeInfo;
  jR.parse(descPath, runtimeInfo);

  // Get data layer threshold and quantize it.
  onnc::json::Object jRoot = runtimeInfo.asObject();
  onnc::json::Object jThresh = jRoot["layer threshold"].asObject();
  float dataThresh = jThresh["data layer threshold"].toFloating();
  int8_t *input = (int8_t *)malloc(inputSize);
  quantize(inputFp32, input, inputSize, dataThresh);

  onnc::json::Object jBatch = jRoot["batch"].asObject();
  auto batchSize = jBatch["size"].toInteger();

  onnc::json::Object jOutputLayer = jRoot["output layer"].asObject();
  std::string outputLayerName = jOutputLayer["onnc output"].toString();
  std::fstream fOutput(outputFileName,
                       std::ios::out | std::ios::trunc | std::ios::binary);

  uint64_t outputOffset;
  size_t outputSize;
  parseOutputInfo(jRoot, outputLayerName, &outputOffset, &outputSize);
  uint8_t *output = new uint8_t[outputSize];
  size_t neuronSize = outputOffset + outputSize;

  bmctx_t ctx;
  bmerr_t ret;
  ret = bm_init(0, &ctx);
  if (ret != BM_SUCCESS) {
    std::cout << "bm_init fail: err = " << ret << "\n";
    assert(0);
  }

  bmnet_inference_once(ctx, (uint8_t *)input, output, batchSize, weight,
                       weightSize, cmdbuf, cmdbufSize, neuronSize, inputSize,
                       outputOffset, outputSize);

  bm_exit(ctx);

  // FIXME: read output nchw from descriptor.
  // dump_data_int8("output", output, batchSize, 1, 1, 10);
  fOutput.write((char *)output, outputSize);

  fOutput.close();

  delete[] output;
  delete[] cmdbuf;
  delete[] weight;
  delete[] inputFp32;
  delete[] input;

  return 0;
}
