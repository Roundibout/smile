/*
File:
    shader_manager.cpp
Authors:
    Lucas
Purpose:
    Implementation of the ShaderManager class, responsible for handling shaders per Renderer
License:
    MIT (see LICENSE file)
*/

#include "shader_manager.hpp"

// Function for reading a file into a string
static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool ShaderManagerGL::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSrc = readFile(vertexPath);
    std::string fragmentSrc = readFile(fragmentPath);

    if (vertexSrc.empty() || fragmentSrc.empty()) {
        std::cerr << "Shader source is empty" << std::endl;
        return false;
    }

    // Turn the fancy strings into C-style strings for use with GL
    const char* vSrc = vertexSrc.c_str();
    const char* fSrc = fragmentSrc.c_str();

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, log);
        std::cerr << "Vertex shader compilation failed: " << log << std::endl;
        glDeleteShader(vertexShader);
        return false;
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
        std::cerr << "Fragment shader compilation failed: " << log << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Shader program linking failed: " << log << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return false;
    }

    // Cleanup shaders (we only need the program now)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shaders[name] = program;
    return true;
}

void ShaderManagerGL::useShader(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        glUseProgram(it->second);
    } else {
        std::cerr << "Shader not found: " << name << std::endl;
    }
}

void ShaderManagerGL::setUniformMat4(const std::string& shaderName, const std::string& uniform, const float* mat) {
    GLuint program = shaders[shaderName];
    glUseProgram(program);
    GLint loc = glGetUniformLocation(program, uniform.c_str());
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
    }
}

ShaderManagerGL::~ShaderManagerGL() {
    for (auto& [_, program] : shaders) {
        glDeleteProgram(program);
    }
    shaders.clear();
}