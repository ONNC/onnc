2018-9-19 Evan Li  <evanli@skymizer.com>
  * release: 0.9.3
  * Executive Summary:
  1.  Complete the x86 interpreter runtime. 
  2.  Enable FuseInplaceValue pass on x86 to further optimize memory footprint.

2018-9-19 Evan Li  <evanli@skymizer.com>
  * release: 0.9.2
  * Executive Summary:
  1.  Complete the x86 memory allocation with liveness analysis information, and
      tested on experimental in-house runtime that will be released for 
      next version.
  2.  Integrate TravisCI with github PR system.

2018-8-3  Luba Tang <luba@skymizer.com>
  * First open source release: 0.9.0
	* Executive Summary:
	1. Finish basic infrastructure of ONNC, including IR, Core, ADT, Support,
	   Diagnostic and backends.
	2. Enable Sophon backend. Now Sophon backend still use old form of compute
	   operators.
  3. Provides onnc, onnx2tg and some utilities.

2018-1-17	Luba Tang <luba@skymizer.com>
	* Initialization: create all directory structures and auto-tool building
	system.
