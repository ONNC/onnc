include_directories(${ONNC_INCLUDE_DIRS} ${ONNC_ROOT_PATH}/export/runtime/include)

add_executable(pb2t main.cpp)
target_link_libraries(pb2t libonnc)

install(TARGETS pb2t
    RUNTIME DESTINATION bin)
