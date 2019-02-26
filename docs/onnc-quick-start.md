ONNC Quick Start
================

# Introduction

本文介紹如何獲得、編譯、及執行 ONNC。

# Step 1: 下載 ONNC 

首先從 gitlab 下載 ONNC 原始碼在你的 PC 上。
```
$ cd <your/working/directory>
$ git clone https://gitlab.com/skymizer/ccu-chang/onnc.git
$ cd onnc
```

# Step 2: 做你的修改

理論上你下載 ONNC 的目的是要造一個 backend ，ONNC 提供一個 backend 空殼子叫 Vanilla ，你可以加自己的程式碼在 Vanilla backend 。
```
$ cd lib/Target/Vanilla
$ vi CodeEmitVisitor.cpp
```

# Step 3: 取得 ONNC docker image

編輯完畢後，編譯的方法是我們提供了一個 docker image ，裡面已經將編譯環境建好，compiler dependency tools 已安裝完畢，用這個可省下你很多功夫。假設你已經安裝 docker app 在你的 PC 上。

得到 ONNC docker image。
```
$ docker pull onnc/onnc-community
```

# Step 4: Compile 你的修改

docker image 事實上就是輕量版的 VirtualBox ，將 ONNC docker image 跑起來然後進入裡面的 Ubuntu shell prompt 。
```
$ docker run -it --rm --cap-add=SYS_PTRACE -v <your/working/directory>/onnc:/onnc/onnc-umbrella/src onnc/onnc-community bash
```
`-it`
: for interactive mode. Can use stdin and stdout for typing commands and displaying response.

`--rm`
: remove the container after exiting.

`--cap-add=SYS_PTRACE`
: enable debug support (like `gdb`) within docker.

`-v <your/working/directory>/onnc:/onnc/onnc-umbrella/src`
: **重要！將你在 PC 上的 ONNC 原始碼 mount 進 docker 中，這樣在 docker 裡面才能 compile 到你改的程式碼。**
: `<your/working/directory>/onnc` 是 PC 上放 ONNC 原始碼的路徑，這部分要根據你自己的狀況更改
: `/onnc/onnc-umbrella/src` 是在 docker 中 mount 進來的路徑，這部分不要動

`onnc/onnc-community`
: the name of the docker image to run.

`bash`
: within the docker container, we will be under the bash Shell prompt.

進到 docker shell prompt 之後，你應該要看到如下畫面
```
onnc@f10d3d4018f9:/onnc/onnc-umbrella/build-normal$
```

接下來的操作要在 docker shell prompt 中進行，我們用 `docker$ ` 代表在 docker prompt 中下指令。編譯方式如下。
```
docker$ cd /onnc/onnc-umbrella/build-normal
docker$ make -j8
```
`-j8`
: 最多用到 CPU 八核做 compile ，加快 compile 速度。


預期執行結果如下。
```
-- Looking for unordered/hash map/set headers
-- Default target quadruple: x86_64-unknown-linux-gnu
-- [onnc] Using onnx include at /onnc/onnc-umbrella/onncroot/include
-- [onnc] Using onnx library at /onnc/onnc-umbrella/onncroot/lib
-- [onnc] Using SkyPat include at /onnc/onnc-umbrella/onncroot/include
-- [onnc] Using llvm include at /usr/lib/llvm-5.0/include
-- Found PROTOBUF: /usr/include
-- Found GLOG: /usr/include
-- [onnc] Using glog include at /usr/include
-- Configuring done
-- Generating done
-- Build files have been written to: /onnc/onnc-umbrella/build-normal
[  0%] Built target libonnc_Target_Sophon_BM188x_asm__bm188x_asm_proto_dir
[  0%] Generating JsonParser.cpp, JsonParser.h
[  0%] Built target libonnc_Target_Sophon_BM188x__common_calibration2_proto_dir
[  0%] Built target libonnc_Target_Sophon_BM168x_asm__bm168x_asm_proto_dir
[  0%] Generating JsonScanner.cpp
  (...skipping...)
[ 96%] Linking CXX executable onni
[ 97%] Linking CXX executable unittest_Digraph
[ 97%] Built target unittest_Digraph
[ 97%] Building CXX object tools/unittests/CMakeFiles/unittest_Json.dir/JsonObjectTest.cpp.o
[ 97%] Built target onni
Scanning dependencies of target unittest_Runtime_Abs
[ 97%] Building CXX object tools/unittests/Runtime/CMakeFiles/unittest_Runtime_Abs.dir/AbsTest.cpp.o
[ 97%] Linking CXX executable unittest_Runtime_Transpose
[ 97%] Built target unittest_Runtime_Transpose
[ 97%] Building CXX object tools/unittests/CMakeFiles/unittest_Json.dir/StorageTest.cpp.o
[ 97%] Linking CXX executable unittest_Runtime_Abs
[ 97%] Built target unittest_Runtime_Abs
[ 98%] Linking CXX executable unittest_Json
[ 98%] Built target unittest_Json
[ 99%] Linking CXX executable unittest_StringMap
[ 99%] Built target unittest_StringMap
[100%] Linking CXX executable unittest_TensorSel
[100%] Built target unittest_TensorSel
[100%] Linking CXX executable unittest_MemAllocTest
[100%] Built target unittest_MemAllocTest
[100%] Linking CXX executable unittest_PassManager
[100%] Built target unittest_PassManager
```

