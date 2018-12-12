# Introduction

**ONNC (Open Neural Network Compiler)**

The ONNC project aims to provide a compiler to
connect [Open Neural Network Exchange Format (ONNX)](https://onnx.ai/)
to every Deep Learning Accelerator (DLA). ONNX represents deep learning
models that enables models to be correctly transferred among frameworks
such as TensorFlow. ONNC guarantees executability across every DLA by
means of transforming ONNX models into DLA specific binary forms and
leveraging the intermediate representation (IR) design of ONNX along with
effective algorithms to eliminate the overhead of data movement.

With the aid of ONNC, DLA vendors can specify customizecd physical cost
model and avoid re-inventing these intricate optimization algorithms.

## Current Status

* [v0.9](https://github.com/ONNC/onnc/releases)

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

## Download and build ONNC.
The simplest way to download and build ONNC is follow the
[instructions of README.md](https://github.com/ONNC/onnc-umbrella) in **onnc-umbrella**.
[Here](https://github.com/ONNC/onnc-umbrella/tree/master/external) is the version of external library we are using in ONNC. 

## Basic Installation
Here is general installation instructions.

```
cd ${ONNC}   # go to the source
./autogen.sh # generate GNU Autotools input files
cd ..
mkdir build  # create placeholder for build
cd build
../${ONNC}/configure --prefix=${INSTALL} \ # configure
                     --with-llvm=${LLVM_DIR} \
                     --with-onnx=${ONNX_DIR} \
                     --with-skypat=${SKYPAT} \
                     --with-target=x86
make
make install
```

The first step is to generate GNU Autotools input files in your package.
There is a script `autogen.sh` in the top-level source directory. You can
easily run it to update your project's output files, and rebuild the project:

```
cd ${ONNC} # go to the top-level source directory
./autogen.sh
```

The `configure` shell script attempts to guess correct values for
various system-dependent variables used during compilation.  It uses
those values to create a `Makefile` in each directory of the package.
It may also create one or more `.h` files containing system-dependent
definitions.  Finally, it creates a shell script `config.status` that
you can run in the future to recreate the current configuration, and a
file `config.log` containing compiler output (useful mainly for
debugging `configure`).

It can also use an optional file (typically called `config.cache`
and enabled with `--cache-file=config.cache` or simply `-C`) that saves
the results of its tests to speed up reconfiguring.  (Caching is
disabled by default to prevent problems with accidental use of stale
cache files.)

If you need to do unusual things to compile the package, please try
to figure out how `configure` could check whether to do them, and mail
diffs or instructions to the address given in the `README' so they can
be considered for the next release.  If you are using the cache, and at
some point `config.cache` contains results you don't want to keep, you
may remove or edit it.

   The file `configure.ac` (or `configure.in`) is used to create
`configure` by a program called `autoconf`.  You only need
`configure.ac` if you want to change it or regenerate `configure` using
a newer version of `autoconf`.

The simplest way to compile this package is:

1. `cd` to the directory containing the package's source code and type
   `./configure` to configure the package for your system.  If you're
   using `csh` on an old version of System V, you might need to type
   `sh ./configure` instead to prevent `csh` from trying to execute
   `configure` itself.

   Running `configure` takes awhile.  While running, it prints some
   Running `configure` takes awhile.  While running, it prints some
   messages telling which features it is checking for.

2. Type `make` to compile the package.

3. Optionally, type `make check` to run any self-tests that come with
   the package.

4. Type `make install` to install the programs and any data files and
   documentation.

5. You can remove the program binaries and object files from the
   source code directory by typing `make clean`.  To also remove the
   files that `configure` created (so you can compile the package for
   a different kind of computer), type `make distclean`.  There is
   also a `make maintainer-clean` target, but that is intended mainly
   for the package's developers.  If you use it, you may have to get
   all sorts of other programs in order to regenerate files that came
   with the distribution.

## x86 interpreter runtime -- onni

## Run onni:

```bash
$ cd ${ONNC_BUILD_FOLDER}/tools/onni && ./onni --help
NAME
        onni -- [Experimental] ONNI is the interpreter of ONNC

SYNOPSIS
        onni [options ...]

DESCRIPTION
        model        <path>       The onnx model file                               
        input        <path>       The input file                                    
        -o           <string>     The output file                                   
        --help                    Show this manual.                                 
        --verbose    =<number>    Set verbose level to <number> (default is 1).     
        -v                        One -v increases one verbose level.               
        --quiet                   Set verbose level to 0.                           
        --dry-run                 Do not do the inference, just print statistics.   
        --onnx-opt                Enable onnx optimizer                             
        -mquadruple  <string>     target quadruple                                  
        -march       <string>     target architecture                               

COPYRIGHT
        onni version 0.1.0
        https://onnc.ai
```

## Use onni to inference model from ONNX model zoo:
```bash

$ mkdir -p ~/onnx_model_zoo
$ cd ~/onnx_model_zoo && wget https://s3.amazonaws.com/download.onnx/models/opset_8/bvlc_alexnet.tar.gz
$ tar xvf bvlc_alexnet.tar.gz
$ cd ${ONNC_BUILD_FOLDER}/tools/onnc
$ ./onni ~/onnx_model_zoo/bvlc_alexnet/model.onnx ~/onnx_model_zoo/bvlc_alexnet/test_data_set_0/input_0.pb 

# you can also try verbose=4 to see details.

```
