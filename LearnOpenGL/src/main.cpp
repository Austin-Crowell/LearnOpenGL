#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <format>

#include "Camera.hpp"
#include "Shader.hpp"

float WindowWidth = 800.0f, WindowHeight = 600.0f;
float DeltaTime = 0.0f;

Camera Camera({0.0f, 0.0f, 3.0f});

void ProcessInput(GLFWwindow* Window);
void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void MouseMovementCallback(GLFWwindow* Window, double XPos, double YPos);
void ScrollCallback(GLFWwindow* Window, double XOffset, double YOffset);

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
  glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cerr << "Failed to initialize GLAD." << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
  glfwSetCursorPosCallback(Window, MouseMovementCallback);
  glfwSetScrollCallback(Window, ScrollCallback);

  Shader ShaderProgram("LearnOpenGL/src/Shaders/Vertex.glsl", "LearnOpenGL/src/Shaders/Fragment.glsl");

  const float VerticeData[]
  {
    // positions texture coords
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left
  };

  const uint32_t Indices[] =
  {
    0, 1, 3,
    1, 2, 3,
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  stbi_set_flip_vertically_on_load(true);

  uint32_t TextureID = 0;
  glGenTextures(1, &TextureID);
  glBindTexture(GL_TEXTURE_2D, TextureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  {
    int Width, Height, nrChannels;
    uint8_t* Data = stbi_load("LearnOpenGL/Assets/Textures/container.jpg", &Width, &Height, &nrChannels, 0);
    if (Data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
      std::cerr << "Failed to load texture image." << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(Data);
  }

  uint32_t TextureIDFace = 0;
  glGenTextures(1, &TextureIDFace);
  glBindTexture(GL_TEXTURE_2D, TextureIDFace);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  {
    int Width, Height, nrChannels;
    uint8_t* Data = stbi_load("LearnOpenGL/Assets/Textures/awesomeface.png", &Width, &Height, &nrChannels, 0);
    if (Data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
      std::cerr << "Failed to load texture image." << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(Data);
  }

  ShaderProgram.Use();
  ShaderProgram.SetIntParameter("u_Texture1", 0);
  ShaderProgram.SetIntParameter("u_Texture2", 1);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);

  float LastFrameTime = 0.0f;
  float AccumulateTime = 0.0f;
  uint32_t FPS = 0;

  while (!glfwWindowShouldClose(Window))
  {
    auto CurrentTime = static_cast<float>(glfwGetTime());
    DeltaTime = CurrentTime - LastFrameTime;
    LastFrameTime = CurrentTime;
    AccumulateTime += DeltaTime;
    FPS++;

    if (AccumulateTime > 1.0f)
    {
      AccumulateTime = 0.0f;
      std::string NewTitle = std::format("LearnOpenGL: FPS {0}", FPS);
      glfwSetWindowTitle(Window, NewTitle.c_str());
    }

    ProcessInput(Window);

    Camera.OnUpdate(DeltaTime);

    const glm::vec3& CamPosition = Camera.GetPosition();
    float Pitch = glm::radians(Camera.GetPitch());
    float Yaw = glm::radians(Camera.GetYaw());
    std::cout << "Camera Position: " << CamPosition.x << ", " << CamPosition.y << ", " << CamPosition.z << std::endl;
    std::cout << "Pitch: " << Pitch << std::endl;
    std::cout << "Yaw: " << Yaw << std::endl;

    glClearColor(0.392f, 0.584f, 0.930f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    auto Projection = glm::mat4(1.0f);
    Projection = glm::perspective(glm::radians(Camera.GetCameraZoom()), WindowWidth / WindowHeight, 0.1f, 100.0f);

    ShaderProgram.Use();
    ShaderProgram.SetMat4Parameter("u_Model", Model);
    ShaderProgram.SetMat4Parameter("u_View", Camera.GetViewMatrix());
    ShaderProgram.SetMat4Parameter("u_Projection", Projection);

    // Element Buffer Object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, TextureIDFace);

    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glfwSwapBuffers(Window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VertexArrayObject);
  glDeleteBuffers(1, &VertexBufferObject);
  glDeleteBuffers(1, &ElementBufferObject);

  glDeleteTextures(1, &TextureIDFace);
  glDeleteTextures(1, &TextureID);

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

  if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    Camera.ProcessMovement(Camera::CameraMovement::Forward, DeltaTime);
  if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    Camera.ProcessMovement(Camera::CameraMovement::Backward, DeltaTime);
  if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    Camera.ProcessMovement(Camera::CameraMovement::Right, DeltaTime);
  if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    Camera.ProcessMovement(Camera::CameraMovement::Left, DeltaTime);
}

void MouseMovementCallback(GLFWwindow* Window, double XPos, double YPos)
{
  std::cout << "MouseMove: " << XPos << ", " << YPos << std::endl;

  Camera.ProcessMouseMovement(static_cast<float>(XPos), static_cast<float>(YPos));
}

void ScrollCallback(GLFWwindow* Window, double XOffset, double YOffset)
{
  std::cout << "Scroll: " << XOffset << ", " << YOffset << std::endl;

  Camera.ProcessMouseScroll(static_cast<float>(YOffset));
}
