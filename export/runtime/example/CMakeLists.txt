project(onnc-runtime-example LANGUAGES C)


add_library(inference
  client-app.c  
  client-lib.c
  onnc-runtime-service.c
  onnc-runtime-core.c
)
