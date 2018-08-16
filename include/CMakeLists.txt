install(DIRECTORY onnc 
    DESTINATION include
    FILES_MATCHING
    PATTERN "*"
    PATTERN "*.in" EXCLUDE
    PATTERN "Makefile.am" EXCLUDE
)

install(DIRECTORY ${PROJECT_BINARY_DIR}/include/onnc
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.def"
    PATTERN "Target/*" EXCLUDE
)
