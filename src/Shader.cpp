#include <iostream>
#include <fstream>
#include <sstream>

#include "glm/glm.hpp"
#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath): m_Filepath(filepath) {
  ShaderProgramSource source = ParseShader();
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
  
}
Shader::~Shader() {
  GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
  GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const {
  GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0) {
  GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0) {
  GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
    return m_UniformLocationCache[name];
  }
  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1) {
    std::cout << "Warning: Uniform " << name << " doesnt exist " << std::endl;
  }
  m_UniformLocationCache[name] = location;
  return location;
}

ShaderProgramSource Shader::ParseShader() {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };
    std::cout << "Reading from filepath: " << m_Filepath << std::endl;
    std::ifstream stream(m_Filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << std::endl;
        }
    }
    return {.VertexSource = ss[0].str(), .FragmentSource = ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLuint id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}