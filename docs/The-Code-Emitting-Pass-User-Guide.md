# The Code Emitting Pass User Guide

Release PDF: [The-Code-Emitting-Pass-User-Guide.pdf](https://github.com/ONNC/onnc/wiki/files/1.0.0/The-Code-Emitting-Pass-User-Guide.pdf)

This application note aims at helping the backend developers to implement the code emitting pass by explaining how it works and how it leverages the visitor design pattern to simplify the implementation process. This document is compliant to the ONNC community Docker image v1.0. You may download the Docker image from the Docker Hub (https://hub.docker.com/r/onnc/onnc-community/).

## 1. Introduction

The purpose of a code emitting pass is to generate machine code for a target hardware to execute or to generate an intermediary file like a Loadable file for the NVDLA-based design. The implementation of the code emitting pass strongly depends on the target hardware specification and it is a non-trivial job. To help developers implement the coding emitting pass, ONNC has many built-in features in the framework to minimize the coding effort. Leveraging the visitor design pattern is the most important feature and the details will be covered in the next section.


## 2. Visitor Design Pattern in the Code Emitting Pass

Visitor design pattern is one of the behavioral design patterns. It is used when we have to perform an operation on a group of similar kind of Objects. With the help of visitor pattern, we can move the operational logic from the objects to a central class. ONNC leverages the visitor design pattern in many occasions and the code emitting pass is one example. Users are encouraged to use the visitor design pattern to minimize their coding effort whenever they can. 

In the ONNC framework, a pass named `CodeEmit` serves as an entry point for generating compiler outputs. In the ONNC software architecture, the `CodeEmit` pass is the last stage in the pipeline and it takes advantage of the [visitor design pattern](https://en.wikipedia.org/wiki/Visitor_pattern) in its implementation. The overall concept is illustrated in Figure 1. 

Figure 1 shows the data structure as well as the pseudo code related to the code emitting pass. There are two segments in Figure 1. The lower segment is part of the ONNC framework and it can be reused for any backend. The upper segment contains the backend-specific implementation. 

![](https://github.com/ONNC/onnc/wiki/files/1.0.0/code-emit-visitor-diagram.png)

**Figure 1: Pseudo code of pass `CodeEmit` designed in the visitor design pattern.**

`ComputeVisitor` is an abstract class used to declare the visit operations for all types of visitable classes. The `CodeEmitVisitor` contains all the visit methods that must be implemented to handle all types of operators. Each operator class derived from the `ComputeOperator` class is a visitable class providing the `accept()` method to accept a visitor. 

A backend developer needs to implement the upper segment in two steps: 

**Step 1. Add the`CodeEmit` pass into your backend.** 

In the `CodeEmit` pass, ONNC loops over every operators in a model and calls the `accept()` method of each operator passing an instance of `CodeEmitVisitor` as an input argument. Subsequently, the `accept()` method of each operator invokes the corresponding `visit()` method for each operator to perform the code generation. 

**Step 2. Implement the `CodeEmitVisitor` class to handle code generation for each supported operator.**

In practice, each operator has a corresponding `visit` method in the `CodeEmitVisitor` class. For example, the convolution operator has the`visit(Conv& pConv)` method and the maximum pooling operator has the ` visit(MaxPool& pMaxPool)` method. All operators have the same method name but different argument types. In this way, the visitor design pattern signifcantly enhances the readability and scalability of the code emitting pass. 

The rest of this application note will describe the detailed implementation shown in the Vanilla backend. 

## 3. Adding the Code Emitting Pass

The `CodeEmit` pass is added to the pass manager as shown in the Code Snippet 1.

```cpp
// lib/Target/Vanilla/VanillaBackend.cpp
void VanillaBackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  // Remember to add code generation codes in class CodeEmitVisitor.
  static vanilla::CodeEmitVisitor ceVisitor;
  // Remember to pass CodeEmitVisitor as input argument.
  pPM.add(CreateCodeEmitPass(ceVisitor));
}
```

**Code Snippet 1: Adding pass `CodeEmit` in a backend.**

## 4. Implementing Code Generation for Each Operator

Code Snippet 2 depicts the declaration of the `CodeEmitVisitor` class in the Vanilla backend. In this class, each operator has its corresponding `visit()` method to handle its code generation.

```cpp
// lib/Target/Vanilla/CodeEmitVisitor.h
class CodeEmitVisitor : public ComputeVisitor
{
  ...
  void visit(Conv& pConv); // code generation for convolution
  void visit(Relu& pRelu); // code generation for ReLU
};
```

**Code Snippet 2: Implementing the `visit` method for every operator in the `CodeEmitVisitor` class.**

## 5. Summary

Up to release V1.0, ONNC has two real backends available in the GitHub repository. The X86 backend has a built-in interpreter, ONNI, to handle runtime execution without generating machine codes. Therefore, it does not have a code emitting pass. A real example for the code emitting pass can be found in the NVDLA backend. Currently, the NVDLA backend supports 10 operators and each has a corresponding `visit()` method in the `CodeEmitVisitor` class. Whoever is interested in porting a new backend with a `CodeEmit` pass may refer to the NVDLA backend for more details. 

```cpp
  void visit(const Conv& pConv) override;
  void visit(const Reshape& pReshape) override;
  void visit(const Relu& pRelu) override;
  void visit(const LRN& pLRN) override;
  void visit(const MaxPool& pMaxPool) override;
  void visit(const AveragePool& pAveragePool) override;
  void visit(const Gemm& pGemm) override;
  void visit(const Softmax& pSoftmax) override;
  void visit(const Concat& pConcat) override;
  void visit(const Sum& pSum) override;
```
