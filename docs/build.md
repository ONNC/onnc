# Build & Test from source

You can build onnc source with either [CMake](#building-with-cmake) or [Automake](#building-with-automake) building system.

# Building with CMake

There are 2 modes, [Release mode](#release-mode) and [debug mode](#debug-mode), provided in CMake building system. 

## Release mode

> Recommended for common use.

Release mode will build onnc with basic optimizations, and skip debug informations.

**Notice: Release mode will NOT build unit tests.**

### Step 1: Configure

```shell
$ mkdir -p build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```

It will create `build` directory, get into it and configure with `Release` build type.

### Step 2: Build

```shell
$ make -j4
```

It will compile sources with 4 threads concurrently. You can modify `-j4` option to meet your needs.

### Step 3: Install

```shell
$ sudo make install
```

It will install essential files to `/usr/local`.

## Debug mode

> Recommended for development use.

Debug mode will build onnc with debug settings, unit tests and some debug informations.

### Step 1: Configure

```shell
$ mkdir -p build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```

It will create `build` directory, get into it and configure with `Debug` build type.

### Step 2: Build

```shell
$ make -j4
```

It will compile sources with 4 threads concurrently. You can modify `-j4` option to meet your needs.

### Step 3: Test

```shell
$ ctest
```

It will trigger all unit tests by CTest.

If you want to execute single unit test, you can find it in `build/tools/unittests` directory.

### Step 4: Install

```shell
$ sudo make install
```

It will install essential files to `/usr/local`.

## Advanced configurations

You can override default configurations by simply add `-DENTRY=VALUE` options to the configure command. The `ENTRY` and `VALUE` can be replaced according to the following table:

"Path string" of value means you have to provide a string of a directory path, and "File path string" means you have to provide a string of a file path.

| Entry | Available values | Default | Description |
|-------|------------------|---------|-------------|
|CMAKE_INSTALL_PREFIX| Path string | `/usr/local` | Path to install onnc into |
|CMAKE_BUILD_TYPE| `Debug`,`Release` | `Release` | Build mode |
|LLVM_CONFIG| File path string | System default path | Path of `llvm-config` | 
|ONNX_INCLUDE_DIR| Path string | System default path | Path to include directory of onnx |
|ONNX_LIBRARIES| File path string list | System default paths | Path of `onnx` and `onnx_proto` libraries|
|ONNX_LIBRARIE_onnx| File path string | System default path | Path of `onnx` library |
|ONNX_LIBRARIE_onnx_proto| File path string | System default path | Path of `onnx_proto` library |
|ONNX_NAMESPACE| string | `onnx` | Namespace of onnx |
|SKYPAT_INCLUDE_DIR| Path string | `/usr/local/include` | Path to include directory of SkyPat |
|SKYPAT_LIBRARIES| File path string | System default path | Path of `SkyPat` library |
|TARGET_ALL| `ON`,`OFF` | `ON` | Whether to build all targets |
|TARGET_SOPHON| `ON`,`OFF` | `ON` | Whether to build Sophon targets |
|TARGET_X86| `ON`,`OFF` | `ON` | Whether to build X86 targets |
|USE_LLVM| `ON`,`OFF` | `ON` | Whether to use LLVM |
|ENABLE_CLOCK_GETTIME| `ON`,`OFF` | `OFF` | Whether to enable `clock_gettime` function |
|ENABLE_GETTIMEOFDAY| `ON`,`OFF` | `ON` | Whether to enable `gettimeofday` function |
|ENABLE_PTHREAD| `ON`,`OFF` | `ON` | Whether to enable pthread |
|ENABLE_UNITTEST| `ON`,`OFF` | `ON` | Whether to build unit test |
|ENABLE_WERROR| `ON`,`OFF` | `OFF` | Whether to use `-Werror` flag |

Use `cmake -L ..` inside `build` directory to get more detailed options.

