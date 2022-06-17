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
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS){
        const float def_vertices[12] = {
            0.5f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f,
            -0.5f,  0.5f,  0.0f
        };
        for(int i = 0; i < 9; ++i){
            vertices[i] = def_vertices[i];
        }
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        vertices[1] += 0.01;
        vertices[4] += 0.01;
        vertices[7] += 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        vertices[1] -= 0.01;
        vertices[4] -= 0.01;
        vertices[7] -= 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vertices[0] += 0.01;
        vertices[3] += 0.01;
        vertices[6] += 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vertices[0] -= 0.01;
        vertices[3] -= 0.01;
        vertices[6] -= 0.01;
    }
}
void reloadShader(GLFWwindow* window, Shader* shader){
    shader->reload("./shaders/vertexShader1.vert","./shaders/fragmentShader1.frag");
}
