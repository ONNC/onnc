import numpy as np
import test_data_set_0.input_0 as test_data

def Conv(X, W, attr):
    return 0

def Add(A, B):
    return 0

def Relu(X):
    return 0

def MaxPool(X, attr):
    return 0

def Reshape(data, shape):
    return 0

def MatMul(A, B):
    return 0

# Do model inference.

Input3 = test_data.tensor()
Parameter5 = np.load('Parameter5.npy')
Convolution28_Output_0 = Conv(Input3, Parameter5,
                              {'kernel_shape': [5, 5],
                               'strides': [1, 1],
                               'auto_pad': 'SAME_UPPER'})

Parameter6 = np.load('Parameter6.npy')
Plus30_Output_0 = Add(Convolution28_Output_0, Parameter6)

ReLU32_Output_0 = Relu(Plus30_Output_0)

Pooling66_Output_0 = MaxPool(ReLU32_Output_0,
                             {'kernel_shape': [2, 2],
                              'stride': [2, 2],
                              'pads': [0, 0, 0, 0]})

Parameter87 = np.load('Parameter87.npy')
Convolution110_Output_0 = Conv(Pooling66_Output_0, Parameter87,
                               {'kernel_shape': [5, 5],
                                'strides': [1, 1],
                                'auto_pad': 'SAME_UPPER'})

Parameter88 = np.load('Parameter88.npy')
Plus112_Output_0 = Add(Convolution110_Output_0, Parameter88)

ReLU114_Output_0 = Relu(Plus112_Output_0)

Pooling160_Output_0 = MaxPool(ReLU114_Output_0,
                              {'kernel_shape': [3, 3],
                               'strides': [3, 3],
                               'pads': [0, 0, 0, 0]})

Pooling160_Output_0_reshape0 = Reshape(Pooling160_Output_0, [1, 256])

Parameter193 = np.load('Parameter193.npy')
Parameter193_reshape1 = Reshape(Parameter193, [256, 10])

Times212_Output_0 = MatMul(Pooling160_Output_0_reshape0, Parameter193_reshape1)

Parameter194 = np.load('Parameter194.npy')
Plus214_Output_0 = Add(Times212_Output_0, Parameter194)

# Check result.
print(Plus214_Output_0)
