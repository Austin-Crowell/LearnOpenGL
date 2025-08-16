#include "Shader.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
  std::string VertexSource;
  std::string FragmentSource;

  std::ifstream VertexFile;
  std::ifstream FragmentFile;

  VertexFile.open(VertexPath);
  FragmentFile.open(FragmentPath);
  std::stringstream VertexStream, FragmentStream;

  if (!VertexFile.is_open() || !FragmentFile.is_open())
  {
    std::cout << "ERROR::SHADER::COULD_NOT_OPEN" << std::endl;
    return;
  }

  VertexStream << VertexFile.rdbuf();
  FragmentStream << FragmentFile.rdbuf();

  VertexFile.close();
  FragmentFile.close();

  VertexSource = VertexStream.str();
  FragmentSource = FragmentStream.str();

  const char* VertexShaderSource = VertexSource.c_str();
  const char* FragmentShaderSource = FragmentSource.c_str();

  uint32_t Vertex, Fragment;
  Vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(Vertex, 1, &VertexShaderSource, nullptr);
  glCompileShader(Vertex);
  CheckCompileErrors(Vertex, ShaderType::Vertex);

  Fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(Fragment, 1, &FragmentShaderSource, nullptr);
  glCompileShader(Fragment);
  CheckCompileErrors(Fragment, ShaderType::Fragment);

  m_ProgramID = glCreateProgram();
  glAttachShader(m_ProgramID, Vertex);
  glAttachShader(m_ProgramID, Fragment);
  glLinkProgram(m_ProgramID);

  CheckProgramErrors(m_ProgramID);

  glDeleteShader(Vertex);
  glDeleteShader(Fragment);
}

void Shader::SetBoolParameter(const std::string &ParameterName, bool Value) const
{
  glUniform1i(glGetUniformLocation(m_ProgramID, ParameterName.c_str()), Value);
}

void Shader::SetIntParameter(const std::string &ParameterName, int Value) const
{
  glUniform1i(glGetUniformLocation(m_ProgramID, ParameterName.c_str()), Value);
}

void Shader::SetFloatParameter(const std::string &ParameterName, float Value) const
{
  glUniform1f(glGetUniformLocation(m_ProgramID, ParameterName.c_str()), Value);
}

void Shader::CheckCompileErrors(const uint32_t Shader, const ShaderType Type) const
{
  int IsSuccess;
  char LogInfo[512];

  glGetShaderiv(Shader, GL_COMPILE_STATUS, &IsSuccess);
  if (!IsSuccess)
  {
    glGetShaderInfoLog(Shader, 512, nullptr, LogInfo);
    std::cout << "ERROR::SHADER::" << Type << "::COMPILE_ERRORS:\n" << LogInfo << std::endl;
  }
}

void Shader::CheckProgramErrors(const uint32_t Program) const
{
  int IsSuccess;
  char LogInfo[512];

  glGetProgramiv(Program, GL_LINK_STATUS, &IsSuccess);
  if (!IsSuccess)
  {
    glGetProgramInfoLog(Program, 512, nullptr, LogInfo);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_ERROR:\n" << LogInfo << std::endl;
  }
}
