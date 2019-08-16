project(onnc-runtime-example LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

add_library(inference
  client-app.c  
  client-lib.c
  onnc-runtime-service.c
  onnc-runtime-core.c
)
set_property(TARGET inference PROPERTY C_STANDARD 99)
add_definitions(-DONNC_TIME)
