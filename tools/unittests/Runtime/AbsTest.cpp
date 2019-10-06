#include <skypat/skypat.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

#define restrict __restrict__
extern "C"{
    #include <onnc/Runtime/operator/abs.h>
}
#undef restrict

SKYPAT_F(Operator_Abs, non_broadcast){
    // Prepare
    srand(time(NULL));
    int32_t ndim = rand() % 3 + 1;
    std::vector<int32_t> dims(ndim);
    int32_t dataSize = 1;
    for(int32_t i = 0; i < ndim; ++i){
        dims[i] = rand() % 100 + 1;
        dataSize *= dims[i];
    }
    std::vector<float> A(dataSize), B(dataSize), Ans(dataSize);
    for(int32_t i = 0; i < dataSize; ++i){
        A[i] = rand() % 1000 / 100;
        Ans[i] = (A[i] > 0) ? A[i] : -A[i];
    }
    // Run
    ONNC_RUNTIME_abs_float(NULL
        ,A.data()
        ,ndim,dims.data()
        ,B.data()
        ,ndim,dims.data()
    );
    // Check
    for(int32_t i = 0; i < dataSize; ++i){
        EXPECT_EQ(B[i], Ans[i]);
    }
}
