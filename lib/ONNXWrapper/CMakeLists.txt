# FIXME: Get rid of this one day if onnx can turn off c++ exceptions.
if(CMAKE_COMPILER_IS_GNUCXX OR
   "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR
   "${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  add_compile_options(-fexceptions)
endif()

add_libonnc_src(
    ONNXWrapper.cpp)
