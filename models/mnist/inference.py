import numpy as np
import test_data_set_0.input_0 as test_data

def Conv(X, W, attr):
    return 0

def Add(A, B):
    #for i in range(A.shape[0]):
    #    for j in range(A.shape[1]):
    #        for k in range(A.shape[2]):
    #            for l in range(A.shape[3]):
    #                A[i][j][k][l]+=B[j][0][0]
    return A

def Relu(X):
    return 0

def MaxPool(X, attr):
    return 0

def Reshape(data, shape):
    a = np.arange(1*16*4*4).reshape((1, 16, 4, 4)) ##自己假設4d numpy
    d=np.reshape(a,(1,256,-1))##reshape 1=>shape[0] 256=>shape[1]
    print(a)
    print(d)
    
    return 0

def MatMul(A, B):
    two_dim_matrix_one = np.array([[1, 2, 3], [4, 5, 6]])  ##自己假設兩個 2d numpy
    wo_dim_matrix_two = np.array([[1, 2], [3, 4], [5, 6]])
    two_multi_result = np.dot(two_dim_matrix_one, wo_dim_matrix_two)  ##result
    return two_multi_result

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
Plus112_Output_0 = 0#Add(Convolution110_Output_0, Parameter88)我先註解掉了

ReLU114_Output_0 = Relu(Plus112_Output_0)

Pooling160_Output_0 = MaxPool(ReLU114_Output_0,
                              {'kernel_shape': [3, 3],
                               'strides': [3, 3],
                               'pads': [0, 0, 0, 0]})

Pooling160_Output_0_reshape0 = Reshape(Pooling160_Output_0, [1, 256])

Parameter193 = np.load('Parameter193.npy')
Parameter193_reshape1 = Reshape(Parameter193, [256, 10])

Times212_Output_0 = MatMul(Pooling160_Output_0_reshape0, Parameter193_reshape1)
print(Times212_Output_0)
Parameter194 = np.load('Parameter194.npy')
Plus214_Output_0 = 0#Add(Times212_Output_0, Parameter194)我先註解掉了

# Check result.
print(Plus214_Output_0)
