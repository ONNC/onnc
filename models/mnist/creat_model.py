import numpy as np
import onnx
from onnx import numpy_helper
from onnx import helper
from onnx import AttributeProto, TensorProto, GraphProto


# Preprocessing: create a Numpy array
# Convert the Numpy array to a TensorProto
# These are weights or constants for operators
Parameter5_array = np.load('./quantized_weight/quant_Parameter5.npy')
Parameter5_tensor = numpy_helper.from_array(Parameter5_array)

Parameter6_array = np.load('./quantized_weight/quant_Parameter6.npy')
Parameter6_tensor = numpy_helper.from_array(Parameter6_array)

Parameter87_array = np.load('./quantized_weight/quant_Parameter87.npy')
Parameter87_tensor = numpy_helper.from_array(Parameter87_array)

Parameter88_array = np.load('./quantized_weight/quant_Parameter88.npy')
Parameter88_tensor = numpy_helper.from_array(Parameter88_array)

Parameter193_array = np.load('./quantized_weight/quant_Parameter193.npy')
Parameter193_tensor = numpy_helper.from_array(Parameter193_array)

Parameter194_array = np.load('./quantized_weight/quant_Parameter194.npy')
Parameter194_tensor = numpy_helper.from_array(Parameter194_array)

Pooling160_Output_0_reshape0_shape_array = np.load('./quantized_weight/Pooling160_Output_0_reshape0_shape.npy')
Pooling160_Output_0_reshape0_shape_tensor = numpy_helper.from_array(Pooling160_Output_0_reshape0_shape_array)

Parameter193_reshape1_shape_array = np.load('./quantized_weight/Parameter193_reshape1_shape.npy')
Parameter193_reshape1_shape_tensor = numpy_helper.from_array(Parameter193_reshape1_shape_array)

# create tensors
Input3 =        helper.make_tensor_value_info('Input3', TensorProto.FLOAT, [1,1,28,28])
Parameter5 =        helper.make_tensor_value_info('Parameter5', TensorProto.FLOAT, [8,1,5,5])
Parameter6 =        helper.make_tensor_value_info('Parameter6', TensorProto.FLOAT, [8,1,1])
Parameter87 =        helper.make_tensor_value_info('Parameter87', TensorProto.FLOAT, [16,8,5,5])
Parameter88 =        helper.make_tensor_value_info('Parameter88', TensorProto.FLOAT, [16,1,1])
Pooling160_Output_0_reshape0_shape =        helper.make_tensor_value_info('Pooling160_Output_0_reshape0_shape', TensorProto.FLOAT, [1,256])
Parameter193_reshape1_shape =        helper.make_tensor_value_info('Parameter193_reshape1_shape', TensorProto.FLOAT, [256,10])
Parameter193 =        helper.make_tensor_value_info('Parameter193', TensorProto.FLOAT, [16,4,4,10])
Parameter194 =        helper.make_tensor_value_info('Parameter194', TensorProto.FLOAT, [1,10])

Convolution28_Output_0 =        helper.make_tensor_value_info('Convolution28_Output_0', TensorProto.FLOAT, [1,8,28,28])
Plus30_Output_0 =               helper.make_tensor_value_info('Plus30_Output_0', TensorProto.FLOAT, [1,8,28,28])
ReLU32_Output_0 =               helper.make_tensor_value_info('ReLU32_Output_0', TensorProto.FLOAT, [1,8,28,28])
Pooling66_Output_0 =            helper.make_tensor_value_info('Pooling66_Output_0', TensorProto.FLOAT, [1,8,14,14])
Convolution110_Output_0 =       helper.make_tensor_value_info('Convolution110_Output_0', TensorProto.FLOAT, [1,16,14,14])
Plus112_Output_0 =              helper.make_tensor_value_info('Plus112_Output_0', TensorProto.FLOAT, [1,16,14,14])
ReLU114_Output_0 =              helper.make_tensor_value_info('ReLU114_Output_0', TensorProto.FLOAT, [1,16,14,14])
Pooling160_Output_0 =           helper.make_tensor_value_info('Pooling160_Output_0', TensorProto.FLOAT, [1,16,4,4])
Pooling160_Output_0_reshape0 =  helper.make_tensor_value_info('Pooling160_Output_0_reshape0', TensorProto.FLOAT, [1,256])
Parameter193_reshape1 =         helper.make_tensor_value_info('Parameter193_reshape1', TensorProto.FLOAT, [256,10])
Times212_Output_0 =             helper.make_tensor_value_info('Times212_Output_0', TensorProto.FLOAT, [1,10])
Plus214_Output_0 =              helper.make_tensor_value_info('Plus214_Output_0', TensorProto.FLOAT, [1,10])

