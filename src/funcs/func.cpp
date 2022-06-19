#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "headers/data.h"
#include "headers/shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
void reloadShader(GLFWwindow* window, Shader* shader){
    shader->reload("./shaders/vertexShader1.vert","./shaders/fragmentShader1.frag");
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    mouse_scroll_y_offset = yoffset/10;
    std::cout << mouse_scroll_y_offset << "\n";
}