#!/usr/bin/env python2

import argparse
from PIL import Image
from onnx import onnx
import sys
import struct

import numpy as np
from onnx import numpy_helper

def main():
    parser = argparse.ArgumentParser(description='convert JPEG image to ONNX tensor.')
    parser.add_argument('input', type=str, metavar='INPUT.jpg', 
                        help='Input JPEG file')
    parser.add_argument('--output', '-o', type=str, metavar='OUTPUT.pb', default='output.pb',
                        help='Output ONNX tensor protocol buffer file (default: output.pb)')
    args = parser.parse_args()

    # read input image
    input_image = Image.open(args.input)
    image_width, image_height = input_image.size
    input_pixels = input_image.load()

    red_idx, green_idx, blue_idx = range(3)

    intermediate_array = np.zeros((1, 3, image_height, image_width), dtype=np.float32) 

    for row in range(image_height):
        for col in range(image_width):
            red, green, blue = input_pixels[row, col]
            intermediate_array[0, red_idx  , row, col] = red
            intermediate_array[0, green_idx, row, col] = green
            intermediate_array[0, blue_idx , row, col] = blue

    output_tensor = numpy_helper.from_array(intermediate_array)
    with open(args.output, 'wb') as f:
        f.write(output_tensor.SerializeToString())

    return 0

if __name__ == '__main__':
    sys.exit(main())
