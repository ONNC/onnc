#!/bin/bash -ex
onnx_as=$1
onnx2tg=$2
$onnx_as ${asm_name} -o ${onnx_name}
$onnx2tg ${onnx_name} -march ${OPT_MARCH} ${OPT_ONNX2TG_ARGS} 2>&1 | tee ${log_name} | FileCheck ${asm_name}
