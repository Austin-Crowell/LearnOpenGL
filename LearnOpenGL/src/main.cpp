#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "Shader.hpp"

#define USE_EBO false

int WindowWidth, WindowHeight;

void FramebufferSizeCallback(GLFWwindow* Window, const int Width, const int Height);

void ProcessInput(GLFWwindow* Window);

struct IntRGBA
{
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  uint8_t Alpha;
};

struct FloatRGBA
{
  float Red;
  float Green;
  float Blue;
  float Alpha;
};

struct Vector3
{
  float x;
  float y;
  float z;
};

Vector3 Normalize3DCoordiates(const Vector3& Vector);

constexpr FloatRGBA IntRGBAToFloat(const IntRGBA Color)
{
  FloatRGBA Result{};
  Result.Red =   static_cast<float>(Color.Red)   / static_cast<float>(255);
  Result.Green = static_cast<float>(Color.Green) / static_cast<float>(255);
  Result.Blue =  static_cast<float>(Color.Blue)  / static_cast<float>(255);
  Result.Alpha = static_cast<float>(Color.Alpha) / static_cast<float>(255);
  return Result;
}

constexpr IntRGBA FloatRGBAToIntRGBA(const FloatRGBA Color)
{
  IntRGBA Result{};
  Result.Red = static_cast<uint8_t>(Color.Red)     * 255.0f;
  Result.Green = static_cast<uint8_t>(Color.Green) * 255.0f;
  Result.Blue = static_cast<uint8_t>(Color.Blue)   * 255.0f;
  Result.Alpha = static_cast<uint8_t>(Color.Alpha) * 255.0f;
  return Result;
}

int main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* Window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (Window == nullptr)
  {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(Window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cerr << "Failed to initialize GLAD." << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

  constexpr IntRGBA CornFlowerBlueInt = { .Red = 100, .Green = 149, .Blue = 237, .Alpha = 1 };
  constexpr FloatRGBA CornFlowerBlueFloat = IntRGBAToFloat(CornFlowerBlueInt);

  Shader ShaderProgram("LearnOpenGL/src/Shaders/Vertex.glsl", "LearnOpenGL/src/Shaders/Fragment.glsl");

  const float VerticeData[]
  {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Left
    0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f// Top
  };

#if USE_EBO
  const uint32_t Indices[] =
  {
    0, 1, 2
  };

  // Element Buffer Object
  uint32_t VertexArrayObject = 0;
  uint32_t VertexBufferObject = 0;
  uint32_t ElementBufferObject = 0;

  glGenVertexArrays(1, &VertexArrayObject);
  glGenBuffers(1, &VertexBufferObject);
  glGenBuffers(1, &ElementBufferObject);

  glBindVertexArray(VertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticeData), VerticeData, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
#else
  // Vertex Array Object
  uint32_t VertexArrayObject = 0;
  uint32_t VertexBufferObject = 0;

  glGenVertexArrays(1, &VertexArrayObject);
  glGenBuffers(1, &VertexBufferObject);

  glBindVertexArray(VertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticeData), VerticeData, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
#endif

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(Window))
  {
    const float Time = static_cast<float>(glfwGetTime());
    ProcessInput(Window);

    glClearColor(CornFlowerBlueFloat.Red, CornFlowerBlueFloat.Green, CornFlowerBlueFloat.Blue, CornFlowerBlueFloat.Alpha);
    glClear(GL_COLOR_BUFFER_BIT);
#if USE_EBO
    // Element Buffer Object
    ShaderProgram.Use();
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
#else
    // Vertex Array Object
    ShaderProgram.Use();
    glBindVertexArray(VertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
#endif
    glfwSwapBuffers(Window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void FramebufferSizeCallback(GLFWwindow* Window, const int Width, const int Height)
{
  std::cout << "Framebuffer size: " << Width << ", " << Height << std::endl;

  WindowWidth = Width;
  WindowHeight = Height;

  glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow* Window)
{
  if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(Window, true);
}

Vector3 Normalize3DCoordiates(const Vector3& Vector)
{
  Vector3 Result{};

  Result.x = Vector.x / static_cast<float>(WindowWidth);
  Result.y = Vector.y / static_cast<float>(WindowHeight);
  Result.z = 0;

  return Result;
}
