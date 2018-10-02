#pragma once

#include <stdint.h>
#include <stdbool.h>

struct ONNC_RUNTIME_Tensor_offset {
  uint64_t offset; /* Tensor offset */
  uint64_t size;   /* Size of tensor in bytes */
};

#define ONNC_RUNTIME_TENSOR_FILE_MAGIC ".TSR"

struct ONNC_RUNTIME_Tensor_offset_table {
  uint8_t magic[8];                                    /* Tensor File magic number. */
  uint64_t number_of_tensors;
  struct ONNC_RUNTIME_Tensor_offset tensor_offsets[];
};

/**
 * ONNC generated entry point.
 * @param context The ONNC Runtime Context.
 */
void model_main(void *context);

/**
 * Initialize runtime.
 * @deprecated
 * @return The ONNC Runtime Context, should be passed to every ONNC Runtime functions.
 */
void *ONNC_RUNTIME_init_runtime();

/**
 * Shutdown runtime.
 * @deprecated
 * @param onnc_runtime_context The ONNC Runtime Context.
 * @return True if shutdown successfully. False if something wrong.
 */
bool ONNC_RUNTIME_shutdown_runtime(void *onnc_runtime_context);

/**
 * Get tensor address from tensor table.
 * @param tensor_table The tensor table start address.
 * @param index Tensor index.
 * @return The memory address of the TensorTable[index].
 */
void *ONNC_RUNTIME_load_from_tensor_table(void *tensor_table, uint32_t index);

#include "operator/abs.h"
#include "operator/acos.h"
#include "operator/add.h"
#include "operator/and.h"
#include "operator/argmax.h"
#include "operator/argmin.h"
#include "operator/asin.h"
#include "operator/atan.h"
#include "operator/averagepool.h"
#include "operator/batchnormalization.h"
#include "operator/cast.h"
#include "operator/ceil.h"
#include "operator/clip.h"
#include "operator/concat.h"
#include "operator/constant.h"
#include "operator/conv.h"
#include "operator/convtranspose.h"
#include "operator/cos.h"
#include "operator/depthtospace.h"
#include "operator/div.h"
#include "operator/dropout.h"
#include "operator/elu.h"
#include "operator/equal.h"
#include "operator/exp.h"
#include "operator/expand.h"
#include "operator/flatten.h"
#include "operator/floor.h"
#include "operator/gru.h"
#include "operator/gather.h"
#include "operator/gemm.h"
#include "operator/globalaveragepool.h"
#include "operator/globallppool.h"
#include "operator/globalmaxpool.h"
#include "operator/greater.h"
#include "operator/hardsigmoid.h"
#include "operator/hardmax.h"
#include "operator/identity.h"
#include "operator/if.h"
#include "operator/instancenormalization.h"
#include "operator/lrn.h"
#include "operator/lstm.h"
#include "operator/leakyrelu.h"
#include "operator/less.h"
#include "operator/log.h"
#include "operator/logsoftmax.h"
#include "operator/loop.h"
#include "operator/lpnormalization.h"
#include "operator/lppool.h"
#include "operator/matmul.h"
#include "operator/max.h"
#include "operator/maxpool.h"
#include "operator/maxroipool.h"
#include "operator/mean.h"
#include "operator/min.h"
#include "operator/mul.h"
#include "operator/multinomial.h"
#include "operator/neg.h"
#include "operator/not.h"
#include "operator/or.h"
#include "operator/prelu.h"
#include "operator/pad.h"
#include "operator/pow.h"
#include "operator/rnn.h"
#include "operator/randomnormal.h"
#include "operator/randomnormallike.h"
#include "operator/randomuniform.h"
#include "operator/randomuniformlike.h"
#include "operator/reciprocal.h"
#include "operator/reducel1.h"
#include "operator/reducel2.h"
#include "operator/reducelogsum.h"
#include "operator/reducelogsumexp.h"
#include "operator/reducemax.h"
#include "operator/reducemean.h"
#include "operator/reducemin.h"
#include "operator/reduceprod.h"
#include "operator/reducesum.h"
#include "operator/reducesumsquare.h"
#include "operator/relu.h"
#include "operator/reshape.h"
#include "operator/scan.h"
#include "operator/selu.h"
#include "operator/shape.h"
#include "operator/sigmoid.h"
#include "operator/sin.h"
#include "operator/size.h"
#include "operator/slice.h"
#include "operator/softmax.h"
#include "operator/softplus.h"
#include "operator/softsign.h"
#include "operator/spacetodepth.h"
#include "operator/split.h"
#include "operator/sqrt.h"
#include "operator/squeeze.h"
#include "operator/sub.h"
#include "operator/sum.h"
#include "operator/tan.h"
#include "operator/tanh.h"
#include "operator/tile.h"
#include "operator/topk.h"
#include "operator/transpose.h"
#include "operator/unsqueeze.h"
#include "operator/upsample.h"
#include "operator/xor.h"
#include "operator/aten.h"
#include "operator/affine.h"
#include "operator/constantfill.h"
#include "operator/crop.h"
#include "operator/gruunit.h"
#include "operator/giventensorfill.h"
#include "operator/imagescaler.h"
#include "operator/meanvariancenormalization.h"
#include "operator/parametricsoftplus.h"
#include "operator/scale.h"
#include "operator/scaledtanh.h"
#include "operator/thresholdedrelu.h"
