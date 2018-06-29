# -*- Python -*-

# Reference: llvm/test/lit.cfg.py

# Configuration file for the 'lit' test runner.
import os
import platform
import subprocess

import lit.util
import lit.formats

# name: The name of this test suite.
config.name = 'ONNC'

# testFormat: The test format to use to interpret tests.
config.test_format = lit.formats.ShTest(execute_external = True)

# suffixes: A list of file extensions to treat as test files.
# onnx.s
config.suffixes = ['.s']

# excludes: A list of directories to exclude from the testsuite.
# config.excludes = []

# test_source_root: The root path where tests are located.
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = config.onnc_test_root

config.substitutions.append(('onnx-as', config.toolBinDir + "/onnx-as/onnx-as"))
config.substitutions.append(('onnx-dis',
                             config.toolBinDir + "/onnx-dis/onnx-dis"))
config.substitutions.append(('onnx2tg', config.toolBinDir + "/onnx2tg/onnx2tg"))

print("This is ONNC test! (=ﾟωﾟ)ﾉ\n")
