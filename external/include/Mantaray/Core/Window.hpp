#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shapes.hpp"
#include "Mantaray/Core/Timer.hpp"
#include "Mantaray/Core/Logger.hpp"
#include "Mantaray/OpenGL/Drawables.hpp"

namespace MR {
class Window {
    public:
        Window();
        ~Window();

        static Window*& CreateWindow(std::string title, Vector2u size, bool shouldKeepAspectRatio = true);
        static Window*& CreateWindow(std::string title, Vector2u size, Vector2u resolution, bool shouldKeepAspectRatio = true);
        static Window*& CreateWindow(std::string title, Vector2u size, Vector2u resolution, Vector2f coordinateScale, bool shouldKeepAspectRatio = true);
        static Window*& GetInstance();

        void iconify();
        void maximize();
        void restore();

        void setTitle(std::string title);

        Vector2i getSize();
        void setSize(Vector2i size);

        Vector2i getPosition();
        void setPosition(Vector2i position);

        Vector2f getMouseWorldPosition();

        bool getShouldClose();
        void setShouldClose(bool shouldClose = true);

        float update();
        void beginFrame();
        void endFrame();

        void draw(Sprite& sprite);
        void draw(Polygon& polygon);
        void draw(class Canvas*& canvas);

        void drawLine(Vector2f p1, Vector2f p2, float thickness = 1.f, Color color = Color(0xFF));

        Vector2f getOffset();
        void setOffset(Vector2f offset);
        void addOffset(Vector2f deltaOffset);

        Vector2f getCoordinateScale();
        void setCoordinateScale(Vector2f coordinateScale);

        float getScale();
        void setScale(float scale);

        Vector2f getScaleCenter();
        void setScaleCenter(Vector2f scaleCenter);

        Shader* getScreenShader();
        void setScreenShader(class Shader* screenShader);

        Color getClearColor();
        void setClearColor(Color clearColor);
        void setClearColor(unsigned char clearColor);

        Rectanglei getViewportRect();

    protected:
        static void OnWindowResized(class GLFWwindow* window, int width, int height);
        void initialize(std::string title, Vector2u size, Vector2u resolution, Vector2f coordinateScale, bool shouldKeepAspectRatio = true);
        void calculateViewDestination(int windowWidth, int windowHeight);
        void display();
    
    private:
        Logger m_Logger = Logger("Window");
        static Window* Instance;
        class GLFWwindow* m_Window = nullptr;
        class Canvas* m_DisplayBuffer = nullptr;
        class Shader* m_DisplayShader;
        Color m_ClearColor = Color(0x00);
        Timer m_Timer;
        bool m_ShouldKeepAspectRatio;
        float m_PrefferedAspectRatio;
        Vector2i m_lastWindowedPosition = Vector2i();
        Vector2i m_lastWindowedSize = Vector2i();
        Rectanglei m_ViewportRect;
};
}