# Create a node (NodeProto)

# Create the graph (GraphProto)
graph_def = helper.make_graph(
    [
        helper.make_node("Conv", ['Input3','Parameter5'], ['Convolution28_Output_0'],'Convolution28',kernel_shape = [5,5],auto_pad  ="SAME_UPPER",strides=[1,1],group=1,dilations = [1,1]),
        helper.make_node("Add", ['Convolution28_Output_0','Parameter6'], ['Plus30_Output_0'],'Plus30'),
        helper.make_node("Relu", ['Plus30_Output_0'], ['ReLU32_Output_0'],'ReLU32'),
        helper.make_node("MaxPool", ['ReLU32_Output_0'], ["Pooling66_Output_0"],'Pooling66',kernel_shape = [2,2], pads =[0,0,0,0],auto_pad  ="NOTSET",strides=[2,2]),
        helper.make_node("Conv", ['Pooling66_Output_0','Parameter87'], ['Convolution110_Output_0'],'Convolution110',kernel_shape = [5,5],auto_pad  ="SAME_UPPER",strides=[1,1],group=1,dilations = [1,1]),
        helper.make_node("Add", ['Convolution110_Output_0','Parameter88'], ['Plus112_Output_0'],'Plus112'),
        helper.make_node("Relu", ['Plus112_Output_0'], ['ReLU114_Output_0'],'ReLU114'),
        helper.make_node("MaxPool", ['ReLU114_Output_0'], ["Pooling160_Output_0"],'Pooling160',kernel_shape = [3,3], pads =[0,0,0,0],auto_pad  ="NOTSET",strides=[3,3]),
        helper.make_node("Reshape",['Pooling160_Output_0','Pooling160_Output_0_reshape0_shape'],['Pooling160_Output_0_reshape0'],'Times212_reshape0'),
        helper.make_node("Reshape",['Parameter193','Parameter193_reshape1_shape'],['Parameter193_reshape1'],'Times212_reshape1'),
        helper.make_node("MatMul",['Pooling160_Output_0_reshape0','Parameter193_reshape1'],['Times212_Output_0'],'Times212'),
        helper.make_node("Add", ['Times212_Output_0','Parameter194'], ['Plus214_Output_0'],'Plus214'),
    ],
    "CNN",
    [
        Input3,
        Parameter5,
        Parameter6,
        Parameter87,
        Parameter88,
        Pooling160_Output_0_reshape0_shape,
        Parameter193_reshape1_shape,
        Parameter193,
        Parameter194
    ],
    [
        Plus214_Output_0
    ]
)

# set up Initializer
graph_def.initializer.extend([Parameter5_tensor])
graph_def.initializer[0].name = 'Parameter5'

graph_def.initializer.extend([Parameter6_tensor])
graph_def.initializer[1].name = 'Parameter6'

graph_def.initializer.extend([Parameter87_tensor])
graph_def.initializer[2].name = 'Parameter87'

graph_def.initializer.extend([Parameter88_tensor])
graph_def.initializer[3].name = 'Parameter88'

graph_def.initializer.extend([Pooling160_Output_0_reshape0_shape_tensor])
graph_def.initializer[4].name = 'Pooling160_Output_0_reshape0_shape'

graph_def.initializer.extend([Parameter193_reshape1_shape_tensor])
graph_def.initializer[5].name = 'Parameter193_reshape1_shape'

graph_def.initializer.extend([Parameter193_tensor])
graph_def.initializer[6].name = 'Parameter193'

graph_def.initializer.extend([Parameter194_tensor])
graph_def.initializer[7].name = 'Parameter194'


# Create the model (ModelProto)
model_def = helper.make_model(graph_def, producer_name='CNTK 2.5.1')

#print('The model is:\n{}'.format(model_def))
onnx.checker.check_model(model_def)
#print(model_def)
#converted_model = version_converter.convert_version(model_def, <int target_version>)
print('The model is checked!')
#print(graph_def.initializer)
onnx.save(model_def, './quantized-new.onnx')
