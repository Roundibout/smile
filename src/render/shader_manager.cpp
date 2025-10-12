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
        Logger::error("Failed to open shader file: " + path);
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
        Logger::error("Shader source is empty");
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
        Logger::error("Vertex shader compilation failed: " + std::string(log));
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
        Logger::error("Fragment shader compilation failed: " + std::string(log));
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
        Logger::error("Shader program linking failed: " + std::string(log));
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
        Logger::error("Shader not found: " + name);
    }
}

GLint ShaderManagerGL::getUniformLocation(const std::string& shaderName, const std::string& uniform) {
    auto it = shaders.find(shaderName);
    if (it == shaders.end()) {
        Logger::error("Shader not found: " + shaderName);
        return -1;
    }
    
    GLint location = glGetUniformLocation(it->second, uniform.c_str());
    if (location == -1) {
        Logger::error("Uniform not found: " + uniform + " in shader: " + shaderName);
    }
    
    return location;
}

// Integer uniforms
void ShaderManagerGL::setUniformInt(const std::string& shaderName, const std::string& uniform, int value) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void ShaderManagerGL::setUniformInt2(const std::string& shaderName, const std::string& uniform, int x, int y) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform2i(location, x, y);
    }
}

void ShaderManagerGL::setUniformInt3(const std::string& shaderName, const std::string& uniform, int x, int y, int z) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform3i(location, x, y, z);
    }
}

void ShaderManagerGL::setUniformInt4(const std::string& shaderName, const std::string& uniform, int x, int y, int z, int w) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform4i(location, x, y, z, w);
    }
}

// Float uniforms
void ShaderManagerGL::setUniformFloat(const std::string& shaderName, const std::string& uniform, float value) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void ShaderManagerGL::setUniformFloat2(const std::string& shaderName, const std::string& uniform, float x, float y) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform2f(location, x, y);
    }
}

void ShaderManagerGL::setUniformFloat3(const std::string& shaderName, const std::string& uniform, float x, float y, float z) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform3f(location, x, y, z);
    }
}

void ShaderManagerGL::setUniformFloat4(const std::string& shaderName, const std::string& uniform, float x, float y, float z, float w) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

// Vector uniforms (array versions)
void ShaderManagerGL::setUniformFloat2v(const std::string& shaderName, const std::string& uniform, const float* values) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform2fv(location, 1, values);
    }
}

void ShaderManagerGL::setUniformFloat3v(const std::string& shaderName, const std::string& uniform, const float* values) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform3fv(location, 1, values);
    }
}

void ShaderManagerGL::setUniformFloat4v(const std::string& shaderName, const std::string& uniform, const float* values) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniform4fv(location, 1, values);
    }
}

// Matrix uniforms
void ShaderManagerGL::setUniformMat2(const std::string& shaderName, const std::string& uniform, const float* mat) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniformMatrix2fv(location, 1, GL_FALSE, mat);
    }
}

void ShaderManagerGL::setUniformMat3(const std::string& shaderName, const std::string& uniform, const float* mat) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, GL_FALSE, mat);
    }
}

void ShaderManagerGL::setUniformMat4(const std::string& shaderName, const std::string& uniform, const float* mat) {
    GLint location = getUniformLocation(shaderName, uniform);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, mat);
    }
}

ShaderManagerGL::~ShaderManagerGL() {
    for (auto& [_, program] : shaders) {
        glDeleteProgram(program);
    }
    shaders.clear();
}