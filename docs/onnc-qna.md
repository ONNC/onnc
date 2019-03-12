---
title: ONNC Q&A
date: Mar 12, 2019
---

# Question

## Problem description

What happens about the following error?
```
$ ./tools/onnc/onnc -mquadruple vanilla /models/mnist/model.onnx
Vanilla is invoked
Fatal: cannot lower ::onnx::Node Times212. Lower not found.
```

## Root cause

This error occurs because your backend is lack of registering some operators. In this case, an operator named `Times212` is not registered. To register an operator means that you add codes in the following function.
```c++
void VanillaBackend::RegisterLowers(LowerRegistry& pRegistry) const
{
  //...
  pRegistry.emplace<BatchNormalizationLower>();
  pRegistry.emplace<ConcatLower>();
  pRegistry.emplace<ConvLower>();
  //...
}
```

In the above codes, this line `pRegistry.emplace<ConvLower>();`, for example, is for registering operator convolution so that your backend can support this type of operator. 

## Solution

Back to the question, now we know an operator named `Times212` is not registered yet. The next question is what type of this operator? Note that `Times212` is just the name of the operator, just like we can have several convolutions in a model and each has its own name. However, we just register the operator type of convolution instead of registering each individual convolution name. 

You need a model viewer to find out the operator type. One visualization tool is "netron" (https://github.com/lutzroeder/netron). 
Below shows a screenshot of using netron to visualize model mnist. You can see `Times212` is of type `MatMul`. 

![Screenshot of netron to find out operator named `Times212`.](images/operator-type-and-name.png?raw=true)

Then you know you need to register that operator in your backend. 

```c++
#include <onnc/Transforms/TensorSel/Standards/MatMulLower.h>

//...

void VanillaBackend::RegisterLowers(LowerRegistry& pRegistry) const
{
  //...
  pRegistry.emplace<MatMulLower>();
  //...
}
```

After that, your backend can support operator `MatMul`. If a given model contains that type of operators, they will appear as nodes in the model graph, and you can do optimization on them, including the code emitting pass. Therefore, don't forget to add visit method for that newly-supported operator. In precise, you need to modify the following codes.
```c++
class CodeEmitVisitor : public ComputeVisitor
{
  //...
  void visit(const MatMul& pMatMul) override;
  void visit(MatMul& pMatMul) override;
};
```

If you need to know the definition of `MatMul`, please refer to https://github.com/onnx/onnx/blob/master/docs/Operators.md#matmul for details.

