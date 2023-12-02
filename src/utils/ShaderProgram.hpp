#pragma once
#include <GL/glew.h>
#include <string>

#include "common.hpp"

class ShaderProgram {
private:
  GLuint vertexShader, fragmentShader;
  GLuint program;

public:
  ShaderProgram(std::string vertexShaderSource, std::string fragmentShaderSource);
  
  GLuint compileShader(std::string shaderSource, GLenum shaderType);
  GLuint compileProgram();
};
