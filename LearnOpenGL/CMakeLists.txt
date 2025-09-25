set(LEARN_OPENGL_SOURCES
  ./src/main.cpp
  ./src/Shader.cpp
  ./src/Shader.hpp
  ./src/Camera.cpp
  ./src/Camera.hpp
)

add_subdirectory(external/GLFW)
add_subdirectory(external/GLAD)
add_subdirectory(external/stb_image)
add_subdirectory(external/glm)

add_executable(LearnOpenGL ${LEARN_OPENGL_SOURCES})

target_link_libraries(LearnOpenGL PRIVATE glfw)
target_link_libraries(LearnOpenGL PRIVATE glad)
target_link_libraries(LearnOpenGL PRIVATE stb_image)
target_link_libraries(LearnOpenGL PRIVATE glm)