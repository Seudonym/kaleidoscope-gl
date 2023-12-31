#include "ShaderProgram.hpp"
#include <iostream>

ShaderProgram::ShaderProgram() {}

ShaderProgram::ShaderProgram(std::string vertexShader,
                             std::string fragmentShader) {
  this->vertexShader = compileShader(vertexShader, GL_VERTEX_SHADER);
  this->fragmentShader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
  this->program = compileProgram();
}

ShaderProgram::ShaderProgram(std::string vertexShader,
                             std::string fragmentShader,
                             UniformData uniformData) {
  this->vertexShader = compileShader(vertexShader, GL_VERTEX_SHADER);
  this->fragmentShader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
  this->program = compileProgram();
  this->uniformData = uniformData;
}

GLuint ShaderProgram::compileShader(std::string shaderSource,
                                    GLenum shaderType) {
  // Create and compile shader
  GLuint shader = glCreateShader(shaderType);
  const char *source = shaderSource.c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  // Check for errors
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return shader;
}

GLuint ShaderProgram::compileProgram() {
  // Create a program
  GLuint program = glCreateProgram();
  // Attach shaders to program
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  // Link program
  glLinkProgram(program);
  // Check for errors
  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

void ShaderProgram::setUniformData(UniformData uniformData) {
  this->uniformData = uniformData;
}
void ShaderProgram::sendUniformData() {
  glUniform2f(glGetUniformLocation(program, "u_resolution"),
              uniformData.resolution[0], uniformData.resolution[1]);
  glUniform2f(glGetUniformLocation(program, "u_center"), uniformData.center[0],
              uniformData.center[1]);
  glUniform1f(glGetUniformLocation(program, "u_zoom"), uniformData.zoom);
  glUniform1f(glGetUniformLocation(program, "u_iterations"),
              uniformData.iterations);
  glUniform1f(glGetUniformLocation(program, "u_power"), uniformData.power);
  glUniform1i(glGetUniformLocation(program, "u_time"), uniformData.time);
  glUniform1f(glGetUniformLocation(program, "u_gamma"), uniformData.gamma);

  glUniform3f(glGetUniformLocation(program, "input1"), uniformData.input1[0], uniformData.input1[1], uniformData.input1[2]);
  glUniform3f(glGetUniformLocation(program, "input2"), uniformData.input2[0], uniformData.input2[1], uniformData.input2[2]);
  glUniform3f(glGetUniformLocation(program, "input3"), uniformData.input3[0], uniformData.input3[1], uniformData.input3[2]);
  glUniform3f(glGetUniformLocation(program, "input4"), uniformData.input4[0], uniformData.input4[1], uniformData.input4[2]);
  // repeat abov till input4
}
void ShaderProgram::use() { glUseProgram(program); }

void ShaderProgram::unuse() { glUseProgram(0); }