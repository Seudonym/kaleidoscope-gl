#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "utils/ShaderProgram.hpp"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, UniformData &uniformData,
                  ShaderProgram &shaderProgram);

real vertices[] = {
    -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0,
};
double prevMouseX, prevMouseY;
double mouseX, mouseY;

unsigned int width = 2560;
unsigned int height = 1440;

ShaderProgram base, blend;

bool record = false;

int main() {
  // Window dimensions
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
  glEnable(GL_MULTISAMPLE);
  // Initialize GLEW
  if (glewInit() != GLEW_OK)
    std::cerr << "Failed to initialize GLEW." << std::endl;

  // Create and compile shader program
  base = ShaderProgram(readFile("./shaders/quad.vs.glsl"),
                       readFile("./shaders/base.fs.glsl"));
  blend = ShaderProgram(readFile("./shaders/quad.vs.glsl"),
                        readFile("./shaders/blend.fs.glsl"));

  ShaderProgram shaderProgram = blend;
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
  uniformData.power = 2.0;
  uniformData.time = 0;
  uniformData.gamma = 2.2;

  // Main loop
  glBindVertexArray(VAO);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  int counter = 0;
  while (!glfwWindowShouldClose(window)) {
    uniformData.time++;
    processInput(window, uniformData, shaderProgram);
    // Uniforms
    shaderProgram.use();
    uniformData.resolution[0] = width;
    uniformData.resolution[1] = height;
    shaderProgram.setUniformData(uniformData);
    shaderProgram.sendUniformData();

    // Rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    // Hacky record
    if (record) {
      std::cout << "recording";
      unsigned char *data = new unsigned char[3 * width * height];
      glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
      std::ofstream file(std::string("video/frame") +
                         std::to_string(counter++) + std::string(".ppm"));
      file << "P3\n" << width << " " << height << "\n255\n";
      for (int i = 0; i < width * height; i++) {
        file << (int)data[3 * i] << " " << (int)data[3 * i + 1] << " "
             << (int)data[3 * i + 2] << "\n";
      }

      file.close();
      delete[] data;
    }

    // ImGui rendering
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Controls");
    ImGui::InputFloat("Iterations", &uniformData.iterations);
    ImGui::SliderFloat("Power", &uniformData.power, 0.0, 20.0);
    ImGui::SliderFloat("Gamma", &uniformData.gamma, 0.0, 4.0);
    ImGui::ColorEdit3("Input 1", uniformData.input1);
    ImGui::ColorEdit3("Input 2", uniformData.input2);
    ImGui::ColorEdit3("Input 3", uniformData.input3);
    ImGui::ColorEdit3("Input 4", uniformData.input4);
    if (ImGui::Button("Screenshot")) {
      unsigned char *data = new unsigned char[3 * width * height];
      glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
      std::ofstream file("screenshot.ppm");
      file << "P3\n" << width << " " << height << "\n255\n";
      for (int i = 0; i < width * height; i++) {
        file << (int)data[3 * i] << " " << (int)data[3 * i + 1] << " "
             << (int)data[3 * i + 2] << "\n";
      }
      file.close();
      delete[] data;
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  ::width = width;
  ::height = height;
}

void processInput(GLFWwindow *window, UniformData &uniformData,
                  ShaderProgram &shaderProgram) {
  auto &io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    uniformData.center[1] += 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    uniformData.center[1] -= 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    uniformData.center[0] -= 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    uniformData.center[0] += 0.01 * uniformData.zoom;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    uniformData.zoom *= 1.01;
    uniformData.iterations -= 0.5;
  } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    uniformData.zoom /= 1.01;
    uniformData.iterations += 0.5;
  }
  if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
    uniformData.iterations += 1.0;
  else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    uniformData.iterations -= 1.0;

  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    uniformData.power += 0.1;
  else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    uniformData.power -= 0.1;

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    glfwGetCursorPos(window, &mouseX, &mouseY);
    uniformData.center[0] -=
        4.0 * (mouseX - prevMouseX) / width * uniformData.zoom;
    uniformData.center[1] +=
        4.0 * (mouseY - prevMouseY) / height * uniformData.zoom;
  }
  glfwGetCursorPos(window, &prevMouseX, &prevMouseY);

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    record = true;
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
    record = false;
}

std::string readFile(std::string filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
