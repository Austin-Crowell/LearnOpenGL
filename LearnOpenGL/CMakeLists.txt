set(LEARN_OPENGL_SOURCES
  ./src/main.cpp
  ./src/Shader.cpp
  ./src/Shader.hpp
)

add_subdirectory(external/GLFW)
add_subdirectory(external/GLAD)

add_executable(LearnOpenGL ${LEARN_OPENGL_SOURCES})

target_link_libraries(LearnOpenGL PRIVATE glfw)
target_link_libraries(LearnOpenGL PRIVATE glad)