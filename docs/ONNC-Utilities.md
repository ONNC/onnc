# ONNC Utilities

Release PDF: [ONNC-Utilities.pdf](https://github.com/ONNC/onnc/wiki/files/1.0.0/ONNC-Utilities.pdf)

This application note lists current ONNC utilities. This document is compliant to the ONNC community Docker image v1.0. You may download the Docker image from the Docker Hub (https://hub.docker.com/r/onnc/onnc-community/)

## 1. Overview

An ONNC Docker image is available in the Docker Hub for fast deployment. It includes a pre-built ONNC source tree cloned from the [ONNC/onnc](https://github.com/ONNC/onnc) GitHub repository, pre-installed dependent libraries, and a ready-to-run working environment. Users may run benchmarks from the [ONNX Model Zoo](https://github.com/onnx/models) easily with this Docker image. For those who like to modify ONNC source code for research or product development, this document describes a couple of available utilities and scripts that are helpful in a common workflow using ONNC. The covered topics include the Docker image, built-in unit tests, benchmarking, and the detailed steps to rebuild modified source code.

## 2. Prerequisite

If Docker is not installed in your system, please download Docker (http://www.docker.com) and install it first. 

## 3. Get Docker Image

Pull the Docker image from the Docker Hub using the following shell command:

```console
$ docker pull onnc/onnc-community
```

## 4. Build ONNC with the Docker Image

Although the Docker image include a source code tree, it might not be the latest release version of ONNC. We strongly suggest you clone the latest version of ONNC from the GitHub repository, mount the source code directory to the Docker image, and modify the source code with your favorite editor on your host machine. You may clone the source code from the GitHub ONNC repository (https://github.com/ONNC/onnc).

```console
$ mkdir -p <source_dir> && cd <source_dir>
$ git clone https://github.com/ONNC/onnc.git
```

Once the latest source code is ready, you may invoke the following command to enter ONNC build environment:

```console
$ docker run -ti --rm --cap-add=SYS_PTRACE -v <source_dir>/onnc:/onnc/onnc onnc/onnc-community
```

`<source_dir>` is the directory where you cloned the latest ONNC source code and the `-ti` option provides a interactive interface for the container, the `-v` option mounts the directory to the Docker image, the `--cap-add=SYS_PTRACE` option enables debug support (e.g. gdb) in the container. You can make some change to the source code (`<source_dir>/onnc`) and run the following command to build ONNC.
 
```console
// run in the container cli, under build directory `/onnc/onnc-umbrella/build-normal` by default
$ smake -j8 install
```

The `smake` command synchronizes the build directory with `<source_dir>/onnc` and invokes the `make` command to build ONNC. The `-j8` option is to parallelize building with 8 cores.
This command will automatically install the compiled binary in this container environment.


### Configure Build Environment

The ONNC project uses CMake as its building system. There is a pre-defined build directory `build-normal` in the container and you may create another build variant (e.g. for debugging ONNC tools) using the following command:

```console
// run in the container cli
$ cd /onnc/onnc-umbrella
$ ssync && ./build.cmake.sh <build_mode>
```

`<build_mode>` supports one of the following modes:

```
normal, dbg, rgn, opt 
```

The `ssync` command synchronize the build directory with the `<source_dir>/onnc` diretory. The `build.cmake.sh` script creates a directory named `build-<build_mode>` for generated files. The mapping between the `<build_mode>` and CMake build type variable `CMAKE_BUILD_TYPE` is listed as below.

| Build Mode | CMAKE_BUILD_TYPE |
|------------|------------------|
| normal     | Release          |
| dbg        | Debug            |
| rgn        | Regression       |
| opt        | Optimized        |

**Table 1. CMAKE_BUILD_TYPE and build mode mapping**

## 5. Prepare Virtual Platform for ONNC verification

ONNC has mad some changes on [nvdla/sw](https://github.com/nvdla/sw/commit/1f44c2463f5473e00f6f9b63da50aeb878bd8825) for supporting ONNX models. You can checkout it and apply patch then build virtual platform.

```console
$ git clone https://github.com/nvdla/sw.git && cd sw
$ git checkout 1f44c24
$ git apply <source_dir>/onnc/nvdla/sw.diff
```

Reference following NVDLA official documents to build virtual platform from scratch:

- [Build linux kernel for virtual platform](http://nvdla.org/vp.html#building-linux-kernel-for-nvdla-virtual-simulator)
- [Build User Mode Driver & Kernel Mode Driver](https://github.com/nvdla/sw#nvdla-open-source-software)

Or just download modified virtual platform docker image:

```console
$ docker pull onnc/vp
```

## 6. Regression Tests

### Run Unit Tests

There are 23 unit tests available in the ONNC repository. Those tests are written in C++ language and you may run all of them using the following shell command:

```console
// run in the container cli
$ ctest
```
  
If all tests pass, you will see the same output as shown below.

```
Test project /onnc/onnc-umbrella/build-normal
      Start  1: Digraph
 1/23 Test  #1: Digraph ..........................   Passed    0.00 sec
      Start  2: FileHandle
 2/23 Test  #2: FileHandle .......................   Passed    0.00 sec
      Start  3: PassManager
 3/23 Test  #3: PassManager ......................   Passed    0.00 sec
      Start  4: Quadruple
 4/23 Test  #4: Quadruple ........................   Passed    0.00 sec
      Start  5: StringRef
 5/23 Test  #5: StringRef ........................   Passed    0.00 sec
      Start  6: Any
 6/23 Test  #6: Any ..............................   Passed    0.00 sec
      Start  7: BinaryTree
 7/23 Test  #7: BinaryTree .......................   Passed    0.00 sec
      Start  8: StringSwitch
 8/23 Test  #8: StringSwitch .....................   Passed    0.00 sec
      Start  9: StringMap
 9/23 Test  #9: StringMap ........................   Passed    0.69 sec
      Start 10: Json
10/23 Test #10: Json .............................   Passed    0.00 sec
      Start 11: ComputeIR
11/23 Test #11: ComputeIR ........................   Passed    0.00 sec
      Start 12: TensorSel
12/23 Test #12: TensorSel ........................   Passed    0.09 sec
      Start 13: StatisticsTest
13/23 Test #13: StatisticsTest ...................   Passed    0.00 sec
      Start 14: MemAllocTest
14/23 Test #14: MemAllocTest .....................   Passed    0.01 sec
      Start 15: CounterTest
15/23 Test #15: CounterTest ......................   Passed    0.00 sec
      Start 16: DivideGlobalAPIntoAPs
16/23 Test #16: DivideGlobalAPIntoAPs ............   Passed    0.00 sec
      Start 17: EliminateIdentityTest
17/23 Test #17: EliminateIdentityTest ............   Passed    0.00 sec
      Start 18: PropagateConstWithDiffShape
18/23 Test #18: PropagateConstWithDiffShape ......   Passed    0.00 sec
      Start 19: ReplaceGemmByConv
19/23 Test #19: ReplaceGemmByConv ................   Passed    0.00 sec
      Start 20: SplitConv
20/23 Test #20: SplitConv ........................   Passed    0.01 sec
      Start 21: Runtime_Abs
21/23 Test #21: Runtime_Abs ......................   Passed    0.00 sec
      Start 22: Runtime_Transpose
22/23 Test #22: Runtime_Transpose ................   Passed    0.00 sec
      Start 23: onnx2tg
23/23 Test #23: onnx2tg ..........................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 23

Total Test time (real) =   0.84 sec
```


If you like to run a single unit test, you may run it interactively in the Docker prompt.

```console
// In the container cli, run a single unit test. 
$ ./tools/unittests/unittest_<test_name>
// e.g.
$ ./tools/unittests/unittest_Json
// You may leave the Docker container by typing ‘exit’ in the Docker shell prompt. 
$ exit
```

Table 2 lists the 23 available unit tests in the ONNC repository.

| Test Name                   |
| --------------------------- |
| Digraph                     |
| FileHandle                  |
| PassManager                 |
| Quadruple                   |
| StringRef                   |
| Any                         |
| BinaryTree                  |
| StringSwitch                |
| StringMap                   |
| Json                        |
| ComputeIR                   |
| TensorSel                   |
| StatisticsTest              |
| MemAllocTest                |
| CounterTest                 |
| DivideGlobalAPIntoAPs       |
| EliminateIdentityTest       |
| PropagateConstWithDiffShape |
| ReplaceGemmByConv           |
| SplitConv                   |
| Runtime_Abs                 |
| Runtime_Transpose           |
| onnx2tg                     |

**Table 2. A list of unit tests in ONNC**

### Run Single-Layer Tests
  
   In addition to the unit tests, there is a set of single-layer ONNX models in the single_layer_test directory for those who like to hack ONNC for their own purposes. Users may run the regression by compiling a single-layer ONNX model into a Loadable and running the Loadable on the virtual platform from Section 5.

Run following script to generating Loadable files for each test models:

```console
# in the onnc/onnc-community container cli
$ <source_dir>/onnc/single_layer_test/compile-models.sh
```

Run virtual platform and Single-Layer test

```console
$ docker run -it -v <source_dir>/onnc/single_layer_test:/usr/local/nvdla/single_layer_test onnc/vp

# in the onnc/vp container cli
$ cd /usr/local/nvdla
$ aarch64_toplevel -c aarch64_nvdla.lua

# login by username: root, password: nvdla
$ mount -t 9p -o trans=virtio r /mnt
$ cd /mnt && ./init_dla.sh
$ ./nvdla_runtime --loadable single_layer_test/Add/4983_Add.nvdla --image single_layer_test/Add/4983_Add.pgm --rawdump

# check result with the sample output
$ diff -b output.dimg single_layer_test/Add/4983_Add.dimg && echo pass
```

## 7. Benchmarking using models from the ONNX Model Zoo

The Docker image includes a set of 12 pre-trained models from the ONNX model zoo (https://onnx.ai/) listed in Table 3. You may access the model files and associated input files in the /models directory. Or you may create your own model in the ONNX format, with input data in the ONNX TensorProto format (https://github.com/onnx/onnx/blob/master/docs/IR.md). 

| Model Name                   |
| ---------------------------- |
| bvlc_alexnet                 |
| bvlc_googlenet               |
| bvlc_reference_caffenet      |
| bvlc_reference_rcnn_ilsvrc13 |
| densenet121                  |
| inception_v1                 |
| inception_v2                 |
| resnet50                     |
| shufflenet                   |
| squeezenet                   |
| vgg19                        |
| zfnet512                     |

**Table 3. A list of ONNX models from the model zoo**

### Running a Single Benchmark

You may run a single model for benchmarking using the following shell command:

```console
// run in the container cli
$ onni <model_file_path>/model.onnx <input_file_path>/input_0.pb -verbose=<level>
```

where `<model_file_path>` is the path to the model file for the pre-trained ONNX model and `<input_file_path>` is the path to the corresponding input file. In the ONNC Docker container, the model file path is `/models/<model_name>` and the input file path is `/models/<model_name>/test_data_set_<0~6>`.  `<level>` indicates different levels of verbose information. Higher-level information is a superset of all lower-level information. For example, level 4 will include all information from level 1 to level 4.

Information for each verbose level:

- Level 1: Inference time & memory usage
- Level 2: ONNX operator statistics
- Level 3: Inference time & ONNX operator statistics per layer
- Level 4: Memory allocation log

Here is an example of running AlexNet and printing out all information. 

```console
// run in the container cli
$ onni /models/bvlc_alexnet/model.onnx /models/bvlc_alexnet/test_data_set_0/input_0.pb -verbose=4
```

### Running All Benchmarks using script

If you like to run all benchmarks in the model zoo, a `run-benchmark.sh` script is included in the Docker image to simplify your job. The script will compile a model and run inference for all models one by one. You may type `run-benchmark.sh -h` to get the following usage description. 

```console
Usage: run-benchmark.sh [options]... MODEL [ARGUMENTS...]
    --rebuild           Rebuild the source code
                        (Will build the source in the /onnc/onnc directory)
    MODEL               Any model from the ONNX model zoo
    ARGUMENTS           Arguments be passed to target
========================================
ONNX model list (from the ONNX model zoo):
bvlc_alexnet  bvlc_googlenet  bvlc_reference_caffenet  bvlc_reference_rcnn_ilsvrc13  densenet121  inception_v1  inception_v2  resnet50  shufflenet  squeezenet  vgg19  zfnet512
```

`run-benchmark.sh` has an optional flag to rebuild the ONNC source code before benchmarking. More details will be covered in the next section. 
  
You may run the script from the Docker prompt as simple as the following example:

```console
// run in the container cli
$ run-benchmark.sh --rebuild bvlc_alexnet 
```

You may also run the script from the shell prompt on your host machine using the following shell command:

```console
// run on the host machine
$ docker run -it --rm -v <source_dir>/onnc:/onnc/onnc onnc/onnc-community run-benchmark.sh --rebuild <model name>
```

Running Customized ONNX Models

Users may create their own ONNX model. In that case, they can run the benchmark using the following command:

```console
// run in the container cli
$ onni <model_directory>/<model_file> <model_directory>/<input_file>
```

## 8. Collecting statistics data via ONNC Statistics API

ONNC provides a set of Statistics classes and APIs in its framework for users to collect and share statistics data across ONNC source code.

For more information, please refer to the [ONNC Statistics API](./api/statistics.md) document. It will keep updating as more statistics types and APIs are defined and implemented.

