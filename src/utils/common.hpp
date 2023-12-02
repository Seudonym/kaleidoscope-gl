#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

struct UniformData {
  real resolution[2];
  real center[2];
  real zoom;
  real iterations;
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, UniformData &uniformData) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    uniformData.center[1] += 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    uniformData.center[1] -= 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    uniformData.center[0] -= 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    uniformData.center[0] += 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    uniformData.zoom *= 1.01;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    uniformData.zoom /= 1.01;
  if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
    uniformData.iterations += 1.0;
  if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    uniformData.iterations -= 1.0;
}

// Write a program to read and return content from a file as a string
std::string readFile(std::string filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}