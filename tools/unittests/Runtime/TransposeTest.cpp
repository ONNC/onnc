#include <skypat/skypat.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define restrict __restrict__
extern "C"{
    #include <onnc/Runtime/operator/transpose.h>
}
#undef restrict

SKYPAT_F(Operator_Transpose, non_broadcast){
    // Prepare
    srand(time(NULL));
    int32_t dims[3]{3, 3, 3};
    float A[27]{
        000.0, 001.0, 002.0,
        010.0, 011.0, 012.0,
        020.0, 021.0, 022.0,

        100.0, 101.0, 102.0,
        110.0, 111.0, 112.0,
        120.0, 121.0, 122.0,

        200.0, 201.0, 202.0,
        210.0, 211.0, 212.0,
        220.0, 221.0, 222.0
    };
    float B[27];
    float Ans[27]{
        000.0, 100.0, 200.0,
        001.0, 101.0, 201.0,
        002.0, 102.0, 202.0,

        010.0, 110.0, 210.0,
        011.0, 111.0, 211.0,
        012.0, 112.0, 212.0,

        020.0, 120.0, 220.0,
        021.0, 121.0, 221.0,
        022.0, 122.0, 222.0
    };
    int32_t perm[3]{2, 0, 1};
    // Run
    ONNC_RUNTIME_transpose_float(NULL
        ,A
        ,3,dims
        ,B
        ,3,dims
        ,perm, 3
    );
    // Check
    for(int32_t i = 0; i < 27; ++i){
        EXPECT_EQ(B[i], Ans[i]);
    }
}
