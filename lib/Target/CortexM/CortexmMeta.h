#ifndef TARGET_CORTEXM_META_H
#define TARGET_CORTEXM_META_H

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/IOStream.h>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <onnc/IR/Compute/Tensor.h>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "weight_pack.h"

#define MAX2(x, y) x > y ? x : y
#define MAX4(w, x, y, z) MAX2((MAX2(w, x)), (MAX2(y, z)))
#define input_buffer(x) x == 0 ? "img_buffer2" : "img_buffer1"
#define output_buffer(x) x == 0 ? "img_buffer1" : "img_buffer2"

#define TYPE_CONV 1
#define TYPE_MAXPOOLING 2
#define TYPE_RELU 3
#define TYPE_SOFTMAX 4
#define TYPE_FULLYCONNECT 5
#define TYPE_ADD 6
#define TYPE_MATMUL 7
#define TYPE_Error 8

namespace onnc {
typedef std::unordered_map<const Tensor*, const Tensor*> RemapTable;
typedef std::unordered_map<Value*, int> MemIdxTable;

class CortexmBackendMeta {
public:
  CortexmBackendMeta();
  ~CortexmBackendMeta();

  struct Layer;
  struct Weight;
  struct Add;
  struct Matmul;
  struct Shape;
  struct Shift;

  std::vector<Layer> m_layerList;
  std::vector<Weight> m_weightList;
  std::vector<Add> m_addList;
  std::vector<Matmul> m_matmulList;
  std::vector<Shape> m_shapeList;
  std::vector<Shift> m_shiftList;
};

struct CortexmBackendMeta::Layer {
  unsigned int input_dimension;
  int batch_size;
  int input_channel;
  int output_channel;
  int kernel_size; // w = h
  int pad;    // w = h
  int stride; // w = h
  int input_size;
  int weight_dim_size;
  int weight_size;
  int buffer_order;
  unsigned int output_dimension;
  int layer_type; // what is this layer doing
  int* pads; // TODO: need initialization
  int matmul_size;
};

struct CortexmBackendMeta::Weight {
  float* weight_value;
  int weight_size;
  float* bias_value;
  int bias_size;
  bool have_bias;
};

struct CortexmBackendMeta::Add {
  float* add_value;
  int add_size;
  int* add_dims;
  int add_dims_size;
  int* input_dims;
  int input_dims_size;
};

struct CortexmBackendMeta::Matmul {
  unsigned int input_dimension;
  unsigned int output_dimension;
  int batch_size;
  int input_channel;
  int output_channel;
  float* matmul_value;
  int matmul_size;
  struct matmul_list* next;
};

struct CortexmBackendMeta::Shape {
  int* shape_value;
};

struct CortexmBackendMeta::Shift {
  int shift_value;
};


}

#endif
