#pragma once

#include <glad/glad.h>

#include <string>
#include <iostream>

class Shader
{
public:
  enum class ShaderType
  {
    None = 0, Vertex, Fragment
  };
public:
  Shader(const char* VertexPath, const char* FragmentPath);
  ~Shader() { glDeleteProgram(m_ProgramID); }

  void Use() const { glUseProgram(m_ProgramID); }

  void SetBoolParameter(const std::string& ParameterName, bool Value) const;
  void SetIntParameter(const std::string& ParameterName, int Value) const;
  void SetFloatParameter(const std::string& ParameterName, float Value) const;
private:
  void CheckCompileErrors(const uint32_t Shader, const ShaderType Type) const;
  void CheckProgramErrors(const uint32_t Program) const;
private:
  uint32_t m_ProgramID;
};

inline std::ostream& operator<<(std::ostream& os, const Shader::ShaderType& type)
{
  switch (type)
  {
    case Shader::ShaderType::None:     os << "NONE"; break;
    case Shader::ShaderType::Vertex:   os << "VERTEX"; break;
    case Shader::ShaderType::Fragment: os << "FRAGMENT"; break;
    default:                           os << "UNKNOWN"; break;
  }
  return os;
}
