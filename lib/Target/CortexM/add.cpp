#include "add.h"
#include <iostream>
#include <stdio.h>

using namespace std;

void MatAdd(q7_t *input, int *input_dim, q7_t *add, int *add_dim, q7_t *output,
            int number_of_input_dim, int right_shift, int add_shift) {
  if (number_of_input_dim == 4) {
    for (int loop_k = 0; loop_k < input_dim[0]; loop_k++) {
      for (int loop_h = 0; loop_h < input_dim[1]; loop_h++) {
        for (int loop_w = 0; loop_w < input_dim[2]; loop_w++) {
          for (int loop_c = 0; loop_c < input_dim[3]; loop_c++) {
            output[loop_k * input_dim[1] * input_dim[2] * input_dim[3] +
                   loop_h * input_dim[2] * input_dim[3] +
                   loop_w * input_dim[3] + loop_c] =
                (input[loop_k * input_dim[1] * input_dim[2] * input_dim[3] +
                       loop_h * input_dim[2] * input_dim[3] +
                       loop_w * input_dim[3] + loop_c] +
                 (add[loop_c] >> add_shift)) >>
                right_shift;
          }
        }
      }
    }
  } else {
    for (int loop_h = 0; loop_h < input_dim[0]; loop_h++) {
      for (int loop_w = 0; loop_w < input_dim[1]; loop_w++) {
        output[loop_h * input_dim[1] + loop_w] =
            (input[loop_h * input_dim[1] + loop_w] +
             (add[loop_h * input_dim[1] + loop_w] >> add_shift)) >>
            right_shift;
      }
    }
  }
}
