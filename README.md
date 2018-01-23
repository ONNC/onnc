# Introduction
ONNC (Open Neural Network Compiler)

# Test

## Add or Remove a Unit-Test

From CI pipeline:
0. ci/.gitlab-ci.yml
1. docker/common.sh

From docker development environment:
0. docker/in-container/testall

# Directory Structure
* README    - This document
* docs      - documents
* externals - The 3rd-party projects
* include   - header files for libonnc
* lib       - implementation for libonnc
* test      - Quick regression tests for ONNC.
* tools     - tools based on libonnc
