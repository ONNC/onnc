
tg_test(ASM_NAME lenet_onnx.s MARCH bm1880 ONNX2TG_ARGS -print-machineinstrs)
tg_test(ASM_NAME lenet_onnx_noctable.s MARCH bm1880 ONNX2TG_ARGS -ignore-calibration-step -print-machineinstrs)
tg_test(ASM_NAME fuse_conv_bn_mul_add_relu.s MARCH bm1880 ONNX2TG_ARGS -ignore-calibration-step -print-module-before-isel)
