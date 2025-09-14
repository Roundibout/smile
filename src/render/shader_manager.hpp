#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/gl.h>

class ShaderManagerGL {
private:
    std::unordered_map<std::string, GLuint> shaders;
public:
    ShaderManagerGL() = default;
    ~ShaderManagerGL();

    // Load and compile a shader program from files
    bool loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    // Use a shader program
    void useShader(const std::string& name);

    // Set uniforms
    void setUniformMat4(const std::string& shaderName, const std::string& uniform, const float* mat);

    // Delete all shaders
    void clear();
};