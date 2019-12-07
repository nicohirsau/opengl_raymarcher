#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <algorithm>
#include <vector>

#include <Windows.h>

#include "Mantaray/Graphics/Shader.h"
#include "Mantaray/Core/FileSystem.h"
#include "Mantaray/Core/Logger.h"
#include "Mantaray/Core/Vector.h"
#include "Mantaray/Core/Math.h"
#include "Mantaray/Core/Mesh.h"
#include "Mantaray/Core/InputManager.h"
#include "Mantaray/Core/KeyCodes.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;
MR::Vector2d cursor_pos;
MR::Vector2d lcursor_pos;
MR::Vector3f player_pos;
MR::Vector2f player_rot;
float player_speed = 1.0f;
MR::Logger logger("Application");

int main()
{
    logger.Log("Initializing GLFW context...");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    MR::InputManager::setWindowHandle(window);
    MR::InputManager::addKeyToWatch(MR_KEY_0);

    logger.Log("Creating shaders...");
    // create shader
    MR::Shader raymarchingShader = MR::Shader(std::string("Content/vertex.vert"), std::string("Content/fragment.frag"));
    raymarchingShader.activate();

    logger.Log("Creating Mesh...");
    // Create triangles

    MR::Mesh screenMesh = MR::Mesh();

    std::vector<MR::Vector2f> vertices {
        MR::Vector2f(-1.0f, 1.0f),
        MR::Vector2f(-1.0f, -1.0f),
        MR::Vector2f(1.0f, 1.0f),
        MR::Vector2f(1.0f, 1.0f),
        MR::Vector2f(-1.0f, -1.0f),
        MR::Vector2f(1.0f, -1.0f),
    };

    screenMesh.addVertices(vertices);
    screenMesh.uploadMeshData();

    logger.Log("Loading Texture...");
    int width, height, nrChannels;
    std::string image_path = MR::FileSystem::getWorkingDirectory() + "Content/earth.png";
    unsigned char *data;
    MR::FileSystem::loadImage("Content/earth.png", data, width, height, nrChannels);
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    MR::FileSystem::unloadImage(data);

    float elapsed_time = 0.0f; // In seconds
    float delta_time = 0.0f;

    logger.Log("Starting Loop...");
    while (!glfwWindowShouldClose(window))
    {
        std::clock_t beginFrame = clock();
        MR::InputManager::update(delta_time);
        processInput(window, delta_time);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        screenMesh.draw();  

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::clock_t endFrame = clock();
        delta_time = (endFrame - beginFrame) / 1000.0f;
        elapsed_time += delta_time;

        raymarchingShader.setUniformVector3f("u_pPos", player_pos);
        raymarchingShader.setUniformVector2f("u_pRot", player_rot);
    }

    logger.Log("Shutting down application...");
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaTime)
{
    if (MR::InputManager::getKey(MR_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    if (MR::InputManager::getKeyDown(MR_KEY_0)) {
        int polygonMode;
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
        if (polygonMode == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else if (polygonMode == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    MR::InputManager::getMousePosition(cursor_pos);
    if (!(cursor_pos == lcursor_pos)) {
        MR::Vector2d delta_mPos = cursor_pos - lcursor_pos;
        player_rot = player_rot + MR::Vector2f(
            delta_mPos.y,
            delta_mPos.x
        ) * 0.1f * deltaTime;
        lcursor_pos = cursor_pos;
    }

    if (MR::InputManager::getKey(MR_KEY_W))
        player_pos.z += player_speed * deltaTime;
    if (MR::InputManager::getKey(MR_KEY_S))
        player_pos.z -= player_speed * deltaTime;
    if (MR::InputManager::getKey(MR_KEY_A))
        player_pos.x -= player_speed * deltaTime;
    if (MR::InputManager::getKey(MR_KEY_D))
        player_pos.x += player_speed * deltaTime;
    if (MR::InputManager::getKey(MR_KEY_Q))
        player_pos.y -= player_speed * deltaTime;
    if (MR::InputManager::getKey(MR_KEY_E))
        player_pos.y += player_speed * deltaTime;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

    glViewport(0, 0, width, height);
}
