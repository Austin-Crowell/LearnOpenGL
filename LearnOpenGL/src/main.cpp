#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD." << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

  constexpr IntRGBA CornFlowerBlueInt = { .Red = 100, .Green = 149, .Blue = 237, .Alpha = 1 };
  constexpr FloatRGBA CornFlowerBlueFloat = IntRGBAToFloat(CornFlowerBlueInt);

  const char* VertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos, 1.0);\n"
    "}\n";

  const char* FragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

  uint32_t VertexShader = glCreateShader(GL_VERTEX_SHADER);
  uint32_t FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(VertexShader, 1, &VertexShaderSource, nullptr);
  glCompileShader(VertexShader);

  glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);
  glCompileShader(FragmentShader);

  int IsSuccessful = 0;
  char LogInfo[512];
  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &IsSuccessful);

  if (!IsSuccessful)
  {
    glGetShaderInfoLog(VertexShader, 512, nullptr, LogInfo);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n" << LogInfo << std::endl;
  }

  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &IsSuccessful);

  if (!IsSuccessful)
  {
    glGetShaderInfoLog(FragmentShader, 512, nullptr, LogInfo);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n" << LogInfo << std::endl;
  }

  uint32_t ShaderProgram = glCreateProgram();

  glAttachShader(ShaderProgram, VertexShader);
  glAttachShader(ShaderProgram, FragmentShader);

  glLinkProgram(ShaderProgram);

  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &IsSuccessful);

  if (!IsSuccessful)
  {
    glGetProgramInfoLog(ShaderProgram, 512, nullptr, LogInfo);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << LogInfo << std::endl;
  }

  glDeleteShader(VertexShader);
  glDeleteShader(FragmentShader);

  VertexShader = FragmentShader = 0;

  const float VerticeData[]
  {
    -0.05f,  0.5f, 0.0f,   // Top Right
    -0.05f, -0.5f, 0.0f,   // Bottom Right
    -0.80f, -0.5f, 0.0f,   // Bottom Left
    -0.80f,  0.5f, 0.0f,   // Top Right
  };

  const uint32_t Indices[] =
  {
    0, 1, 3, // First Triangle
    1, 2, 3, // Second Triangle
  };

  float Triangles[]
  {
    0.8f,  0.5f, 0.0f,   // Top Right
    0.8f,  -0.5f, 0.0f,  // Bottom Right
    0.05f, 0.5f, 0.0f,   // Top Left

    0.8f, -0.5f, 0.0f,  // Bottom Right
    0.05f,  -0.5f, 0.0f,  // Bottom Left
    0.05f,  0.5f, 0.0f,  // Top Left
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
  glEnableVertexAttribArray(0);

  // Vertex Array Object
  uint32_t VertexArrayObjectTriangles = 0;
  uint32_t VertexBufferObjectTriangles = 0;

  glGenVertexArrays(1, &VertexArrayObjectTriangles);
  glGenBuffers(1, &VertexBufferObjectTriangles);

  glBindVertexArray(VertexArrayObjectTriangles);

  glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectTriangles);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles), Triangles, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(Window))
  {
    ProcessInput(Window);

    glClearColor(CornFlowerBlueFloat.Red, CornFlowerBlueFloat.Green, CornFlowerBlueFloat.Blue, CornFlowerBlueFloat.Alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    // Element Buffer Object
    glUseProgram(ShaderProgram);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // Vertex Array Object
    glUseProgram(ShaderProgram);
    glBindVertexArray(VertexArrayObjectTriangles);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

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
