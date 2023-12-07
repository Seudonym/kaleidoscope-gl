#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif
typedef float real;

struct UniformData {
  real resolution[2];
  real center[2];
  real zoom;
  real iterations;
  real power;
  int time;
  real gamma;
  // Specific uniforms
  real input1[3], input2[3], input3[3], input4[3];
};

std::string readFile(std::string filename);