#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "headers/data.h"
#include "headers/shader.h"
#include "headers/func.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
void reloadShader(GLFWwindow* window, Shader* shader){
    shader->reload();
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    mouse_scroll_y_offset = yoffset/10;
    std::cout << (camera_zoom * (1.0f + mouse_scroll_y_offset)) << "\n";
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    // if((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)){
    //     glfwGetCursorPos(window, &mouse_left_current_x_pos, &mouse_left_current_y_pos);
    //     if(!mouse_left_pressed){
    //         glfwGetCursorPos(window, &mouse_left_press_x_pos, &mouse_left_press_y_pos);
    //     }
    //     std::cout << "left click detected" << "\n";
    //     std::cout << mouse_left_current_x_pos <<" "<< mouse_left_current_y_pos << "\n";

    //     mouse_left_pressed = true;
    // }
    // if((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_RELEASE) && mouse_left_pressed){
    //     std::cout << "left click release" << "\n";
    //     mouse_left_pressed = false;
    // }
}