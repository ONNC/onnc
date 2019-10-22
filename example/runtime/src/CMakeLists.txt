project(inference LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(${PROJECT_INCLUDE_DIR})

add_executable(inference
  client-app.c  
  client-lib.c
  onnc-runtime-service.c
  onnc-runtime-core.c
)

target_link_libraries(inference
  onnc-rt
  m
)
