#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mantaray/Core/FileSystem.h"
#include "Mantaray/Core/Logger.h"
#include "Mantaray/Graphics/Shader.h"

using namespace MR;

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    m_VertexShaderID = compileShader(VERTEX_SHADER, vertexShaderSource);
    m_FragmentShaderID = compileShader(FRAGMENT_SHADER, fragmentShaderSource);
    m_ShaderProgramID = linkShader(m_VertexShaderID, m_FragmentShaderID);
}

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    std::string vertexShaderContent;
    FileSystem::readFile(vertexShaderPath, vertexShaderContent);
    const char* vertexShaderSource = vertexShaderContent.c_str();
    
    std::string fragmentShaderContent;
    FileSystem::readFile(fragmentShaderPath, fragmentShaderContent);
    const char* fragmentShaderSource = fragmentShaderContent.c_str();

    m_VertexShaderID = compileShader(VERTEX_SHADER, vertexShaderSource);
    m_FragmentShaderID = compileShader(FRAGMENT_SHADER, fragmentShaderSource);
    m_ShaderProgramID = linkShader(m_VertexShaderID, m_FragmentShaderID);
}

Shader::~Shader() {
    glDeleteShader(m_VertexShaderID);
    glDeleteShader(m_FragmentShaderID);
    glDeleteProgram(m_ShaderProgramID);
}

void Shader::activate() {
    glUseProgram(m_ShaderProgramID);
}

void Shader::setUniformFloat(std::string uniformName, float value) {
    int uniformLocation = getUniformLocation(uniformName);
    glUniform1f(uniformLocation, value);
}

void Shader::setUniformVector2f(std::string uniformName, Vector2f value) {
    int uniformLocation = getUniformLocation(uniformName);
    glUniform2f(uniformLocation, value.x, value.y);    
}

void Shader::setUniformVector3f(std::string uniformName, Vector3f value) {
    int uniformLocation = getUniformLocation(uniformName);
    glUniform3f(uniformLocation, value.x, value.y, value.z);    
}

int Shader::getUniformLocation(std::string uniformName) {
    int uniformLocation = -1;

    std::unordered_map<std::string, int>::iterator it;
    it = m_Uniforms.find(uniformName);

    if (it == m_Uniforms.end()) {
        uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        if (uniformLocation == -1) {
            Logger::Log("Shader", "Cannot find uniform location for: " + uniformName, Logger::LOG_WARNING);
        }
        m_Uniforms[uniformName] = uniformLocation;
    } else {
        uniformLocation = it->second;
    }
    return uniformLocation;
}

unsigned int Shader::compileShader(Shader::ShaderType shaderType, const char* source) {
    unsigned int shaderID;

    switch (shaderType)
    {
    case Shader::VERTEX_SHADER:
        shaderID = glCreateShader(GL_VERTEX_SHADER);
        break;
    case Shader::FRAGMENT_SHADER:
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    
    default:
        return -1;
    }

    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::string s_ShaderType = (shaderType == VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::string message = s_ShaderType + "::COMPILING_SHADER_FAILED\n";
        message.append(infoLog);
        Logger::Log("ShaderCompiler", message, Logger::LOG_ERROR);
        return -1;
    }

    return shaderID;
}

unsigned int MR::Shader::linkShader(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    
    int  success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::string message = "LINKING_SHADER_FAILED\n";
        message.append(infoLog);
        Logger::Log("ShaderLinker", message, Logger::LOG_ERROR);
        return -1;
    }
    
    return programID;
}
