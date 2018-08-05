# Introduction

**ONNC (Open Neural Network Compiler)**

A compiler specifically designed for Deep Learning Accelerator (DLA).
Based on Open Neural Network Exchange (ONNX), ONNC transforms ONNX models into
binary machine code for DLA ASIC. ONNC leverages the IR design of ONNX and
provides effective algorithms to eliminate the overhead of data movement.
ONNC increases DLA ASIC performance and shorten production time for DLA ASIC.

## Current Status

* [v0.9](https://github.com/ONNC/onnc/releases)
 
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

