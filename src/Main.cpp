// External includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Internal includes
#include <iostream>
// Local includes
#include "utils/ShaderProgram.hpp"
#include "utils/common.hpp"

real vertices[] = {
    -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0,
};

int main() {
  // Window dimensions
  const unsigned int width = 640;
  const unsigned int height = 480;
  // Initialize GLFW
  if (!glfwInit())
    std::cerr << "Failed to init GLFW." << std::endl;
  // Set GLFW window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create and setup GLFW window
  GLFWwindow *window = glfwCreateWindow(width, height, "Test", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  // Initialize GLEW
  if (glewInit() != GLEW_OK)
    std::cerr << "Failed to initialize GLEW." << std::endl;

  // Create and compile shader program
  std::string vertexShaderSource = readFile("./shaders/quad.vs.glsl");
  std::string fragmentShaderSource = readFile("./shaders/mandelbrot.fs.glsl");
  ShaderProgram shaderProgram(vertexShaderSource, fragmentShaderSource);
  GLuint program = shaderProgram.compileProgram();

  // Create and populate a VBO, and setup VAO
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set up locations
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(real), (void *)0);
  glEnableVertexAttribArray(0);
  // Set up uniforms
  UniformData uniformData;
  uniformData.resolution[0] = width;
  uniformData.resolution[1] = height;
  uniformData.center[0] = 0.0;
  uniformData.center[1] = 0.0;
  uniformData.zoom = 1.0;
  uniformData.iterations = 100.0;
  // Send uniform data to GPU
  glUseProgram(program);
  glUniform2f(glGetUniformLocation(program, "u_resolution"),
              uniformData.resolution[0], uniformData.resolution[1]);
  glUniform2f(glGetUniformLocation(program, "u_center"), uniformData.center[0],
              uniformData.center[1]);
  glUniform1f(glGetUniformLocation(program, "u_zoom"), uniformData.zoom);
  glUniform1f(glGetUniformLocation(program, "u_iterations"),
              uniformData.iterations);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    processInput(window, uniformData);
    // Send updated uniforms to GPU
    glUniform2f(glGetUniformLocation(program, "u_center"),
                uniformData.center[0], uniformData.center[1]);
    glUniform1f(glGetUniformLocation(program, "u_zoom"), uniformData.zoom);
    glUniform1f(glGetUniformLocation(program, "u_iterations"),
                uniformData.iterations);

    // Rendering commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    // Check and call events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}