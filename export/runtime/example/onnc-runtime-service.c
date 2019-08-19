#include <onnc-runtime.h>
#include <internal/common.h>
#include <operators.h>

#ifndef restrict
# define restrict
#endif


#undef restrict

int model_main(const struct ONNC_RUNTIME_inference_context* context)
{
  char * const memory = calloc(7169856, 1);
  ONNC_RUNTIME_init();
  // Conv
  {
    float* const v1 = ONNC_RUNTIME_read_tensor((context->input), 0).data;
    const int32_t v2[] = { 1, 3, 224, 224 };
    float* const v3 = ONNC_RUNTIME_read_tensor((context->weight), 0).data;
    const int32_t v4[] = { 96, 3, 11, 11 };
    float* const v5 = ONNC_RUNTIME_read_tensor((context->weight), 1).data;
    const int32_t v6[] = { 96 };
    float* const v7 = ((float*)(memory + 0));
    const int32_t v8[] = { 1, 96, 54, 54 };
    int32_t v9[] = { 1, 1 };
    int32_t v10[] = { 11, 11 };
    int32_t v11[] = { 0, 0, 0, 0 };
    int32_t v12[] = { 4, 4 };
    ONNC_RUNTIME_conv_float(NULL, ((float*)(v1)), 4, v2, ((float*)(v3)), 4, v4, ((float*)(v5)), 1, v6, ((float*)(v7)), 4, v8, "NOTSET", v9, 2, 1, v10, 2, v11, 4, v12, 2);
  }
  // Relu
  {
    float* const v13 = ((float*)(memory + 0));
    const int32_t v14[] = { 1, 96, 54, 54 };
    float* const v15 = ((float*)(memory + 1119744));
    const int32_t v16[] = { 1, 96, 54, 54 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v13)), 4, v14, ((float*)(v15)), 4, v16);
  }
  // LRN
  {
    float* const v17 = ((float*)(memory + 1119744));
    const int32_t v18[] = { 1, 96, 54, 54 };
    float* const v19 = ((float*)(memory + 2239488));
    const int32_t v20[] = { 1, 96, 54, 54 };
    ONNC_RUNTIME_lrn_float(NULL, ((float*)(v17)), 4, v18, ((float*)(v19)), 4, v20, 0.000100, 0.750000, 1.000000, 5);
  }
  // MaxPool
  {
    float* const v21 = ((float*)(memory + 2239488));
    const int32_t v22[] = { 1, 96, 54, 54 };
    float* const v23 = ((float*)(memory + 3359232));
    const int32_t v24[] = { 1, 96, 26, 26 };
    void* v25 = NULL;
    int32_t* v26 = NULL;
    int32_t v27[] = { 3, 3 };
    int32_t v28[] = { 0, 0, 0, 0 };
    int32_t v29[] = { 2, 2 };
    ONNC_RUNTIME_maxpool_float(NULL, ((float*)(v21)), 4, v22, ((float*)(v23)), 4, v24, ((float*)(v25)), 0, v26, "NOTSET", v27, 2, v28, 4, 0, v29, 2);
  }
  // Conv
  {
    float* const v30 = ((float*)(memory + 3359232));
    const int32_t v31[] = { 1, 96, 26, 26 };
    float* const v32 = ONNC_RUNTIME_read_tensor((context->weight), 2).data;
    const int32_t v33[] = { 256, 48, 5, 5 };
    float* const v34 = ONNC_RUNTIME_read_tensor((context->weight), 3).data;
    const int32_t v35[] = { 256 };
    float* const v36 = ((float*)(memory + 3618816));
    const int32_t v37[] = { 1, 256, 26, 26 };
    int32_t v38[] = { 1, 1 };
    int32_t v39[] = { 5, 5 };
    int32_t v40[] = { 2, 2, 2, 2 };
    int32_t v41[] = { 1, 1 };
    ONNC_RUNTIME_conv_float(NULL, ((float*)(v30)), 4, v31, ((float*)(v32)), 4, v33, ((float*)(v34)), 1, v35, ((float*)(v36)), 4, v37, "NOTSET", v38, 2, 2, v39, 2, v40, 4, v41, 2);
  }
  // Relu
  {
    float* const v42 = ((float*)(memory + 3618816));
    const int32_t v43[] = { 1, 256, 26, 26 };
    float* const v44 = ((float*)(memory + 4311040));
    const int32_t v45[] = { 1, 256, 26, 26 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v42)), 4, v43, ((float*)(v44)), 4, v45);
  }
  // LRN
  {
    float* const v46 = ((float*)(memory + 4311040));
    const int32_t v47[] = { 1, 256, 26, 26 };
    float* const v48 = ((float*)(memory + 5003264));
    const int32_t v49[] = { 1, 256, 26, 26 };
    ONNC_RUNTIME_lrn_float(NULL, ((float*)(v46)), 4, v47, ((float*)(v48)), 4, v49, 0.000100, 0.750000, 1.000000, 5);
  }
  // MaxPool
  {
    float* const v50 = ((float*)(memory + 5003264));
    const int32_t v51[] = { 1, 256, 26, 26 };
    float* const v52 = ((float*)(memory + 5695488));
    const int32_t v53[] = { 1, 256, 12, 12 };
    void* v54 = NULL;
    int32_t* v55 = NULL;
    int32_t v56[] = { 3, 3 };
    int32_t v57[] = { 0, 0, 0, 0 };
    int32_t v58[] = { 2, 2 };
    ONNC_RUNTIME_maxpool_float(NULL, ((float*)(v50)), 4, v51, ((float*)(v52)), 4, v53, ((float*)(v54)), 0, v55, "NOTSET", v56, 2, v57, 4, 0, v58, 2);
  }
  // Conv
  {
    float* const v59 = ((float*)(memory + 5695488));
    const int32_t v60[] = { 1, 256, 12, 12 };
    float* const v61 = ONNC_RUNTIME_read_tensor((context->weight), 4).data;
    const int32_t v62[] = { 384, 256, 3, 3 };
    float* const v63 = ONNC_RUNTIME_read_tensor((context->weight), 5).data;
    const int32_t v64[] = { 384 };
    float* const v65 = ((float*)(memory + 5842944));
    const int32_t v66[] = { 1, 384, 12, 12 };
    int32_t v67[] = { 1, 1 };
    int32_t v68[] = { 3, 3 };
    int32_t v69[] = { 1, 1, 1, 1 };
    int32_t v70[] = { 1, 1 };
    ONNC_RUNTIME_conv_float(NULL, ((float*)(v59)), 4, v60, ((float*)(v61)), 4, v62, ((float*)(v63)), 1, v64, ((float*)(v65)), 4, v66, "NOTSET", v67, 2, 1, v68, 2, v69, 4, v70, 2);
  }
  // Relu
  {
    float* const v71 = ((float*)(memory + 5842944));
    const int32_t v72[] = { 1, 384, 12, 12 };
    float* const v73 = ((float*)(memory + 6064128));
    const int32_t v74[] = { 1, 384, 12, 12 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v71)), 4, v72, ((float*)(v73)), 4, v74);
  }
  // Conv
  {
    float* const v75 = ((float*)(memory + 6064128));
    const int32_t v76[] = { 1, 384, 12, 12 };
    float* const v77 = ONNC_RUNTIME_read_tensor((context->weight), 6).data;
    const int32_t v78[] = { 384, 192, 3, 3 };
    float* const v79 = ONNC_RUNTIME_read_tensor((context->weight), 7).data;
    const int32_t v80[] = { 384 };
    float* const v81 = ((float*)(memory + 6285312));
    const int32_t v82[] = { 1, 384, 12, 12 };
    int32_t v83[] = { 1, 1 };
    int32_t v84[] = { 3, 3 };
    int32_t v85[] = { 1, 1, 1, 1 };
    int32_t v86[] = { 1, 1 };
    ONNC_RUNTIME_conv_float(NULL, ((float*)(v75)), 4, v76, ((float*)(v77)), 4, v78, ((float*)(v79)), 1, v80, ((float*)(v81)), 4, v82, "NOTSET", v83, 2, 2, v84, 2, v85, 4, v86, 2);
  }
  // Relu
  {
    float* const v87 = ((float*)(memory + 6285312));
    const int32_t v88[] = { 1, 384, 12, 12 };
    float* const v89 = ((float*)(memory + 6506496));
    const int32_t v90[] = { 1, 384, 12, 12 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v87)), 4, v88, ((float*)(v89)), 4, v90);
  }
  // Conv
  {
    float* const v91 = ((float*)(memory + 6506496));
    const int32_t v92[] = { 1, 384, 12, 12 };
    float* const v93 = ONNC_RUNTIME_read_tensor((context->weight), 8).data;
    const int32_t v94[] = { 256, 192, 3, 3 };
    float* const v95 = ONNC_RUNTIME_read_tensor((context->weight), 9).data;
    const int32_t v96[] = { 256 };
    float* const v97 = ((float*)(memory + 6727680));
    const int32_t v98[] = { 1, 256, 12, 12 };
    int32_t v99[] = { 1, 1 };
    int32_t v100[] = { 3, 3 };
    int32_t v101[] = { 1, 1, 1, 1 };
    int32_t v102[] = { 1, 1 };
    ONNC_RUNTIME_conv_float(NULL, ((float*)(v91)), 4, v92, ((float*)(v93)), 4, v94, ((float*)(v95)), 1, v96, ((float*)(v97)), 4, v98, "NOTSET", v99, 2, 2, v100, 2, v101, 4, v102, 2);
  }
  // Relu
  {
    float* const v103 = ((float*)(memory + 6727680));
    const int32_t v104[] = { 1, 256, 12, 12 };
    float* const v105 = ((float*)(memory + 6875136));
    const int32_t v106[] = { 1, 256, 12, 12 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v103)), 4, v104, ((float*)(v105)), 4, v106);
  }
  // MaxPool
  {
    float* const v107 = ((float*)(memory + 6875136));
    const int32_t v108[] = { 1, 256, 12, 12 };
    float* const v109 = ((float*)(memory + 7022592));
    const int32_t v110[] = { 1, 256, 6, 6 };
    void* v111 = NULL;
    int32_t* v112 = NULL;
    int32_t v113[] = { 3, 3 };
    int32_t v114[] = { 0, 0, 1, 1 };
    int32_t v115[] = { 2, 2 };
    ONNC_RUNTIME_maxpool_float(NULL, ((float*)(v107)), 4, v108, ((float*)(v109)), 4, v110, ((float*)(v111)), 0, v112, "NOTSET", v113, 2, v114, 4, 0, v115, 2);
  }
  // Reshape
  {
    float* const v116 = ((float*)(memory + 7022592));
    const int32_t v117[] = { 1, 256, 6, 6 };
    float* const v118 = ONNC_RUNTIME_read_tensor((context->weight), 16).data;
    const int32_t v119[] = { 2 };
    float* const v120 = ((float*)(memory + 7059456));
    const int32_t v121[] = { 1, 9216 };
    ONNC_RUNTIME_reshape_float(NULL, ((float*)(v116)), 4, v117, ((float*)(v118)), 1, v119, ((float*)(v120)), 2, v121);
  }
  // Gemm
  {
    float* const v122 = ((float*)(memory + 7059456));
    const int32_t v123[] = { 1, 9216 };
    float* const v124 = ONNC_RUNTIME_read_tensor((context->weight), 10).data;
    const int32_t v125[] = { 4096, 9216 };
    float* const v126 = ONNC_RUNTIME_read_tensor((context->weight), 11).data;
    const int32_t v127[] = { 4096 };
    float* const v128 = ((float*)(memory + 7096320));
    const int32_t v129[] = { 1, 4096 };
    ONNC_RUNTIME_gemm_float(NULL, ((float*)(v122)), 2, v123, ((float*)(v124)), 2, v125, ((float*)(v126)), 1, v127, ((float*)(v128)), 2, v129, 1.000000, 1.000000, 0, 1);
  }
  // Relu
  {
    float* const v130 = ((float*)(memory + 7096320));
    const int32_t v131[] = { 1, 4096 };
    float* const v132 = ((float*)(memory + 7112704));
    const int32_t v133[] = { 1, 4096 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v130)), 2, v131, ((float*)(v132)), 2, v133);
  }
  // Gemm
  {
    float* const v134 = ((float*)(memory + 7112704));
    const int32_t v135[] = { 1, 4096 };
    float* const v136 = ONNC_RUNTIME_read_tensor((context->weight), 12).data;
    const int32_t v137[] = { 4096, 4096 };
    float* const v138 = ONNC_RUNTIME_read_tensor((context->weight), 13).data;
    const int32_t v139[] = { 4096 };
    float* const v140 = ((float*)(memory + 7129088));
    const int32_t v141[] = { 1, 4096 };
    ONNC_RUNTIME_gemm_float(NULL, ((float*)(v134)), 2, v135, ((float*)(v136)), 2, v137, ((float*)(v138)), 1, v139, ((float*)(v140)), 2, v141, 1.000000, 1.000000, 0, 1);
  }
  // Relu
  {
    float* const v142 = ((float*)(memory + 7129088));
    const int32_t v143[] = { 1, 4096 };
    float* const v144 = ((float*)(memory + 7145472));
    const int32_t v145[] = { 1, 4096 };
    ONNC_RUNTIME_relu_float(NULL, ((float*)(v142)), 2, v143, ((float*)(v144)), 2, v145);
  }
  // Gemm
  {
    float* const v146 = ((float*)(memory + 7145472));
    const int32_t v147[] = { 1, 4096 };
    float* const v148 = ONNC_RUNTIME_read_tensor((context->weight), 14).data;
    const int32_t v149[] = { 1000, 4096 };
    float* const v150 = ONNC_RUNTIME_read_tensor((context->weight), 15).data;
    const int32_t v151[] = { 1000 };
    float* const v152 = ((float*)(memory + 7161856));
    const int32_t v153[] = { 1, 1000 };
    ONNC_RUNTIME_gemm_float(NULL, ((float*)(v146)), 2, v147, ((float*)(v148)), 2, v149, ((float*)(v150)), 1, v151, ((float*)(v152)), 2, v153, 1.000000, 1.000000, 0, 1);
  }
  // Softmax
  {
    float* const v154 = ((float*)(memory + 7161856));
    const int32_t v155[] = { 1, 1000 };
    float* const v156 = ((float*)(memory + 7165856));
    const int32_t v157[] = { 1, 1000 };
    ONNC_RUNTIME_softmax_float(NULL, ((float*)(v154)), 2, v155, ((float*)(v156)), 2, v157, 1);
  }
  // OutputOperator
  {
    float* const v158 = ((float*)(memory + 7165856));
    struct ONNC_RUNTIME_tensor_view v159 = {
      .data = v158,
      .size = 4000
    };
    (context->completed)((context->id), v159);
  }
  ONNC_RUNTIME_terminate();
  free(memory);
  return 0;
}
