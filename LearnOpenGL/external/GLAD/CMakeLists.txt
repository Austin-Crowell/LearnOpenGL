set(GLAD_SOURCES
  ./src/glad.c
)

add_library(glad STATIC ${GLAD_SOURCES})

target_include_directories(glad PUBLIC ./include)