#include "Mantaray/Core/InputManager.h"
#include "Mantaray/Core/Logger.h"

using namespace MR;

GLFWwindow* InputManager::m_WindowHandle = nullptr;

std::vector<int> InputManager::m_WatchedKeys = std::vector<int>();
std::map<int, float> InputManager::m_WatchedKeysElapsedTime = std::map<int, float>();
std::map<int, bool> InputManager::m_WatchedKeysDown = std::map<int, bool>();
std::map<int, bool> InputManager::m_WatchedKeysUp = std::map<int, bool>();

void InputManager::setWindowHandle(GLFWwindow* windowHandle) {
    InputManager::m_WindowHandle = windowHandle;
}

GLFWwindow* InputManager::getWindowHandle() {
    if (InputManager::m_WindowHandle == nullptr) {
        Logger::Log("InputManager", "Window handle not set!", Logger::LOG_ERROR);
    }
    return InputManager::m_WindowHandle;
}

void InputManager::update(float deltaTime) {
    GLFWwindow* windowHandle = InputManager::getWindowHandle();
    if (windowHandle == nullptr)
        return;
    
    for (unsigned int i = 0; i < InputManager::m_WatchedKeys.size(); i++) {
        int keyCode = InputManager::m_WatchedKeys[i];
        if (InputManager::getKey(keyCode)) {
            InputManager::m_WatchedKeysDown[keyCode] = (InputManager::m_WatchedKeysElapsedTime[keyCode] > 0);
            InputManager::m_WatchedKeysElapsedTime[keyCode] = 0;
            InputManager::m_WatchedKeysUp[keyCode] = false;
        } else {
            InputManager::m_WatchedKeysUp[keyCode] = (InputManager::m_WatchedKeysElapsedTime[keyCode] == 0);
            InputManager::m_WatchedKeysElapsedTime[keyCode] += deltaTime;
            InputManager::m_WatchedKeysDown[keyCode] = false;
        }
    }
}

void InputManager::addKeyToWatch(int keyCode) {
    InputManager::m_WatchedKeys.push_back(keyCode);

    InputManager::m_WatchedKeysElapsedTime[keyCode] =  1.0f;
    InputManager::m_WatchedKeysDown[keyCode] =  false;
    InputManager::m_WatchedKeysUp[keyCode] =  false;
}

bool InputManager::getKey(int keyCode) {
    return (glfwGetKey(InputManager::getWindowHandle(), keyCode) == GLFW_PRESS);
}

bool InputManager::getKeyDown(int keyCode) {
    return InputManager::m_WatchedKeysDown[keyCode];
}

bool InputManager::getKeyUp(int keyCode) {
    return InputManager::m_WatchedKeysUp[keyCode];
}

void InputManager::getMousePosition(Vector2d &mousePos) {
    glfwGetCursorPos(InputManager::getWindowHandle(), &mousePos.x, &mousePos.y);
}
