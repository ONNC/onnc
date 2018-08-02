# Introduction

                     ONNC (Open Neural Network Compiler)

A compiler specifically designed for Deep Learning Accelerator (DLA).
Based on Open Neural Network Exchange (ONNX), ONNC transforms ONNX models into
binary machine code for DLA ASIC. ONNC leverages the IR design of ONNX and
provides effective algorithms to eliminate the overhead of data movement.
ONNC increases DLA ASIC performance and shorten production time for DLA ASIC.

## Lastest release

* v0.9
  - Tarball:
 
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

## Download and build ONNC.
For now, the simplest way to download and build ONNC is follow the
instructions of README.md in **onnc-umbrella**.
