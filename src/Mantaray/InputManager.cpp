#include "Mantaray/Core/InputManager.h"
#include "Mantaray/Core/Logger.h"

using namespace MR;

GLFWwindow* InputManager::m_WindowHandle = nullptr;

void InputManager::setWindowHandle(GLFWwindow* windowHandle) {
    InputManager::m_WindowHandle = windowHandle;
}

GLFWwindow* InputManager::getWindowHandle() {
    if (InputManager::m_WindowHandle == nullptr) {
        Logger::Log("InputManager", "Window handle not set!", Logger::LOG_ERROR);
    }
    return InputManager::m_WindowHandle;
}

bool InputManager::getKey(int keyCode) {
    return (glfwGetKey(InputManager::getWindowHandle(), keyCode) == GLFW_PRESS);
}

bool InputManager::getKeyDown(int keyCode) {
    return false;
}

void InputManager::getMousePosition(Vector2d &mousePos) {
    glfwGetCursorPos(InputManager::getWindowHandle(), &mousePos.x, &mousePos.y);
}
