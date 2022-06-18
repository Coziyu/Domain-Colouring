#include <iostream>
#include <fstream>
#include <streambuf>
#include <cmath>
#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "headers/func.h"
#include "headers/data.h"
#include "headers/shader.h"
#include "headers/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

unsigned const int WIN_WIDTH = 800;
unsigned const int WIN_HEIGHT = 600; 

//! FIND OUT WHY GRAPH VECTOR RENDER PROPERLY WHEN USING STL VECTOR

struct Point {
  float m_x;
  float m_y;
  Point(float x, float y) : m_x{x}, m_y{y}{}
};

int main(){

    std::vector<Point> graph;
    graph.reserve(2000);
    for(int i = 0; i < 2000; i++) {
        float x = (i - 1000.0f) / 100.0f;
        graph.push_back(Point(x,(sin(x * 10.0f) / (1.0f + x * x))));
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"Learning OpenGL",NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create OpenGL window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialze GLAD\n";
        return -1;
    }

    Shader myShader("./shaders/vertexShader1.vert","./shaders/fragmentShader1.frag");

    unsigned int VAO; 
    unsigned int VBO_Vertex;
    //unsigned int EBO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO_Vertex);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); //<--- Start of VAO Bind
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex); 
    glBufferData(GL_ARRAY_BUFFER, graph.size() * sizeof(Point), &graph[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); //<--- End of VAO Bind

    myShader.use();

    bool button_r_pressed = false; //For hot shader reloading


    while(!glfwWindowShouldClose(window)){        
        // inputA
        processInput(window);
        if((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && !button_r_pressed){
            reloadShader(window, &myShader);
            myShader.use();
            button_r_pressed = true;
        }
        if(glfwGetKey(window,GLFW_KEY_R) == GLFW_RELEASE){
            button_r_pressed = false;
        }
        //rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        myShader.use();        
        glBindVertexArray(VAO);

        glDrawArrays(GL_LINE_STRIP, 0, graph.size());
        //check and call events and swap buffers

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0; 
}