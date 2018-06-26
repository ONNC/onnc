
set(common_option -S -print-module-before-isel)
tg_test(ASM_NAME lenet_onnx.s                MARCH bm1880 ONNX2TG_ARGS ${common_option})
tg_test(ASM_NAME lenet_onnx_noctable.s       MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step)
tg_test(ASM_NAME fuse_conv_bn_mul_add_relu.s MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step)
tg_test(ASM_NAME fuse_mul_add.s              MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step)
tg_test(ASM_NAME fuse_bn_mul_add.s           MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step)
tg_test(ASM_NAME fuse_conv_bn_mul_add.s      MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step)
tg_test(ASM_NAME average_pool_onnx.s         MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step)
