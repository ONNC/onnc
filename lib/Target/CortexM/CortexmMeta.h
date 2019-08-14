#ifndef TARGET_CORTEXM_META_H
#define TARGET_CORTEXM_META_H

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/IOStream.h>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <onnc/IR/Compute/Tensor.h>

#include "my_loadable/loadable.h"
#include "my_loadable/loadable_generated.h"

#include "weight_pack.h"
//#include "matmul.h"

#define MAX2(x,y) x>y?x:y
#define MAX4(w,x,y,z) MAX2((MAX2(w,x)),(MAX2(y,z)))
#define input_buffer(x) x==0?"img_buffer2":"img_buffer1"
#define output_buffer(x) x==0?"img_buffer1":"img_buffer2"

#define TYPE_CONV 1
#define TYPE_MAXPOOLING 2
#define TYPE_RELU 3
#define TYPE_SOFTMAX 4
#define TYPE_FULLYCONNECT 5
#define TYPE_ADD 6
#define TYPE_MATMUL 7
#define TYPE_Error 8

using namespace cortexm_loadable;
using namespace cortexm_loadable::my_loadable;

namespace onnc{
  typedef std::unordered_map<const Tensor*,const Tensor*> RemapTable; 
  typedef std::unordered_map<Value*,int> MemIdxTable;
  
  struct code_list{
    unsigned int input_dimention;
    int input_channel;
    int output_channel;
    int kernel_size;//w = h
    int pad;//w = h
    int stride;// w = h
    int buffer_order;
    unsigned int output_dimention;
    int layer_type;//what is this layer doing
    int layer_id;
    int *pads;
    struct code_list *next;  
  };
  
  struct weight_list{
    float *weight_value;
    int weight_size;
    float *bias_value;
    int bias_size;
    bool have_bias;
    struct weight_list *next;
  };

  struct add_list{
    float *add_value;
    int add_size;
    int *add_dims;
    int add_dims_size;
    int *input_dims;
    int input_dims_size;
    struct add_list *next;
  };

  struct matmul_list{
    float *matmul_value;
    int matmul_size;
    struct matmul_list *next;
  };

  struct shape_list{
    int *shape_value;
    struct shape_list *next;
  };

  struct shift_list{
    int shift_value;
    struct shift_list *next;
  };

  class CortexmBackendMeta{
    public:
      CortexmBackendMeta();
      ~CortexmBackendMeta();

      MemIdxTable m_MemIdxTable;
      
      int m_NumMlobs;
    //public:
      //std::vector<ILoadable::MemoryListEntry> m_MemoryListEntries;
      
      my_loadable::LoadableFactory_cortexm::LoadablePrivPair m_Loadable;
  };

}



#endif
