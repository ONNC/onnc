# Testing

There are three kinds of testing in ONNC:

- [Unit Test](#Unit-Test)
- [Quick Regression](#Quick-Regression-Tests)
- [Full Regression](#Full-Regression-Tests)

## Unit Test

The unit tests ensure the correctness of the functionalities for each module.

The ONNC project adopts the [Skypat](https://skypat.skymizer.com/) testing framework for unit tests, which is developed by Skymizer Inc., the main maintainer of the ONNC.

The tests are located at [tools/unittest](https://github.com/ONNC/onnc/tree/master/tools/unittests)

After compiling, the build targets are in `${BUILD}/tools/unittests`. See more information about the build in [build.md]() (Not exist yet).

### Run Unit Tests

Run the tests simply use the command under the ONNC dictionary:

```sh
ctest
```

Or run the test executable in the targets dictionary for a certain test. For example:

```sh
./${BUILD_DIR}/tool/unittests/unittest_StringRef
```

### Add New Unit Tests

- Put the new unit test file `XXXTest.cpp` in `tools/unittest`
- Update `tools/unittests/Makefile.am`
- Update `tools/unittests/CMakeLists.txt`

## Quick Regression Tests

The quick regression tests run simplified functional and non-functional tests to ensure the software still performs after a change.

The regression tests are driven by CTest, the testing framework of CMake building system.

The tests are put under the counterpart project, [onnc-umbrella](https://github.com/ONNC/onnc-umbrella), and located at [onnc-umbrella/test/](https://github.com/ONNC/onnc-umbrella/tree/master/test).

### Run Quick Regression Test

1. Configure & Build
  Configure CMake to `Regression` mode and build:

    ```bash
    cmake -DCMAKE_BUILD_TYPE=Regression
    ```

2. Run CTest

    ```bash
    ctest
    # or in verbose mode
    ctest -v
    ```

### Add New Quick Regression Tests

1. Create a new directory inside `onnc-umbrella/test/regression`
2. Write a shell script named `main.sh`
    - ONNC binaries will be added to `PATH`
    - Results will be checked by exit values as below:
        - PASS: exit with `0`
        - FAILED: exit with a none-zero value

## Full Regression Tests

Not yet.