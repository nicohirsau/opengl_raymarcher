#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <ctime>

#include <Windows.h>

#include "Mantaray/Graphics/Shader.h"
#include "Mantaray/Core/FileSystem.h"
#include "Mantaray/Core/Logger.h"
#include "Mantaray/Core/Math.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime);

unsigned int SCR_WIDTH = 750;
unsigned int SCR_HEIGHT = 750;
MR::Vector2d cursor_pos;
MR::Logger logger("Application");

int main()
{
    logger.Log("Initializing GLFW context...");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        logger.Log("Failed to create GLFW window", MR::Logger::LOG_ERROR);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger.Log("Failed to initialize GLAD", MR::Logger::LOG_ERROR);
        return -1;
    }

    logger.Log("Compiling shaders...");
    // compile shaders
    std::string vertexShaderContent;
    MR::FileSystem::readFile("Content/vertex.vert", vertexShaderContent);
    const char* vertexSrc = vertexShaderContent.c_str();

    unsigned int vertexShader;
    vertexShader = MR::Shader::compileShader(MR::Shader::VERTEX_SHADER, vertexSrc);

    std::string fragmentShaderContent;
    MR::FileSystem::readFile("Content/fragment.frag", fragmentShaderContent);
    const char* fragmentSrc = fragmentShaderContent.c_str();
    
    unsigned int fragmentShader;
    fragmentShader = MR::Shader::compileShader(MR::Shader::FRAGMENT_SHADER, fragmentSrc);

    logger.Log("Linking shaders...");
    // link shaders
    unsigned int shaderProgram = MR::Shader::linkShader(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    int u_time = glGetUniformLocation(shaderProgram, "u_time");
    glUniform1f(u_time, 0.0f);
    int u_mPos = glGetUniformLocation(shaderProgram, "u_mPos");
    glUniform2f(u_mPos, 0.0f, 0.0f);

    logger.Log("Creating Mesh...");
    // Create triangles
    float vertices[] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f
    };
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    logger.Log("Loading Texture...");
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    std::string image_path = MR::FileSystem::getWorkingDirectory() + "Content/earth.png";
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        logger.Log("Image from " + image_path + " could not be loaded", MR::Logger::LOG_ERROR);
        return -1;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    float elapsed_time = 0.0f;
    float delta_time = 0.0f;

    logger.Log("Starting Loop...");
    while (!glfwWindowShouldClose(window))
    {
        std::clock_t beginFrame = clock();
        processInput(window, delta_time);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float) / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::clock_t endFrame = clock();
        delta_time = (endFrame - beginFrame) / 1000.0f;
        elapsed_time += delta_time;
        glUniform1f(u_time, elapsed_time);
        glUniform2f(
            u_mPos, 
            (float)((cursor_pos.x / SCR_WIDTH - 0.5) * 2.0), 
            -(float)((cursor_pos.y / SCR_HEIGHT - 0.5) * 2.0)
        );
    }

    logger.Log("Shutting down application...");
    // delete opengl objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwGetCursorPos(window, &cursor_pos.x, &cursor_pos.y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

    glViewport(0, 0, width, height);
}
