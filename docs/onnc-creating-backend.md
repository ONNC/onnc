ONNC - Creating Backend
=======================

# Introduction

This note introduces how to create a new backend. Basically we provide a script to ease the task. 

# Step 1: Enter docker container

You should run the script within the ONNC docker, so the first step is to enter ONNC docker prompt. The method is presented in [onnc-quick-start.md](./onnc-quick-start.md).

# Step 2: Run the script

Follow the below instructions to run the script. Here the Shell prompt `docker$` means we are within the ONNC docker.
```
docker$ cd /onnc/onnc-umbrella/src
docker$ ./scripts/create-new-backend.sh <YourBackendName>
```

Check if the new backend directory is created. Here we assume `<YourBackendName>` is `Foo`.
```
docker$ ls -l lib/Target/Foo
total 28
-rw-r--r-- 1 onnc onnc  131 Mar 15 13:15 CMakeLists.txt
-rw-r--r-- 1 onnc onnc 1403 Mar 15 13:15 CodeEmitVisitor.cpp
-rw-r--r-- 1 onnc onnc 1111 Mar 15 13:15 CodeEmitVisitor.h
-rw-r--r-- 1 onnc onnc 5189 Mar 15 13:15 FooBackend.cpp
-rw-r--r-- 1 onnc onnc  841 Mar 15 13:15 FooBackend.h
-rw-r--r-- 1 onnc onnc  180 Mar 15 13:15 Makefile.am
drwxr-xr-x 6 onnc onnc  192 Mar 15 13:15 TargetInfo
```

