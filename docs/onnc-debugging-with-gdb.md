ONNC - Debugging with gdb
================

# Introduction

有時用 printf 來除錯實在是很麻煩，更有效率的方法是使用 gdb 之類的工具。本文介紹如何將 ONNC compile 成 debug mode ， 以便使用 gdb 來除錯。

# Step 1: 進入 docker container

按以下指令進入 docker prompt。注意`--cap-add=SYS_PTRACE`是很重要的參數，有它才能在 docker 裡跑`gdb`。
```
$ docker run -it --rm --cap-add=SYS_PTRACE -v <your/working/directory>/onnc:/onnc/onnc-umbrella/src onnc/onnc-community bash
```


# Step 2: 安裝 gdb 

ONNC docker image 裡原廠並沒有安裝 gdb 。你可以在進入 docker prompt 後安裝。
```
docker$ sudo apt-get update
docker$ sudo apt-get install gdb
```

# Step 3: 第一次 compile ONNC 成為可 debug 的執行檔

你可能注意到在 ONNC Quick Start 文件中，我們是到 `/onnc/onnc-umbrella/build-normal` 下去做 make。在那個地方 compile 出來的執行檔是不含 debug 資訊的，所以不能被 gdb 讀入。現在我們要重新 build 一個目錄叫 `/onnc/onnc-umbrella/build-dbg` ，裡面的 Makefile 將包含 debug flags 。
```
docker$ cd /onnc/onnc-umbrella
docker$ ./build.cmake.sh dbg
```

你應該看到以下執行結果。
```
  (...skipping...)
[ 98%] Building CXX object tools/onnx2tg/CMakeFiles/onnx2tg.dir/ONNX2TGApp.cpp.o
[ 98%] Building CXX object tools/onnx2tg/CMakeFiles/onnx2tg.dir/Config.cpp.o
[ 99%] Linking CXX executable onnx2tg
[ 99%] Built target onnx2tg
Scanning dependencies of target onnc-jit
[ 99%] Building CXX object tools/onnc-jit/CMakeFiles/onnc-jit.dir/main.cpp.o
[ 99%] Building CXX object tools/onnc-jit/CMakeFiles/onnc-jit.dir/ONNCJITApp.cpp.o
[100%] Building CXX object tools/onnc-jit/CMakeFiles/onnc-jit.dir/ONNCJITConfig.cpp.o
[100%] Linking CXX executable onnc-jit
[100%] Built target onnc-jit
Install the project...
-- Install configuration: "Debug"
-- Installing: /onnc/onnc-umbrella/install-dbg/lib/libonnc.a
-- Installing: /onnc/onnc-umbrella/install-dbg/include
-- Installing: /onnc/onnc-umbrella/install-dbg/include/onnc
-- Installing: /onnc/onnc-umbrella/install-dbg/include/onnc/Target
  (...skipping...)
[/onnc/onnc-umbrella/build-dbg/umbrella/tools] building tools: 'onnx-dis'
make: Entering directory '/onnc/onnc-umbrella/tools/onnx-dis'
g++ -std=c++14 -DONNX_NAMESPACE=onnx -o /onnc/onnc-umbrella/build-dbg/umbrella/tools/onnx-dis/onnx-dis -I/onnc/onnc-umbrella/install-dbg/include -I/onnc/onnc-umbrella/onncroot/include -L/onnc/onnc-umbrella/install-dbg/lib -L/onnc/onnc-umbrella/onncroot/lib main.cpp -lonnc -lonnx -lonnx_proto -lprotobuf
cp /onnc/onnc-umbrella/build-dbg/umbrella/tools/onnx-dis/onnx-dis /onnc/onnc-umbrella/install-dbg/bin/onnx-dis
make: Leaving directory '/onnc/onnc-umbrella/tools/onnx-dis'
[/onnc/onnc-umbrella] packaging tarball '/onnc/onnc-umbrella/onnc-research-public.tar.gz'
[/onnc/onnc-umbrella] build /onnc/onnc-umbrella/onnc-research-public.tar.gz for installation on /onnc/onnc-umbrella/install-dbg: success
```

產生出來的 ONNC 執行檔是`/onnc/onnc-umbrella/build-dbg/tools/onnc/onnc` 

# Step 4: 啟動 gdb

```
docker$ gdb --args /onnc/onnc-umbrella/build-dbg/tools/onnc/onnc -mquadruple vanilla /models/bvlc_alexnet/model.onnx
```

# Step 5: 修改程式碼後再次 compile

這次作法和 Step 3 不一樣，因為 `/onnc/onnc-umbrella/build-dbg` 目錄已經 build 出來了。當你要再次 compile 只要到此目錄下 make 即可。
```
docker$ cd /onnc/onnc-umbrella/build-dbg
docker$ make -j8
```
