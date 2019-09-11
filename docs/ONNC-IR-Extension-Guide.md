# ONNC IR Extension Guide

Release PDF: [ONNC-IR-Extension-Guide.pdf](https://github.com/ONNC/onnc/wiki/files/1.0.0/ONNC-IR-Extension-Guide.pdf)

This application note describes how to extend the ONNC intermediate representation (IR) for the operators that  are supported by the target device but not provided by the default ONNC IR. This document is compliant to the ONNC community Docker image v1.0. You may download the Docker image from the [Docker Hub](https://hub.docker.com/r/onnc/onnc-community).

## 1. Introduction

ONNC (Open Neural Network Compiler) is a retargetable compilation framework designed specifically for proprietary deep learning accelerators. It supports ONNX (Open Neural Network Exchange) operators and its intermediate representation can be extended to support target specific operators. 

### Generic vs. target-specific operator

The ONNC IR contains two types of operators, generic and target-specific operators. Generic operators such as convolution, ReLU, max pooling, and etc. are commonly used in neural network models, whereas target-specific operators are usually designed for special purpose. A complete list of the generic operators covered in ONNC can be found in the source directory (https://github.com/ONNC/onnc/tree/master/include/onnc/IR/Compute). This directory contains C++ header files describing a top-level definition of each generic operator. Since the ONNC IRs have a one-to-one mapping to the ONNX IRs, you may refer to the ONNX website (https://github.com/onnx/onnx/blob/master/docs/Operators.md) for the details of each ONNC IR functionality. 

### Why target-specific operators?

Different from generic operators, target-specific operators are tailor-made operators for customized DLA hardware designs. A good example is a fused operator with a convolution followed by a ReLU. Certain DLA hardware like NVDLA (http://nvdla.org/) provides a special mode that fuses the two operators together and executes them in a pipeline. The fusion mode improves the performance and power consumption because the convolution output is redirected into the ReLU computing unit without costly passing through the system memory outside the DLA. In other words, the memory access requirement is eliminated. To take advantage of this mode, you need to extend the ONNC IR for the new type of compound operator such that the ONNC optimization passes can make an adjustment for the supported hardware feature. For example, if convolution and ReLu are fused together, no memory allocation is needed for the convolution output.

### When do we need target-specific operators?

There are several occasions when you might need to add target-specific operators into the ONNC IRs. We have presented the case of compound operators. Another case is the load and store operators. Some DLA hardware contains an internal memory like the scratchpad memory in the CPU area. Explicit load/store operators are provided to control the data movement between the internal and the outside system memory. They are also a common type of target-specific operators. In general, if the target hardware supports any command/feature/mode that cannot be described by generic operators, a target-specific operator should be implemented in the ONNC IR level for optimization or backend code generation. 

### Challenges of adding target-specific operators

There are two challenges to support target-specific operators in ONNC: 
1. Mapping a model into a computation graph that contains the target-specific operators.
2. Integrating the new operators into optimization passes in ONNC.

In ONNC, a given model is firstly parsed and translated into the ONNC IR. The ONNC IR is coded by the data structure of the graph, where a node stands for an operator and an edge for data dependency between operators. Initially, every node is a generic operator. The injection of any target-specific information is performed as a post-processing step on the initial graph. Briefly, it is to remove unnecessary generic operator nodes and insert target-specific operator nodes in the graph. The ONNC framework provides APIs for easing the editing of models.

The extended target-specific operators either fuse multiple generic operators or introduce new hardware features. ONNC represents a model using a computation graph. In either case, we need to transform a model into a computation graph that contains the new operators. For operator fusion, the ONNC framework provides APIs for easing the graph rewriting. For new hardware features, users need to define a new IR and modify the transformation from a model to the ONNC computation graph. 

Furthermore, some optimization passes must be modified to recognize the new operators. For example, the code emitting pass which translates each ONNC IR operator into the low-level machine codes requires adding new functions for translating the new operators. There might be other optimization passes that depend on IR design. It is always a good practice to check all the passes in use to make a proper modification for the IR extension. In addition, the ONNC framework tries to ease the extension task by adopting an appropriate design pattern which will be introduced later.

The rest of this application note documents step-by-step instructions for adding into ONNC IR a compound operator that fuses convolution and ReLU together. 

## 2. Prerequisite

All code examples presented below are included in a released Docker image. Installing Docker is a requirement for the hands-on tutorial in this application note. You may type the following shell command to test if Docker is installed in your computer. 

```console
$ docker -v
```

If Docker has not been installed yet, you may download it from its official website (http://www.docker.com). Once Docker is installed, you may get the released ONNC-Community Docker image using the following shell command:

```console
$ docker pull onnc/onnc-community
```

It will take a while to download since the Docker image size is around 6.68GB. This Docker image contains all the sample code in this tutorial. You can access the code by running Docker in the interactive mode. 

```console
// Run the Docker image in the interactive mode.
$ docker run -ti --rm onnc/onnc-community bash
```

After the above command, the Docker prompt is present as shown below:

```console
$ docker run -ti --rm onnc/onnc-community bash
onnc@ba5dc745e3b2:/onnc/onnc-umbrella/build-normal$
```

The Docker prompt is a Linux shell prompt. You may browse the project files using Linux commands. All the sample code is placed in the following folder:

```
<ONNC source folder>/lib/Target/X86
```

The default `<ONNC source folder>` is `/onnc/onnc` but the source code in that folder might be out of date. You may clone the latest ONNC repository and mount it to the Docker container to work on the latest ONNC tree. Please refer to section 4, "Build ONNC with the Docker Image", in [ONNC Utilities](ONNC-Utilities.md) for more details. Once you have source code ready, you may find related files listed in the following table for the sample code.

| Files | Description |
| ----- | ----------- |
| `Compute/X86ConvRelu.cpp` `Compute/X86ConvRelu.h`| Define the new compound operator of convolution plus ReLU. |
| `X86FuseConvRelu.cpp` `X86FuseConvRelu.h` | Implement an optimization pass for inserting the new compound operator into a model. |

To play around, you may edit the sample code using the pre-installed utilities in the Docker prompt. For example, 
```console
$ vim <ONNC source folder>/lib/Target/X86/X86FuseConvRelu.cpp
```
After making chages to the source code, you may rebuild ONNC by following the instructions in [ONNC Utilities](ONNC-Utilities.md).

The following command will exit the Docker prompt.
```console
$ exit
```
If you have no problem getting the above done, you are all set to go through the hands-on exercise for IR extension in this application note. The ONNC-Community Docker image contains many useful utilities to help ONNC developers and users. For further information, you may refer to the [ONNC Untilities](ONNC-Utilities.md) application note in the ONNC GitHub repository. 


## 3. Adding IR definition for a compound operator that fuses convolution and ReLU operations together

It is common to see a deep learning hardware that fuse the Conv and ReLU layers. To support hardware with this type of fusion, an efficient implementation in compiler is to extend its IR with a new compound operator. With this new operator, compiler may emit code that exploit this hardware feature to improve performance.

### Defining a new class for the new compound operator

In ONNC, each operator needs a new C++ class derived from a base class, `ComputeOperator`, to store operator-specific information including input, output, and operator attributes as shown in the Code Snippet 1. 

```cpp
class X86ConvRelu : public ComputeOperator
{
public:
  static char ID;

public:
  X86ConvRelu(Conv &pConv, Relu &pRelu)
    : ComputeOperator("X86ConvRelu", ID), m_Conv(pConv), m_Relu(pRelu) {
  }

  virtual ~X86ConvRelu() { }

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

  Conv m_Conv;
  Relu m_Relu;
};

} // namespace of onnc
```
**Code Snippet 1.** (from `lib/Target/X86/Compute/X86ConvRelu.h`) The C++ header file for the new compound operator, `X86ConvRelu`. 

### Implementing the new class for the compound operator 

  ONNC adopts the visitor design pattern for the supported operators. For each new operator derived from the `ComputeOperator` class, both `accept()` and `classof()` methods are mandatory. For debugging purpose, the `printAttributes()` function is also required. 

```cpp
#include "X86ConvRelu.h"

// Initially assign a zero value as an indication of invalidity. 
// The ONNC framework will re-assign a valid ID.
char X86ConvRelu::ID = 0;

void X86ConvRelu::printAttributes(std::ostream& pOS) const
{
  m_Conv.printAttributes(pOS);
  m_Relu.printAttributes(pOS);
}

// Method ‘accept’ comes from the visitor design pattern.
// The input parameter is a visitor object. 
void X86ConvRelu::accept(ComputeVisitor &pV)
{
  X86ComputeVisitor* visitor = dyn_cast<X86ComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}
void X86ConvRelu::accept(ComputeVisitor &pV) const
{
  X86ComputeVisitor* visitor = dyn_cast<X86ComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

// A member function for checking if a computeOperator is of 
// the X86ConvRelu type.
bool X86ConvRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}

```
**Code Snippet 2.** (from `lib/Target/X86/Compute/X86ConvRelu.cpp`) The implementation code for the new compound operator class, `X86ConvRelu`. 

Code Snippet 2 depicts the implementation of the class `X86ConvRelu`. Method `classof()` checks if the input operator is of the type `X86ConvRelu`. Method `accept()` is called in passes to perform optimization specific to this operator. Precisely, here we take advantage of the *visitor design pattern* (https://en.wikipedia.org/wiki/Visitor_pattern). The input to this method is a so-called visitor object. The visitor is called with operator `X86ConvRelu` itself as the input, so actually method `visit(X86ConvRelu& op)` of the visitor gets invoked due to *overloading* (https://en.wikipedia.org/wiki/Function_overloading) in C++. That `visit()` method contains optimization algorithm for the operator of `X86ConvRelu`. Note that in the visitor, every operator has its own `visit()` method about its specific optimization. For example, the code emitting pass contains a visitor for implementing code generation of each operator. Please refer to [The Code Emitting Pass User Guide](The-Code-Emitting-Pass-User-Guide.md) for details. 

## 4. Extending ONNC IR with the new compound operator

In the previous section we have added a new class `X86ConvRelu` to represent the new operator. Next, we will extend the ONNC IR with this new operator. The extension is carried out in a fusing pass. In this section, we describe how to implement this fusing pass and how to schedule this fusing pass to run. 

### Defining a new pass for the layer fusion

A new class derived from the `CustomPass` class template is defined for the layer fusion process. This pass provides a member function, `mergeConvRelu` to find a pattern matching the pair of Conv and ReLu, detach the pair from the network graph, and rewrite the network graph with an extra X86ConvRelu operator. 

```cpp
class X86FuseConvRelu : public CustomPass<X86FuseConvRelu>
{
public:
  ReturnType runOnModule(Module& pModule) override;

  .....
};

```
**Code Snippet 3.** (from `lib/Target/X86/X86FuseConvRelu.h`) The C++ header file for the new fusing pass.

### Implementing the operator-fusion pass
  For each pass derived from the `CustomPass` class template in ONNC, users need to at least override the `runOnModule` member function. The input to this pass is a network graph represented by a `ComputeGraph` object with nodes mapped to ONNC IRs. The goal is to find all the pairs of a Conv node followed by a ReLU node, and replace them by a new`X86ConvRelu` node. Code Snippet 4 and 5 show the detailed implementation.

```cpp
Pass::ReturnType X86FuseConvRelu::runOnComputeGraph(ComputeGraph& pCG)
{
  // ...
  // Loop over every operator of a given model.
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator* node = nodeIt;

    // Check if a convolution followed by a ReLU happens.
    if (!isFusible(*node))
      continue;

    // Yes, a convolution + ReLU case happens. 
    // Get those two generic operators and prepare to replace them.
    Conv& conv = *(Conv *)node;
    Relu& relu = *(Relu *)conv.getOutput(0)->getUses()[0].getUser();

    // After the call, the new operator appears in the model.
    mergeConvRelu(pCG, conv, relu);

    // Remove the two unused generic operators from the model.
    pCG.erase(conv);
    pCG.erase(relu);

    //...
  }
}
```
**Code Snippet 4.** (from `lib/Target/X86/X86FuseConvRelu.cpp`) The key algorithm in the `X86FuseConvRelu` pass. It searches for all the pairs of a Conv node followed by a ReLU node in the ONNC IR ComputeGraph, and replace those pairs with `X86ConvRelu` nodes  

```cpp
bool X86FuseConvRelu::isFusible(ComputeOperator& pNode)
{
  if (!isa<Conv>(&pNode))
    return false;

  Value* outv = pNode.getOutput(0);

  // if Conv's result has more than one users, we can't fuse it.
  if (outv->getUses().size() > 1)
    return false;

  ComputeOperator* userNode = outv->getUses()[0].getUser();
  if (!isa<Relu>(userNode))
    return false;
  return true;
}

X86ConvRelu* X86FuseConvRelu::mergeConvRelu(ComputeGraph& pCG,
                                            Conv& pConv, Relu& pRelu)
{
  // Remove the edges between the two old operators 
  // because after merging, those edges no longer have meaning.
  Value* outv = pRelu.getOutput(0);
  Value* out_conv = pConv.getOutput(0);
  pConv.replaceOutput(0, *outv);
  pCG.erase(*out_conv);

  // Create a new compound operator X86ConvRelu.
  X86ConvRelu* newOp = pCG.addOperator<X86ConvRelu>(pConv, pRelu);
  Value* emptyV = new Value;

  // Reconnect input edges from the old operators to the new operator.
  for (unsigned i = 0; i < pConv.getNumOfInputs(); ++i) {
    newOp->addInput(*pConv.getInput(i));

    pConv.replaceInput(i, *emptyV);
  }
  pRelu.replaceInput(0, *emptyV);

  // Reconnect output edges to the new operator.
  outv->clearDefine();
  newOp->addOutput(*outv);

  return newOp;
}
```
**Code Snippets 5.** (from `X86FuseConvRelu.cpp`): Two private member functions,`isFusible` and `mergeConvRelu`, use the APIs from the ONNC framework to manipulate the ComputeGraph.

In addition, a function `onnc::CreateX86FuseConvReluPass` is added to creates a `X86FuseConvRelu` pass instance and return its pointer. 

```cpp
X86FuseConvRelu* onnc::CreateX86FuseConvReluPass()
{
  return new X86FuseConvRelu();
}
```
**Code Snippet 6.** (from `X86FuseConvRelu.cpp`) The `CreateX86FuseConvReluPass` function simply instantiates the `X86FuseConvRelu` pass and returns the pass instance.

### Scheduling the operator-fusing pass to run

The usage and the execution order of passes are described in a file named `lib/Target/<target>/<target>Backend.cpp`, where `<target>` is the target hardware name. For example, for the target X86 CPU, it would be the file `lib/Target/X86/X86Backend.cpp`. In this file, a set of passes are created, and the creation order will reflect the execution order. Therefore, it is important to know the right place to create a pass in the file. Assume that we have a pass called `X86FuseConvRelu` for inserting operator `X86ConvRelu` into a model. The following code snippet shows how this pass is inserted into the `addTensorSel()` phase. 

```cpp
#include "X86FuseConvRelu.h"

// This method is for collecting passes related to lowering ONNX IR into ONNC IR.
void X86Backend::addTensorSel(PassManager& pPM)
{
  // This ONNC built-in pass translates a model in ONNX into ONNC IR operators 
  // based on only generic operators without knowledge of target-specific operators.
  addStandardTensorSel(pPM, *this);

  if (EnableX86FuseConvRelu) {
    // Target-specific operators are inserted within this user-provided pass.
    pPM.add(CreateX86FuseConvReluPass());
  }
}
```
**Code Snippet 7.** (`X86Backend.cpp`) Creating the `X86FuseConvRelu` pass to add the new compound operator into a model.

In Code Snippet 7, the function `CreateX86FuseConvReluPass()` is called to create the target pass. There are two things we like to point out. First, this function call is placed within method `addTensorSel()`. Second, the function call is inserted after the built-in pass `addStandardTensorSel()`. The method `addTensorSel()` represents one of the four optimization phases in ONNC, along with `addTensorSched()`, `addMemAlloc()`, and `addCodeEmit()`. It is the first phase and responsible for translating a given model into the ONNC IR. Since our pass of supporting new operators is a part of the IR translation, it must be added in the method of `addTensorSel()`. In addition, the built-in pass `addStandardTensorSel()` has no knowledge of any new target-specific operators, so it will not take the new operators into consideration within the translated ONNC IR. Our operator-adding pass is performed as a post-processing task on the initial ONNC IR, so it is added after `addStandardTensorSel()`. 


### Updating the building scripts

In the previous sections, we have added several new files. You need to update the building scripts to compile ONNC. In ONNC, two common building systems, autotools and cmake, are supported concurrently. Therefore, You need to update corresponding scripts for both building systems. Since the example here is to enable the compound operator for the X86 backend, all the new C++ files are located within the directory `lib/Target/X86`. You need to update the `CMakeLists.txt` and `Makefile.am` file in that directory.

```cmake
add_libonnc_src(
    Compute/X86ConvRelu.cpp
    X86FuseConvRelu.cpp
    ...)
```

**Code Snippet 8.** (`CMakeLists.txt`) Including new files in the *cmake* building system.

```make
ONNC_TARGET_SOURCES += \
  Target/X86/Compute/X86ConvRelu.cpp \
  Target/X86/X86FuseConvRelu.cpp \
  ...
```
**Code Snippet 9.** (`Makefile.am`) Including new files in the *autotools* building system.

## 5. Integrating target-specific operators with the compilation flow and optimization passes

When a new operator is added for a target backend, the compilation pipeline and the optimization passes are not aware that the new operator exists. Developers need to go through the compilation flow and each optimization pass to handle the new operator. For example, we have to modify the code emitting pass to handle the machine code generation of the new operator. Remember to check every pass to see if such a modification is needed.

