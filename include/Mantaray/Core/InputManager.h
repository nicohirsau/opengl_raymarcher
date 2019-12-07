#include <map>
#include <vector>

#include <GLFW/glfw3.h>

#include "Mantaray/Core/Vector.h"

namespace MR {
class InputManager{
    public:
        static void setWindowHandle(GLFWwindow* windowHandle);
        static GLFWwindow* getWindowHandle();

        static void update(float deltaTime);

        static void addKeyToWatch(int keyCode);

        static bool getKey(int keyCode);
        static bool getKeyDown(int keyCode);
        static bool getKeyUp(int keyCode);
        static void getMousePosition(Vector2d &mousePos);

    private:
        static GLFWwindow* m_WindowHandle;

        static std::vector<int> m_WatchedKeys;
        static std::map<int, float> m_WatchedKeysElapsedTime;
        static std::map<int, bool> m_WatchedKeysDown;
        static std::map<int, bool> m_WatchedKeysUp;
};
}
