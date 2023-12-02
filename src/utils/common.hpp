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

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, UniformData &uniformData);
std::string readFile(std::string filename);