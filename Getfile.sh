#!/bin/bash
echo Get the file from docker to local
dir=../build-normal
cp $dir/cortexm_out.cpp $dir/cortexm_out.h $dir/cortexm_input_pre_proc.h $dir/cortexm_weight.h .
