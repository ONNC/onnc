# ONNC v1.2.0 Supported Operators

## Supported ONNX Operators

- *Add*
- *AveragePool*
- *BatchNormalization*
- *Concat*
- *Conv*
- *Gemm*
- *GlobalAveragePool*
- *LRN*
- *MaxPool*
- *Mul*
- *Relu*
- *Reshape*
- *Softmax*
- *Sum*
- *Transpose* *(used in ShuffleNet)*
- *Unsqueeze*

## Hardware Execution Unit Mapping

|Operator|Execution Unit|
|:-|:-:|
|*Add*|*SDP*|
|*AveragePool*|*PDP*|
|*BatchNormalization*|*SDP*|
|*Concat*|*RUBIK*|
|*Conv*|*CONV*, *SDP*|
|*Gemm*|*CONV*, *SDP*|
|*GlobalAveragePool*|*PDP*, *SDP*|
|*LRN*|*CDP*|
|*MaxPool*|*PDP*|
|*Mul*|*SDP*|
|*Relu*|*SDP*|
|*Reshape*|*Compiler Optimization*|
|*Softmax*|***EMU***|
|*Sum*|*SDP*|
|*Transpose*|*RUBIK*|
|*Unsqueeze*|*Computer Optimization*|

## Limitations

### Add

- Support [*conditional broadcasting*](#Conditional-Broadcasting)
- Accept **at most** *1* input constant tensor

### AveragePool

- Attribute *auto_pad* should be "NOTSET"
- Attribute *kernel_shape* should contain values in the range of *[1, 8]*
- Attribute *pads* should contain values in the range of *[0, 7]*
- Attribute *strides* should contain values in the range of *[1, 8]*

### BatchNormalization

- Input *scale* should be a constant tensor
- Input *B* should be a constant tensor
- Input *mean* should be a constant tensor
- Input *var* should be a constant tensor

### Concat

- Attribute *axis* should be *1*
- Input cannot be a constant tensor
- Input tensor's height<sub>input</sub>, width<sub>input</sub> , and channel<sub>input</sub> should all be in the range of *[1, 8192]*
- Output tensor's height<sub>output</sub>,  width<sub>output</sub> and channel<sub>output</sub> should all be in the range of *[1, 8192]*

### Conv

- Input *W* should be a constant tensor
- Input *B* should be a constant tensor
- Attribute *auto_pad* should be "NOTSET"
- Attribute *group* should in the range of *[1, 8192]*
- Attribute *dilations* should contain values in the range of *[1, 32]*
- Attribute *kernel_shape* should contain values in the range of *[1, 32]*
- Attribute *pads* should contain values in the range of *[0, 31]*
- Attribute *strides* should contain values in the range of *[1, 8]*

### Gemm

- Input *B* should be a constant tensor
- Input *C* should be a constant tensor

### GlobalAveragePool

*None*

### LRN

- Attribute *alpha* should be *0.0001*
- Attribute *beta* should be *0.75*
- Attribute *bias* should be *1.0*
- Attribute *size* should be one of [*3*, *5*, *7*, *9*]

### MaxPool

- Attribute *storage_order* should be *0*
- Attribute *auto_pad* should be "NOTSET"
- Attribute *kernel_shape* should contain values in the range of *[1, 8]*
- Attribute *pads* should contain values in the range of *[0, 7]*
- Attribute *strides* should contain values in the range of *[1, 8]*

### Mul

- Support [*conditional broadcasting*](#Conditional-Broadcasting)
- Accept **at most** *1* input constant tensor

### Relu

*(none)*

### Reshape

- Input *shape* should be a constant tensor
- Input *data*'s height<sub>data</sub> * width<sub>data</sub> should be equal to its output tensor's height<sub>reshaped</sub> * width<sub>reshaped</sub>

### Softmax

- Attribute *axis* should be *1*

### Sum

- Accept **at most** *2* input tensors
- Input tensors should **not** be constant tensors
- Input tensors' shape should be identical

### Transpose

- Only support the *Reshape*-*Transpose*-*Reshape* pattern used *in ShuffleNet*
- Dimension of input tensor should be *5*
- Attribute *perm* should be *[0, 2, 1, 3, 4]*

### Unsqueeze 

- Input should be a constant tensor
- Dimension of output tensor should be less than or equal to *4*

## Conditional Broadcasting

Unlike ONNX's broadcasting [definition](https://github.com/onnx/onnx/blob/rel-1.3.0/docs/Broadcasting.md), *ONNC*  can broadcast tensors in some cases due to limited NVDLA hardware support.

Limitation of *NVDLA* hardware
- For broadcasting sources in SDP operations, only support constants. 
- No support for bi-directional broadcasting

Broadcasting support in *ONNC*
- Single direction per-channel broadcasting
- Single direction per-layer broa
- |dcasting (Source should be a constant tensor)

