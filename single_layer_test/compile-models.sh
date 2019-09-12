#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
MODELS=($(find $SCRIPT_DIR -iname "*.onnx"))

ONNC=onnc

for model in "${MODELS[@]}"; do
  model_dir="$(dirname $model)"
  model_basename="$(basename $model .onnx)"
  $ONNC -mquadruple nvdla $model
  cp out.nvdla $model_dir/$model_basename.nvdla
done
