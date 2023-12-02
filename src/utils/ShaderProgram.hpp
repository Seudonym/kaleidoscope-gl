#pragma once
#include <GL/glew.h>
#include <string>

#include "common.hpp"

class ShaderProgram {
private:
  GLuint vertexShader, fragmentShader;
  GLuint program;

  UniformData uniformData;
public:
  ShaderProgram(std::string vertexShaderSource, std::string fragmentShaderSource);
  ShaderProgram(std::string vertexShaderSource, std::string fragmentShaderSource, UniformData uniformData);
  
  GLuint compileShader(std::string shaderSource, GLenum shaderType);
  GLuint compileProgram();

  void setUniformData(UniformData uniformData);
  void sendUniformData();
  void use();
  void unuse();
};
