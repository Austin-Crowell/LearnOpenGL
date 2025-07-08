#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD." << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

  constexpr IntRGBA CornFlowerBlueInt = { .Red = 100, .Green = 149, .Blue = 237, .Alpha = 1 };
  constexpr FloatRGBA CornFlowerBlueFloat = IntRGBAToFloat(CornFlowerBlueInt);

  while (!glfwWindowShouldClose(Window))
  {
    ProcessInput(Window);

    glClearColor(CornFlowerBlueFloat.Red, CornFlowerBlueFloat.Green, CornFlowerBlueFloat.Blue, CornFlowerBlueFloat.Alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(Window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void FramebufferSizeCallback(GLFWwindow* Window, const int Width, const int Height)
{
  std::cout << "Framebuffer size: " << Width << ", " << Height << std::endl;
  glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow* Window)
{
  if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(Window, true);
}
