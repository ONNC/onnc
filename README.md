# ONNC (Open Neural Network Compiler)

## Introduction

ONNC (Open Neural Network Compiler) is a retargetable compilation framework designed specifically for proprietary deep learning accelerators. Its software architecture expedites porting ONNC to any Deep Learning Accelerator (DLA) design that supports [ONNX (Open Neural Network Exchange)](https://onnx.ai/) operators. ONNC guarantees executability across every DLA by means of transforming ONNX models into DLA-specific binary forms and leveraging the intermediate representation (IR) design of ONNX along with effective algorithms to eliminate the overhead of data movement. **ONNC is the first open source compiler available for NVDLA-based hardware designs**. Its NVDLA backend can compile a model into an executable NVDLA Loadable file. Integrating ONNC with the NVDLA software stack opens up opportunities for developers and researchers to explore the NVDLA-based inference design at system level. 

## Papers

* W. F. Lin, D. Y. Tsai, L. Tang, C. T. Hsieh, C. Y. Chou, P. H. Chang, and L. Hsu, “ONNC: A compilation framework connecting ONNX to proprietary deep learning accelerators,” in IEEE International Conference on Artificial Intelligence Circuits and Systems (AICAS 2019). IEEE, 2019.

* W.F. Lin, C. T. Hsieh, C. Y. Chou, "ONNC-based Software Development Platform for Configurable NVDLA Designs", to appear in IEEE International Symposium on VLSI Design, Automation and Test (VLSI-DAT 2019). IEEE, 2019

## Documentation

- [ONNC Utilities](docs/ONNC-Utilities.md)
- [ONNC Pass Manager Getting Started Guide](docs/ONNC-Pass-Manager-Getting-Started-Guide.md)
- [ONNC Backend Developer Guide](docs/ONNC-Backend-Porting-Guide.md)
- [The Code Emitting Pass User Guide](docs/The-Code-Emitting-Pass-User-Guide.md)
- [ONNC IR Extension Guide](docs/ONNC-IR-Extension-Guide.md)

## Current Status

* [v1.2.0](https://github.com/ONNC/onnc/releases)

## How to contribute

* Please see [contribute guidline](https://github.com/ONNC/onnc/blob/master/CONTRIBUTING.md)
 
## Directory Structure
* README.md - This document
* docs      - documents
* include   - header files for libonnc
* lib       - implementation for libonnc
* tools     - tools based on libonnc

## Supported platforms

ONNC supports Ubuntu/x86_64 and MacOSX.

Here is a list of verified versions:
* Ubuntu/x86_64
  - 16.04

* MacOSX
  - High Sierra

## Getting Started

There are three ways to build ONNC:

1. Build ONNC via Docker  
    Please refer to the [ONNC Utilities](docs/ONNC-Utilities.md) document.
2. Build ONNC via ONNC umbrella  
    Please follow the [instructions of README.md](https://github.com/ONNC/onnc-umbrella) in **onnc-umbrella**.  
    [Here](https://github.com/ONNC/onnc-umbrella/tree/master/external) is the version of external library we are using in ONNC. 
3. Build ONNC without ONNC umbrella  
    Please refer to the [ONNC Automake build instruction](docs/automake-build.md) or [ONNC CMake build instruction](docs/build.md)
