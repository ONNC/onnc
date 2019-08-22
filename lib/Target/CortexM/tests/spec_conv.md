# Conv

## Support Spec

* Attribute `auto_pad`
    * Support all the four values defined in ONNX: `NOTSET`, `SAME_UPPER`, `SAME_LOWER`, and `VALID`

* Attribute `dilations`
    * dilations == 1
    
* Attribute `group`
    * group == 1


## Testing Spec

* conv_basic
    * A very simple convolution for quick testing.
    * input: 1x1x10x10
    * kernel_shape: 3x3
    * output: 1x2x8x8


