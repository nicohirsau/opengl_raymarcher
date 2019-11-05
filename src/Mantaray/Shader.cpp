#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mantaray/Graphics/Shader.h"
#include "Mantaray/Core/Logger.h"

using namespace MR;

unsigned int Shader::compileShader(Shader::ShaderType st, const char* source) {
    unsigned int shaderID;
    std::string shaderType;

    switch (st)
    {
    case Shader::VERTEX_SHADER:
        shaderType = "VERTEX";
        shaderID = glCreateShader(GL_VERTEX_SHADER);
        break;
    case Shader::FRAGMENT_SHADER:
        shaderType = "FRAGMENT";
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
        std::string message = shaderType + "::COMPILING_SHADER_FAILED\n";
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
