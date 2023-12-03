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

unsigned int width = 1280;
unsigned int height = 720;

ShaderProgram grayscale, smooth;

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
  // Initialize GLEW
  if (glewInit() != GLEW_OK)
    std::cerr << "Failed to initialize GLEW." << std::endl;

  // Create and compile shader program
  grayscale = ShaderProgram(readFile("./shaders/quad.vs.glsl"),
                            readFile("./shaders/greyscale.fs.glsl"));

  smooth = ShaderProgram(readFile("./shaders/quad.vs.glsl"),
                         readFile("./shaders/smooth.fs.glsl"));

  ShaderProgram shaderProgram = grayscale;
  // Create and populate a VBO, and setup VAO
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

  // Main loop
  glBindVertexArray(VAO);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  while (!glfwWindowShouldClose(window)) {
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

    // ImGui rendering
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Controls");
    ImGui::ColorEdit3("input1", uniformData.input1);
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
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    uniformData.zoom *= 1.01;
  else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    uniformData.zoom /= 1.01;
  if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
    uniformData.iterations += 1.0;
  else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    uniformData.iterations -= 1.0;
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    shaderProgram = grayscale;
  else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    shaderProgram = smooth;

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    glfwGetCursorPos(window, &mouseX, &mouseY);
    uniformData.center[0] -=
        2.0 * (mouseX - prevMouseX) / width * uniformData.zoom;
    uniformData.center[1] +=
        2.0 * (mouseY - prevMouseY) / height * uniformData.zoom;
  }
  glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
}

std::string readFile(std::string filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}