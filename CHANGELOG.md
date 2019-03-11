# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2019-3-11 Der-Yu Tsai <a127a127@skymizer.com>
### Added
- NVDLA Backend
- Vanilla Backend - A template for porting a new backend
- Interpreter interface
- ONNC IR
	- Add a stable topological sort for `ComputeGraph`
- Best-Fit linear scan memory allocation
- [Vanilla backend](https://github.com/ONNC/onnc/pull/97) - A template for porting a new backend
- Statistic API
    - [First release](./docs/api/statistics.md)
    - [Initial version](https://github.com/ONNC/onnc/pull/122) 
- Utility for pass
    - Template class `CustomPass<T>` for convenient to create new `ModulePass` type
- Utility for ONNC IR
    - Template class `CustomVisitor<T>` for convenient to create new `ComputeVisitor` type
### Changed
- ONNI
    - [ONNI v2](https://github.com/ONNC/onnc/pull/91)
        - Add `CountOperatorPass`: Operator counts statistics
        - Add `OnnxOptPass`: Run onnx optimizer
        - Add more verbose level for debugging or benchmarking.
    - [ONNI v3](https://github.com/ONNC/onnc/pull/133)
        - Rewrite the Interpreter interface.
        - Add best-fit algorithm to LinearScan pass, you can pass `-fLinearScanAlgo=best-fit` to onni to use it.
        - Add ComputeGraph::topologicalSort() to do the stable topological sort.
        - Add a new X86 ONNC IR: `X86ConvRelu` and `X86FuseConvReluPass` to demonstrate how to fuse operators.
- Runtime
    - [Support more operators in the X86 backend](https://github.com/ONNC/onnc/pull/100)
- PassManager
    - Add new template method `add<T>()` to the `PassManager` class for users to emplace a new `Pass` object in the pass manager.
    - Change `Pass` object storing mechanism in the pass manager, allowing multiple same type `Pass` objects.
### Fixed
 - Refine cmake building system
 - Fix Memory leaks in framework
 - Fix the shown error messags if running into unsupported ONNX operators in the model

## [0.9.3] - 2018-9-19 Evan Li  <evanli@skymizer.com>
### Executive Summary
1. Complete the x86 interpreter runtime. 
2. Enable FuseInplaceValue pass on x86 to further optimize memory footprint.

## [0.9.2] - 2018-9-19 Evan Li  <evanli@skymizer.com>
### Executive Summary
1. Complete the x86 memory allocation with liveness analysis information, and
   tested on experimental in-house runtime that will be released for 
   next version.
2. Integrate TravisCI with github PR system.

## [0.9.1] - 2018-8-3  Luba Tang <luba@skymizer.com>
First open source release: 0.9.0
### Executive Summary
1. Finish basic infrastructure of ONNC, including IR, Core, ADT, Support,
   Diagnostic and backends.
2. Enable Sophon backend. Now Sophon backend still use old form of compute
   operators.
3. Provides onnc, onnx2tg and some utilities.

## [0.9.0] - 2018-1-17	Luba Tang <luba@skymizer.com>
### Executive Summary
* Initialization: create all directory structures and auto-tool building system.

[Unreleased]: https://github.com/ONNC/onnc/compare/1.0.0...HEAD
[1.0.0]: https://github.com/ONNC/onnc/compare/0.9.3...1.0.0
[0.9.3]: https://github.com/ONNC/onnc/compare/0.9.2...0.9.3
[0.9.2]: https://github.com/ONNC/onnc/compare/0.9.1...0.9.2
[0.9.1]: https://github.com/ONNC/onnc/compare/0.9.0...0.9.1
[0.9.0]: https://github.com/ONNC/onnc/compare/57a6ca4bb7372a7a6cbcf1ce64695cb2aab99b8a...0.9.0