# Step 5: 執行 ONNC Vanilla backend 看你修改的結果

如果 compile 沒問題，就可以執行。我們建議你在 Step 2 先不要急著修改。先用原版的程式碼看看能否編譯並成功執行。確定原版沒問題，再作修改。如果修改後發生錯誤，就是你的問題了。

以下是執行 vanilla backend 並讀進 alexnet model。
```
docker$ /onnc/onnc-umbrella/build-normal/tools/onnc/onnc -mquadruple vanilla /models/bvlc_alexnet/model.onnx
```

你應該要看到下列結果。
```
Vanilla is invoked
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
Initializer
InputOperator
%conv1_1<float>[1, 96, 54, 54] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 1, kernel_shape: [11, 11], pads: [0, 0, 0, 0], strides: [4, 4]>(%data_0<float>[1, 3, 224, 224], %conv1_w_0<float>[96, 3, 11, 11], %conv1_b_0<float>[96])
%conv2_1<float>[1, 256, 26, 26] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 2, kernel_shape: [5, 5], pads: [2, 2, 2, 2], strides: [1, 1]>(%pool1_1<float>[1, 96, 26, 26], %conv2_w_0<float>[256, 48, 5, 5], %conv2_b_0<float>[256])
%conv3_1<float>[1, 384, 12, 12] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 1, kernel_shape: [3, 3], pads: [1, 1, 1, 1], strides: [1, 1]>(%pool2_1<float>[1, 256, 12, 12], %conv3_w_0<float>[384, 256, 3, 3], %conv3_b_0<float>[384])
%conv4_1<float>[1, 384, 12, 12] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 2, kernel_shape: [3, 3], pads: [1, 1, 1, 1], strides: [1, 1]>(%conv3_2<float>[1, 384, 12, 12], %conv4_w_0<float>[384, 192, 3, 3], %conv4_b_0<float>[384])
%conv5_1<float>[1, 256, 12, 12] = Conv<auto_pad: "NOTSET", dilations: [1, 1], group: 2, kernel_shape: [3, 3], pads: [1, 1, 1, 1], strides: [1, 1]>(%conv4_2<float>[1, 384, 12, 12], %conv5_w_0<float>[256, 192, 3, 3], %conv5_b_0<float>[256])
OutputOperator
```

# Step 6: 再修改 ONNC 原始碼

**不要離開 docker prompt** ，你可以開另一新視窗在 docker 外面用你自己熟悉的 IDE 去編輯程式碼。因為 docker 有 mount 到外面 PC 上的 ONNC 程式碼，所以你在外面的改變在 docker 內部看得到。

再次強調，在你修改的同時不要離開 docker prompt 。因為你離開後剛剛 compile 的結果就會消失，你就沒辦法做 incremental compilation 。

不要離開 docker prompt 的意思是說你沒有在 docker prompt 下 `exit` 指令。

# Step 7: 再次 compile 你的修改

假設你沒有離開 docker prompt ，完成修改原始碼後，回到 docker prompt 視窗。
```
docker$ cd /onnc/onnc-umbrella/build-normal
docker$ make -j8
```

你會發現只有修改過的檔案需要重新編譯。時間上會比第一次編譯快很多。

