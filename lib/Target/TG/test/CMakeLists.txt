
set(common_option -S -print-module-before-isel)

tg_test(ASM_NAME lenet_with_weight.s         MARCH bm1880 ONNX2TG_ARGS ${common_option})
tg_test(ASM_NAME lenet.s                     MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-weight)
tg_test(ASM_NAME fuse_conv_bn_mul_add_relu.s MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable -add-dummy-weight)
tg_test(ASM_NAME fuse_mul_add.s              MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable)
tg_test(ASM_NAME fuse_bn_mul_add.s           MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable)
tg_test(ASM_NAME fuse_conv_bn_mul_add.s      MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable -add-dummy-weight)
tg_test(ASM_NAME average_pool_onnx.s         MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable)
tg_test(ASM_NAME concat_onnx.s               MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable)
tg_test(ASM_NAME transpose_onnx.s            MARCH bm1880 ONNX2TG_ARGS ${common_option} -ignore-calibration-step -add-dummy-ctable)
tg_test(ASM_NAME fuse_sum_relu.s             MARCH bm1880 ONNX2TG_ARGS ${common_option} -add-dummy-ctable -add-dummy-weight)
