
add_subdirectory(unittests)
add_subdirectory(onnc)
add_subdirectory(calibration)
add_subdirectory(onnx-dis)
add_subdirectory(onnx-as)
if (TARGET_TG)
    add_subdirectory(onnx2tg)
endif()
