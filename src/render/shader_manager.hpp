/*
File:
    shader_manager.hpp
Authors:
    Lucas
Purpose:
    Definition of the ShaderManager class, responsible for handling shaders per Renderer
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <glad/gl.h>

#include <util/logger.hpp>

class ShaderManagerGL {
private:
    std::unordered_map<std::string, GLuint> shaders;

    // Helper function to get uniform location
    GLint getUniformLocation(const std::string& shaderName, const std::string& uniform);
public:
    ShaderManagerGL() = default;
    ~ShaderManagerGL();

    // Load and compile a shader program from files
    bool loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    // Use a shader program
    void useShader(const std::string& name);

    // Set uniforms - integers
    void setUniformInt(const std::string& shaderName, const std::string& uniform, int value);
    void setUniformInt2(const std::string& shaderName, const std::string& uniform, int x, int y);
    void setUniformInt3(const std::string& shaderName, const std::string& uniform, int x, int y, int z);
    void setUniformInt4(const std::string& shaderName, const std::string& uniform, int x, int y, int z, int w);

    // Set uniforms - floats
    void setUniformFloat(const std::string& shaderName, const std::string& uniform, float value);
    void setUniformFloat2(const std::string& shaderName, const std::string& uniform, float x, float y);
    void setUniformFloat3(const std::string& shaderName, const std::string& uniform, float x, float y, float z);
    void setUniformFloat4(const std::string& shaderName, const std::string& uniform, float x, float y, float z, float w);

    // Set uniforms - vectors (array versions)
    void setUniformFloat2v(const std::string& shaderName, const std::string& uniform, const float* values);
    void setUniformFloat3v(const std::string& shaderName, const std::string& uniform, const float* values);
    void setUniformFloat4v(const std::string& shaderName, const std::string& uniform, const float* values);

    // Set uniforms - matrices
    void setUniformMat2(const std::string& shaderName, const std::string& uniform, const float* mat);
    void setUniformMat3(const std::string& shaderName, const std::string& uniform, const float* mat);
    void setUniformMat4(const std::string& shaderName, const std::string& uniform, const float* mat);

    // Delete all shaders
    void clear();
};