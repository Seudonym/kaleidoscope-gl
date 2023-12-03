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
  // Specific uniforms
  real input1[3];
};

class ShaderProgram;

std::string readFile(std::string filename);