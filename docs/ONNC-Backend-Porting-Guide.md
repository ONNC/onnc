# ONNC Backend Developer Guide

Release PDF: [ONNC-Backend-Developer-Guide.pdf](https://github.com/ONNC/onnc/wiki/files/1.0.0/ONNC-Backend-Porting-Guide.pdf)

This application note demonstrates how to create an ONNC backend for a target hardware. This document is compliant to the ONNC community Docker image v1.0. You may download the Docker image from the Docker Hub (https://hub.docker.com/r/onnc/onnc-community/).

## 1. Introduction

ONNC is a collection of open source, modular, reusable compiler algorithms, and tool chains targeted on deep learning accelerators (DLAs). ONNC has been built from ground up for translating ONNX intermediate representations (IRs) to proprietary DLA code. Its software architecture design emphasizes portability and reusability, thus simplifying retargeting. Figure 1 depicts the top-level block diagram of ONNC software stacks. The software stack illustrates the functional blocks from importing an ONNX computation graph model to emitting corresponding hardware binaries. In addition to leveraging the LLVM backend, ONNC paves another fast track for proprietary DLAs to execute ONNX models by defining ONNC IR, an intermediate representation (IR) that has one-to-one mapping to the ONNX IR. Two other popular compilation frameworks in deep learning systems, TVM and Glow, built their software stacks on top of the LLVM backend. The intermediate representations of LLVM have a finer granularity than ONNC IRs while mapping to hardware operators. For accelerators built with coarse-grained operators such as convolution, it requires more porting efforts to hack the LLVM backend. Many DLA designs such as Nvidia’s [NVDLA](http://nvdla.org/) and Bitman’s [Sophon BM168X series](https://sophon.ai/product/introduce/bm1682.html) favor coarse-grained operators over LLVM operators. In those cases, ONNC provides a more straightforward way to convert ONNX models to target binaries using its own Vanilla backend. thus speeding up porting a compiler to new hardware. For fast porting, users only need to copy the Vanilla backend as a template, override two software pipes at minimum, add optional optimization passes and the framework will handle the rest of work like a charm.

![](https://github.com/ONNC/onnc/wiki/files/1.2.0/onnc-software-architecture-diagram.png)

**Figure 1. ONNC Software Architecture Diagram**
 

## 2. Quick Start

ONNC provides a script to quickly derive a backend “skeleton”. This skeleton does nothing useful but shows the minimum runnable code of a backend. You can start the backend development based on the skeleton. In this section we introduce how to run that script to generate the skeleton and how to compile the generated backend. 


### 2.1. Getting the Docker Image

We provide a Docker image to facilitate ONNC building process. Please make sure the Docker program (http://www.docker.com) is installed in your computer before you move forward. 

Then get the ONNC Docker image by the following shell command.
```console
$ docker pull onnc/onnc-community
```

### 2.2. Getting the ONNC source codes

The latest ONNC source code is available on GitHub. Follow these commands to download the source code.

```console
$ git clone https://github.com/ONNC/onnc.git
```

### 2.3. Creating a new backend

There is a script in the ONNC-community Docker for new backend creation. To run that script, please enter the Ubuntu prompt within the ONNC Docker. 

```console
// Use the interactive mode to enter the Docker prompt.
$ docker run -ti --rm -v <onnc_source_dir>:/onnc/onnc onnc/onnc-community
```

Option `-v <onnc_source_dir>:/onnc/onnc` means to mount the ONNC source code to the Docker container. The path before the colon `<onnc_source_dir>` indicates the absolute path to the downloaded ONNC sources code. The path after the colon `/onnc/onnc` indicates the absolute path in the Docker container. 

Option `-ti` means to enable the interactive mode which will enter the Ubuntu shell prompt in the Docker container.

The last argument `onnc/onnc-community` is the name of the ONNC Docker image. After typing the above command, you should see the Docker prompt as follows.

```console
$ docker run -ti --rm -v ~/work/onnc_projects/onnc:/onnc/onnc onnc/onnc-community
onnc@1fda72903b5c:/onnc/onnc-umbrella/build-normal$ 
```
 
Within the Docker prompt, type the following commands to create a new backend called Foo.

```console
// Within the Docker prompt, go to the path where the source codes are mounted to.
$ cd /onnc/onnc

// Run the script to create a new backend called Foo.
$ ./scripts/create-new-backend.sh Foo
```

The source code of the new backend, Foo, will appear in the folder `/onnc/onnc/lib/Target/Foo`. Actually, that folder is copied from the template folder `/onnc/onnc/lib/Target/Vanilla` and adjust to the new backend name accordingly. 

In case you want to exit the Docker prompt, use the following command.

```console
$ exit
```

### 2.4. Compiling the new backend

Once the new backend is created, you must recompile ONNC to activate that new backend. Use the following commands to compile ONNC as well as the new backend.

```console
// Do the following within the Docker prompt.
$ cd /onnc/onnc-umbrella/build-normal/

// Use “-j8” to invoke 8 CPU cores to do the parallel compilation.
$ smake -j8 install
```

When executing those commands successfully, you should see results as the following.

```console
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/Rope.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/HashTable.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/StringSwitch.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/NodeIterator.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/Flags.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/IListNode.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/StringHasher.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/StringHashTable.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/TypeSwitch.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/ADT/If.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Notation.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Reader.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Array.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Type.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Group.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Storage.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Value.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/String.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/JSON/Object.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/Config
-- Installing: /onnc/onnc-umbrella/install-normal/include/onnc/Config/Config.h
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/Config/ONNX.h
-- Installing: /onnc/onnc-umbrella/install-normal/include/onnc/Config/Backends.def
-- Installing: /onnc/onnc-umbrella/install-normal/include/onnc/Config/Platforms.def
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/Support
-- Up-to-date: /onnc/onnc-umbrella/install-normal/include/onnc/Support/DataTypes.h
onnc@705b08cf8a7d:/onnc/onnc-umbrella/build-normal$
```

### 2.5. Invoking the new backend

The following commands demonstrate processing the AlexNet model with the new backend. 

```console
// Do the following within the Docker prompt.
$ cd /onnc/onnc-umbrella/build-normal/

// Invoke the new backend Foo.
$ ./tools/onnc/onnc /models/bvlc_alexnet/model.onnx -mquadruple foo
```

Option `-mquadruple foo`<sup id="quadruple-1">[1](#quadruple)</sup> is for invoking the new backend Foo. Use all lowercase letters as the backend name in this option. 

The following console output shows the execution result of the new backend. It prints information of the operators in the AlexNet model. 

```console
onnc@705b08cf8a7d:/onnc/onnc-umbrella/build-normal$ ./tools/onnc/onnc /models/bvlc_alexnet/model.onnx -mquadruple foo
Foo is invoked
%conv1_w_0<float>[96, 3, 11, 11] = Initializer<unimplemented>()
%conv1_b_0<float>[96] = Initializer<unimplemented>()
%conv2_w_0<float>[256, 48, 5, 5] = Initializer<unimplemented>()
%conv2_b_0<float>[256] = Initializer<unimplemented>()
%conv3_w_0<float>[384, 256, 3, 3] = Initializer<unimplemented>()
%conv3_b_0<float>[384] = Initializer<unimplemented>()
%conv4_w_0<float>[384, 192, 3, 3] = Initializer<unimplemented>()
%conv4_b_0<float>[384] = Initializer<unimplemented>()
%conv5_w_0<float>[256, 192, 3, 3] = Initializer<unimplemented>()
%conv5_b_0<float>[256] = Initializer<unimplemented>()
%fc6_w_0<float>[4096, 9216] = Initializer<unimplemented>()
%fc6_b_0<float>[4096] = Initializer<unimplemented>()
%fc7_w_0<float>[4096, 4096] = Initializer<unimplemented>()
%fc7_b_0<float>[4096] = Initializer<unimplemented>()
%fc8_w_0<float>[1000, 4096] = Initializer<unimplemented>()
%fc8_b_0<float>[1000] = Initializer<unimplemented>()
%OC2_DUMMY_1<int64>[2] = Initializer<unimplemented>()
%data_0<float>[1, 3, 224, 224] = InputOperator<unimplemented>()
%conv1_1<float>[1, 96, 54, 54] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 1, kernel_shape: [11, 11], pads: [0, 0, 0, 0], strides: [4, 4]>(%data_0<float>[1, 3, 224, 224], %conv1_w_0<float>[96, 3, 11, 11], %conv1_b_0<float>[96])
%conv2_1<float>[1, 256, 26, 26] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 2, kernel_shape: [5, 5], pads: [2, 2, 2, 2], strides: [1, 1]>(%pool1_1<float>[1, 96, 26, 26], %conv2_w_0<float>[256, 48, 5, 5], %conv2_b_0<float>[256])
%conv3_1<float>[1, 384, 12, 12] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 1, kernel_shape: [3, 3], pads: [1, 1, 1, 1], strides: [1, 1]>(%pool2_1<float>[1, 256, 12, 12], %conv3_w_0<float>[384, 256, 3, 3], %conv3_b_0<float>[384])
%conv4_1<float>[1, 384, 12, 12] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 2, kernel_shape: [3, 3], pads: [1, 1, 1, 1], strides: [1, 1]>(%conv3_2<float>[1, 384, 12, 12], %conv4_w_0<float>[384, 192, 3, 3], %conv4_b_0<float>[384])
%conv5_1<float>[1, 256, 12, 12] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 2, kernel_shape: [3, 3], pads: [1, 1, 1, 1], strides: [1, 1]>(%conv4_2<float>[1, 384, 12, 12], %conv5_w_0<float>[256, 192, 3, 3], %conv5_b_0<float>[256])
 = OutputOperator<unimplemented>(%prob_1<float>[1, 1000])
onnc@705b08cf8a7d:/onnc/onnc-umbrella/build-normal$
```

## 3. Files of the new backend

By following the commands in the previous section, we derive a new backend Foo and all the files are created under `/onnc/onnc/lib/Target/Foo`. Table 1 describes the files in the created folder. 

**Table 1: Files of a backend and their purposes.**

| File | Purpose |
| ---- | ------- |
| `FooBackend.cpp & .h` | The main file of a backend. Developers need to modify this file to add optimization passes. |
| `CodeEmitVisitor.cpp & .h` | Implementation of the `CodeEmitVisitor` class. Developers need to modify this file to handle the code generation for each operator. |
| `TargetInfo/FooTargetInfo.cpp & .h` | This file containing functions for registering this backend to the ONNC framework. |
| `TargetInfo/FooTargetMemInfo.cpp & .h` | The file for configuring  memory size and alignment for each data type in neural network models. Developers need to modify this file based on the target hardware attributes to optimize memory allocation. |
| `CMakeLists.txt` | Configuration file for the CMake building system. |
| `Makefile.am` | Configuration file for the Autotools building system. |


## 4. Customizing the new backend


### 4.1. Extending ONNC IR for Unsupported Operators

To support the target-specific operators that does not exist in the default ONNC IR, please refer to the application note, [ONNC IR Extension Guide](ONNC-IR-Extension-Guide.md), for more details.

### 4.2. Handling code generation in the `codeEmit` pass
The `codeEmit` pass is added to handle code generation for the target backend. Please refer to [The Code Emitting Pass User Guide](The-Code-Emitting-Pass-User-Guide.md) for more details.

### 4.3. Adding optimization passes

The generated backend has included some optimization algorithms by default. Each algorithm is implemented in ONNC as a “pass” (the same concept as the LLVM pass). The default optimization passes may not fit your need so you need to develop your own passes and then edit `FooBackend.cpp` to add those passes into the compilation flow. Below is an example of adding a pass. Refer to the application note, [Pass Manager Getting Started Guide](ONNC-Pass-Manager-Getting-Started-Guide.md),  for more details about how to add a pass.

```cpp
void FooBackend::addOnncIrOptimization(PassManager& pPM, OptimizationOptions& options)
{
  TargetBackend::addOnncIrOptimization(pPM, options);
  // One example of adding your optimization pass. 
  pPM.add<YourProprietaryPass>();
}
```
**Code Snippet 2. Example of adding an optimization pass into a backend.**
 
In the above example, the optimization pass is added in the method, `addOnncIrOptimization()`. There are five stages in the compilation flow for users to add passes. Each stage in the compilation flow is implemented in a corresponding method. The following table shows the meaning and input/output of each method. 

**Table 2. The five methods representing the five compilation phases.**

| Method | Input | Output | Description |
| ------ | ----- | ------ | ----------- |
| `addTensorSel` | **ONNX IR** | **ONNC IR** | This method contains passes for translating models in the ONNX format into ONNC IR. |
| `addOnncIrOptimization` | **ONNC IR** | **ONNC IR** in optimized order | This method contains passes for optimizing ONNC IR in order to better performance. |
| `addTensorSched` | **ONNC IR** in optimized order | **ONNC IR** in optimized order | This method contains passes for better scheduling the execution order of ONNC IR. |
| `addMemAlloc` | **ONNC IR** in optimized order | **ONNC IR** with addresses | This method contains passes for allocating memory space of input data, weights, and activation data. |
| `addCodeEmit` | **ONNC IR** with address | **Machine codes** | This method contains passes for handling code generation and optimization. |

ONNC provides a couple of basic optimization passes by default. They are listed in the following table. 

**Table 3. The default passes in each optimization phase.**

| Method | Default passes | Description |
| ------ | -------------- | ----------- |
| `addTensorSel` | `addStandardTensorSel` | This pass translates models in the ONNX format into ONNC IR. |
| `addTensorSched` | N/A | |
| `addMemAlloc` | `addStandardCreateLiveIntervals` | This pass calculates the liveness intervals of tensors (input/output of operators). |
| `addMemAlloc` | `addStandardMemoryAllocation` | This pass allocates addresses for tensors with the consideration of tensors’ liveness intervals. |
| `addMemAlloc` | `addStandardSetMemOperands` | This pass saves the result of memory allocation to an internal data structure called `MemoryOperand` so that the result can be accessed by other passes. |
| `addCodeEmit` | `CodeEmit` | This pass generates the target machine codes. Note that this pass initially is just empty, which needs backend developers to add target-dependent implementation. |

### 4.4. Rebuilding ONNC 
After all modification is done in the new backend, remember to rebuild ONNC by following the steps in Section 2.4. 


----

<b id="quadruple">[1]</b>: `Quadruple` in ONNC is a string used for encapsulating backend information. The string can contain rich target information such as version, vendor, operating system, toolchain, and etc. A funny example is `-mquadruple foo-apple-darwin-gnu-ar-0.9.3-onnc-ca7`. Here we set our quadruple string to contain only one information: a special name `foo` to represent the target backend. This is also the minimum requirement of such a string. [↩](#quadruple-1)
