#include "matmul.h"

void matmul(q7_t* input_1,int input_1_dim[2],q7_t* input_2,int input_2_dim[2],q7_t* output,int right_shift){
    if(input_1_dim[1] == input_2_dim[0]){
        int index = 0;
        for(int input1_high = 0;input1_high < input_1_dim[0];input1_high++){
            for(int input2_weight = 0;input2_weight < input_2_dim[1];input2_weight++){
                int sum = 0;
                for(int loop = 0;loop < input_1_dim[1];loop++){
                    sum += input_1[loop + input1_high * input_1_dim[0] ] * input_2[ loop * input_2_dim[1] + input2_weight ];
                }
                output[index] = sum >> right_shift;
                index++;
            }
        }
    }
}
