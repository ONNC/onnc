
add_subdirectory(unittests)
add_subdirectory(onnc)
add_subdirectory(calibration)
if (TARGET_TG)
    add_subdirectory(onnx2tg)
endif()
