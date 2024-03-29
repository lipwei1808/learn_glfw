#pragma once
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
  Shader(const std::string& filepath);
  ~Shader();
  void Bind() const;
  void Unbind() const;
  void SetUniform1i(const std::string& name, int value);
  void SetUniform1f(const std::string& name, float f0);
  void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
  void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private: 
  int GetUniformLocation(const std::string& name);
  ShaderProgramSource ParseShader();
  int CreateShader(const std::string& vertexShader, const std::string& fragmentShader); 
  unsigned int CompileShader(unsigned int type, const std::string& source);
private:
  unsigned int m_RendererID;
  std::string m_Filepath;
  std::unordered_map<std::string, int> m_UniformLocationCache;
};