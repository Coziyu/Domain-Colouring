#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/**
 * @brief Provides useful methods to streamline shader compiling and passing in uniforms 
 * 
 */
class Shader
{
public:
    /**
     * @brief Holds the ID from @b glCreateProgram()
     */
    unsigned int ID;
    /**
     * @brief Holds the currently loaded fragment shader path
     */
    const char* fragmentPath;
    /**
     * @brief Holds the currently loaded vertex shader path
     */
    const char* vertexPath;

    /**
     * @brief Construct and compile new Shader using external files containing shader code.
     * @param vertexPath relative path (to the executable) to vertex shader file 
     * @param fragmentPath relative path (to the executable) to fragment shader file
     */
    Shader(const char* vertexPath, const char* fragmentPath);
    /**
     * @brief Runs @b glUseProgram() with the @b Shader::ID as argument 
     */
    void use();
    /**
     * @brief Pass in a bool value as a uniform to a shader 
     * @param name identifier of the uniform inside shader code
     * @param value value to pass into the shader
     */
    void setBool(const std::string &name, bool value) const;  
    /**
     * @brief Pass in an int value as a uniform to a shader 
     * @param name identifier of the uniform inside shader code
     * @param value value to pass into the shader
     */
    void setInt(const std::string &name, int value) const;
    /**
     * @brief Pass in a Float value as a uniform to a shader 
     * @param name identifier of the uniform inside shader code
     * @param value value to pass into the shader
     */
    void setFloat(const std::string &name, float value) const;
    /**
     * @brief Pass in a mat4 value as a uniform to a shader 
     * @param name identifier of the uniform inside shader code
     * @param value value to pass into the shader
     */
    void setMat(const std::string &name, glm::mat4 value) const; //TODO: Overload with other mat types when needed
    /**
     * @brief Compiles and loads new shader using external files containing shader code.
     * This will store the new relative file paths into @b Shader::vertexPath and @b Shader::fragmentPath.
     **Shader will not reload if there is a compile error.
     *! Note that this will generate a new ID for the shader program and all uniforms needs to be set again. 
     * @param vertexPath relative path (to the executable) to vertex shader file
     * @param fragmentPath relative path (to the executable) to fragment shader file 
     */
    void reload(const char* vertexPath, const char* fragmentPath);
    /**
     * @brief Compiles and loads new shader using the currently stored shader file paths.
     */
    void reload();
};
  
