
tg_test(ASM_NAME lenet_onnx.s MARCH bm1880)
tg_test(ASM_NAME lenet_onnx_noctable.s MARCH bm1880 ONNX2TG_ARGS -ignore-calibration-step)
