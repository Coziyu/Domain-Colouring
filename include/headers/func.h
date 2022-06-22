#pragma once
#include "headers/shader.h"
#include <GLFW/glfw3.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/**
 * @brief Recompile the program of @b target_shader using it's existing shader paths.
 * @details This will recompile the program of @b target_shader using the prexisting @b Shader::vertexPath and @b Shader::fragmentPath .
 * To load different shaders into @b target_shader 's program, use <b>reload(const char* vertexPath, const char* fragmentPath)</b> instead.
 * @param window @b GLFWwindow pointer to currently focused window.
 * @param target_shader @b Shader to reload. 
 */
void reloadShader(GLFWwindow* window, Shader* target_shader);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);


