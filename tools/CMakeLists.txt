
add_subdirectory(unittests)
add_subdirectory(onnc)
if (TARGET_TG)
    add_subdirectory(onnx2tg)
endif()
