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
unsigned const int WIN_HEIGHT = 800; 

//* FRAGMENT INTERPOLATION ACTUALLY PASSES INTERPOLATED COORDINATED INTO THE FRAGMENT SHADER!!!!!!!
//TODO: Implement mouse drag translation
//TODO: Implement equation parser into fragment shader?
//TODO: Implement zoom aware WASD translation
//TODO: Add contourmode: both phase and modulus
struct Point {
    float m_x;
    float m_y;
    Point(float x, float y) : m_x{x}, m_y{y}{}
};

std::vector<unsigned int> generate_grid_indices(unsigned int size){ //Generate indices for triangulation
    std::vector<unsigned int> r;
    for(unsigned int i = 0; i <= size - 2; i++){
        for(unsigned int j = 1; j <= size - 1; j++){
            r.push_back(i*size + j - 1);
            r.push_back(i*size + j - 1 + 1);
            r.push_back(i*size + j - 1 + size);
            r.push_back(i*size + j - 1 + size);
            r.push_back(i*size + j - 1 + size + 1);
            r.push_back(i*size + j - 1 + 1);
        }
    }
    return r;
}

enum contourMode{
    disable = 0,
    modulus = 1,
    phase = 2
};

int main(){

    std::vector<Point> grid;
    int span = 3; //* NEEDS TO BE ODD
    float density = 0.1f;
    if((span + 1) % 2){
        std::cout << "WARNING: span NEEDS TO BE ODD." << std::endl;
    }
    for(int y = -(span - 1)/2; y <= (span - 1)/2; y++){
        for(int x = -(span - 1)/2; x <= (span - 1)/2; x++){
            // float norm_x = (float)x / ((float)(span - 1.0f)/2.0f); 
            // float norm_y = (float)y / ((float)(span - 1.0f)/2.0f);
            float norm_x = (float)x / density; 
            float norm_y = (float)y / density;
            grid.push_back(Point(norm_x, norm_y));
        }
    }
    std::vector<unsigned int> indices = generate_grid_indices(span);
    // for(auto i : indices){
    //     std::cout << i << "\n";
    // }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"Domain Colouring",NULL, NULL);
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
    glfwSetScrollCallback(window, scroll_callback);

    
    Shader myShader("./shaders/vertexShader1.vert","./shaders/fragmentShader1.frag");

    unsigned int VAO; 
    unsigned int VBO_Vertex;
    unsigned int EBO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO_Vertex);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); //<--- Start of VAO Bind
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex); 
    glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(Point), &grid[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    

    glBindVertexArray(0); //<--- End of VAO Bind

    myShader.use();

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);


    int setContourMode = contourMode::disable;
    float blend = 1;
    myShader.setInt("setContourMode", setContourMode);
    myShader.setFloat("blend", blend);
    myShader.setMat("model", model);
    myShader.setMat("view", view);

    bool button_r_pressed = false; //For hot shader reloading
    bool button_f_pressed = false; //To toggle polygonmode
    bool button_horizontal_pressed = false; //Change blend
    bool button_space_pressed = false;
    while(!glfwWindowShouldClose(window)){        
        // inputA
        processInput(window);
        {//Inputs
            if((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && !button_r_pressed){
                reloadShader(window, &myShader);
                myShader.use();
                myShader.setInt("setContourMode", setContourMode);
                myShader.setFloat("blend", blend);
                myShader.setMat("model", model);
                myShader.setMat("view", view);
                button_r_pressed = true;
            }
            if(glfwGetKey(window,GLFW_KEY_R) == GLFW_RELEASE){
                button_r_pressed = false;
            }
            //
            if(glfwGetKey(window,GLFW_KEY_F) == GLFW_PRESS && !button_f_pressed){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                button_f_pressed = true;
            }
            if(glfwGetKey(window,GLFW_KEY_F) == GLFW_RELEASE){
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                button_f_pressed = false;
            }
            //
            if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS && !button_space_pressed){
                switch (setContourMode){
                    case contourMode::disable:
                        setContourMode = contourMode::modulus;
                        break;
                    case contourMode::modulus:
                        setContourMode = contourMode::phase;
                        break;
                    case contourMode::phase:
                        setContourMode = contourMode::disable;
                        break;
                }
                button_space_pressed = true;
                myShader.setInt("setContourMode", setContourMode);
            }
            if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_RELEASE){
                button_space_pressed = false;
            }
            //
            if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !button_horizontal_pressed){
                blend += 1.00f;
                myShader.setFloat("blend", blend);
                button_horizontal_pressed = true;
            }
            if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !button_horizontal_pressed){
                blend -= 1.00f;
                myShader.setFloat("blend", blend);
                button_horizontal_pressed = true;
            }
            if((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)){
                button_horizontal_pressed = false;
            }
            if(glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS){
                blend = 1;
                myShader.setFloat("blend", blend);
            } 
            //
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
                view = glm::scale(view, glm::vec3(1.025f, 1.025f, 0.0f));
                myShader.setMat("view", view);
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
                view = glm::scale(view, glm::vec3(0.975f, 0.975f, 0.0f));
                myShader.setMat("view", view);
            }
            if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
                view = glm::mat4(1.0f);
                myShader.setMat("view", view);
            }

            if(mouse_scroll_y_offset != 0){
                view = glm::scale(view, glm::vec3(1.0f + mouse_scroll_y_offset, 1.0f + mouse_scroll_y_offset, 0.0f));
                myShader.setMat("view", view);
                mouse_scroll_y_offset = 0;
            }
            //
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                model = glm::translate(model, glm::vec3(0.0f,-0.015f,0.0f));
                myShader.setMat("model", model);
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                model = glm::translate(model, glm::vec3(0.0f,0.015f,0.0f));
                myShader.setMat("model", model);
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                model = glm::translate(model, glm::vec3(0.015f,0.0f,0.0f));
                myShader.setMat("model", model);
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                model = glm::translate(model, glm::vec3(-0.015f,0.0f,0.0f));
                myShader.setMat("model", model);
            }
            if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
                model = glm::mat4(1.0f);
                myShader.setMat("model", model);
            }
        }

        //rendering commands
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
                
        myShader.use();        
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        //check and call events and swap buffers

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0; 
}