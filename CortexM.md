# ONNC-CortexM Backend

## Overview

This is a joint project created by Skymizer Taiwan Inc. and SSLAB in the National Chung Cheng University. The goal is to create a Cortext-M backend to support the the [mnist model](https://github.com/onnx/models/tree/master/vision/classification/mnist) integrated with the [CMSIS-NN library](https://github.com/ARM-software/CMSIS_5) running on a Cortext-M microproessor.


## Acknowledgement

This project is sponsored by Skymizer Taiwan Inc.

We thank Professor Rong-Guey Chang in the National Chung Cheng University for mentoring the project.
We thank the following students in the SSLAB for contributing to the source code:

* 王聖融 (Sheng-Rong Wang)
* 孫仕寧 (Shin-Ning Sun)
* 黃兆瑋 (Chao-wei Huang)
* Tam (Van Tam Hoang)
* 林丞熙 (Cheng-Hsi Lin)
* 盧怡靜 (Yi-Jing LU)

We thank Dr. Cheng-Tao Hsieh who provided insight and expertise that greatly assisted the project, and Dr. Wei-Fen Lin for comments and revision that greatly improved the demonstration and tutorial.

We would also like to show our gratitude to anonymous reviewers for their feedback.

Skymizer will have a demonstration of this project in the [\[MICRO 2019 Tutorial: ONNC Compiler Porting and Optimization for NVDLA-Based Neural NEtwork Inference Engines\]. Welcome to join the tutorial for an live demonstration. 

## Digit Recognition with ARM CortexM

### Prerequisite

If Docker is not installed in your system, please download Docker (http://www.docker.com) and install it first. In addition, you need to install Git (https://git-scm.com/) to fetch the source code from the GitHub server. Furthermore, the demonstration uses a popular GUI programming framework, Processing, please install Processing (https://processing.org/) as well. Lastly, you need to prepare a development board equipped with ARM Cortex-M CPU. We suggest to use [Mbed compatible boards](https://os.mbed.com/platforms/) because we use the [Mbed framework](https://www.mbed.com/en/) for the firmware compilation. If your board is not compatible twith Mbed, you might need to rewrite some demonstration code following the regulation from the board vendor. 