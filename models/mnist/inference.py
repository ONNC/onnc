import numpy as np
import sys

def Conv(X, W, attr):
    if type(X) != np.ndarray:
        return
    assert len(X.shape) == 4
    assert X.shape[0] == 1 # batch must be 1
    in_channel = X.shape[1]
    in_height = X.shape[2]
    in_width = X.shape[3]

    num_kernels = W.shape[0]
    assert W.shape[1] == in_channel
    kernel_width, kernel_height = attr['kernel_shape']
    stride_width, stride_height = attr['strides']
    
    out_channel = num_kernels
    assert attr['auto_pad'] == 'SAME_UPPER' # In this case, out_height/out_width must be the same as in_height/in_width
    out_height = in_height
    out_width = in_width

    # Calculate padding.
    padding_width = (kernel_width + (out_width - 1) * stride_width) - in_width
    padding_left = int(padding_width / 2)
    padding_right = padding_width - padding_left
    # FIXME: assume height and width are symmetric.
    padding_top = padding_left
    padding_bottom = padding_right
    
    Y = np.zeros((1, out_channel, out_height, out_width)).astype(np.float32)
    for oc in range(out_channel):
        for oh in range(out_height):
            for ow in range(out_width):
                for ic in range(in_channel):
                    ih = oh - padding_top
                    iw = ow - padding_left
                    for kh in range(kernel_height):
                        for kw in range(kernel_width):
                            if iw + kw < 0 or iw + kw >= in_width or ih + kh < 0 or ih + kh >= in_height:
                                x_val = 0
                            else:
                                x_val = X[0, ic, ih + kh, iw + kw]
                            Y[0, oc, oh, ow] += x_val * W[oc, ic, kh, kw]
    return Y

def Add(A, B):
    if A.ndim == 4:
        out_channel=A.shape[1]
        out_height=A.shape[2]
        out_width=A.shape[3]
        Y = np.zeros((1, out_channel, out_height, out_width)).astype(np.float32)
        for i in range(A.shape[0]):
            for j in range(A.shape[1]):
                for k in range(A.shape[2]):
                    for l in range(A.shape[3]):
                        Y[i][j][k][l]+=B[j][0][0]

    else:
        out_channel=A.shape[1]
        Y = np.zeros((1, out_channel)).astype(np.float32)
        for i in range(1):
            for j in range(A.shape[1]):
                Y[i][j]+=B[i][j]
        #print(B.shape[0])
        #print(B.shape[1])
        #for i in range(A.shape[0]):
            #for j in range(A.shape[1]):
                #print("d")

    return Y

def Relu(X):
    return np.maximum(0,X)

def MaxPool(X, attr):
    in_channel = X.shape[1]
    in_height = X.shape[2]
    in_width = X.shape[3]

    pad_left, pad_right, pad_top, pad_bottom = attr['pads']
    kernel_width, kernel_height = attr['kernel_shape']
    stride_width, stride_height = attr['strides']

    out_channel = in_channel
    out_height = int((in_height + pad_top + pad_bottom - kernel_height) / stride_height) + 1
    out_width = int((in_width + pad_left + pad_right - kernel_width) / stride_width) + 1

    Y = np.zeros((1, out_channel, out_height, out_width)).astype(np.float32)
    for oc in range(out_channel):
        for oh in range(out_height):
            for ow in range(out_width):
                ih = oh * stride_height - pad_top
                iw = ow * stride_width - pad_left
                max_val = np.float32('-inf')
                for kh in range(kernel_height):
                    for kw in range(kernel_width):
                        if ih + kh < 0 or ih + kh >= in_height or iw + kw < 0 or iw + kw >= in_width:
                            x_val = 0
                        else:
                            x_val = X[0, oc, ih + kh, iw + kw]
                        if x_val > max_val:
                            max_val = x_val
                Y[0, oc, oh, ow] = max_val

    return Y

def Reshape(data, shape):
    #a = np.arange(1*16*4*4).reshape((1, 16, 4, 4)) ##自己假設4d numpy
    Reshaped_data=data.reshape(shape[0],shape[1])##reshape 1=>shape[0] 256=>shape[1]
    
    
    return Reshaped_data

def MatMul(A, B):
    #two_dim_matrix_one = np.array([[1, 2, 3], [4, 5, 6]])  ##自己假設兩個 2d numpy
    #wo_dim_matrix_two = np.array([[1, 2], [3, 4], [5, 6]])
    two_multi_result = np.dot(A, B)  ##result
    return two_multi_result

if len(sys.argv) != 2:
    print('Help: python inference.py <test_data_set: 0, 1, or 2>')
    exit(0)
    
exec('import test_data_set_' + sys.argv[1] + '.input_0 as test_data')

# Print pretty floating-point format for array.
np.set_printoptions(precision=3)
np.set_printoptions(suppress=True)

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
                              'strides': [2, 2],
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
#print(Times212_Output_0)
Parameter194 = np.load('Parameter194.npy')
Plus214_Output_0 = Add(Times212_Output_0, Parameter194)

# Check result.
print(Plus214_Output_0)
