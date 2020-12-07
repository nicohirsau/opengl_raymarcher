#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <algorithm>
#include <vector>

#include <Mantaray/Core/Window.hpp>
#include <Mantaray/Core/InputManager.hpp>
#include <Mantaray/Core/KeyCodes.hpp>
#include <Mantaray/OpenGL/ObjectLibrary.hpp>
#include <Mantaray/OpenGL/Drawables.hpp>

#define PI 3.14159265359f

using namespace MR;

void processInput(float deltaTime);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 500;
unsigned int SCR_RES_WIDTH = 1600;
unsigned int SCR_RES_HEIGHT = 1000;
Vector2d cursor_pos;
Vector2d lcursor_pos;
Vector3f player_pos;
Vector2f player_rot;
float player_speed = 1.0f;
Logger logger("Application");
bool ROTATION_LOCKED = false;

int main()
{
    Window* window = Window::CreateWindow("OpenGL", Vector2u(SCR_WIDTH, SCR_HEIGHT), Vector2u(SCR_RES_WIDTH, SCR_RES_HEIGHT));
    InputManager::AddKeyToWatch(MR_KEY_ESCAPE);
    InputManager::AddKeyToWatch(MR_KEY_SPACE);
    InputManager::AddKeyToWatch(MR_KEY_R);
    //InputManager::SetCursorMode(InputManager::CursorMode::DISABLED);

    Shader *raymarchingShader = ObjectLibrary::CreateShader("RaymarchingShader", std::string("Content/vertex.vert"), std::string("Content/fragment.frag"));
    raymarchingShader->setUniformVector2f("u_Res", MR::Vector2f((float)SCR_WIDTH, (float)SCR_HEIGHT));

    Texture* tex = ObjectLibrary::CreateTexture("color_texture", "Content/earth.png");
    Texture* tex1 = ObjectLibrary::CreateTexture("displacement_texture", "Content/earth_displacement.png");
    raymarchingShader->setTexture("u_texture0", 0, *tex);
    raymarchingShader->setTexture("u_texture1", 1, *tex1);

    VertexArray* defaultVertexArray;
    ObjectLibrary::FindObject("DefaultVertexArray", defaultVertexArray);
    Polygon screenPolygon = Polygon(defaultVertexArray);
    screenPolygon.color = Color(255, 255, 255);
    screenPolygon.size = Vector2f(SCR_WIDTH, SCR_HEIGHT);
    screenPolygon.shader = raymarchingShader;

    float elapsed_time = 0.0f;
    float delta_time = 0.0f;

    // Warmup frame
    window->update();
    InputManager::GetMousePosition(lcursor_pos);
    InputManager::GetMousePosition(cursor_pos);

    logger.Log("Starting Loop...");
    while (!window->getShouldClose())
    {
        delta_time = window->update();
        elapsed_time += delta_time;

        if (InputManager::GetKeyDown(MR_KEY_ESCAPE))
            window->setShouldClose(true);

        if (InputManager::GetKeyDown(MR_KEY_R)) {
            ObjectLibrary::DeleteObject("RaymarchingShader");
            raymarchingShader = ObjectLibrary::CreateShader("RaymarchingShader", std::string("Content/vertex.vert"), std::string("Content/fragment.frag"));
        }

        processInput(delta_time);

        window->beginFrame();

        raymarchingShader->setUniformVector3f("u_pPos", player_pos);
        raymarchingShader->setUniformVector2f("u_pRot", player_rot);
        raymarchingShader->setUniformFloat("u_time", elapsed_time);
        raymarchingShader->setUniformVector2f("u_Res", Vector2f(SCR_RES_WIDTH, SCR_RES_HEIGHT));
        window->draw(screenPolygon);

        window->endFrame();
    }

    logger.Log("Shutting down application...");
    delete window;
    return 0;
}

void processInput(float deltaTime)
{
    if (InputManager::GetKeyDown(MR_KEY_SPACE)) {
        ROTATION_LOCKED = !ROTATION_LOCKED;
        InputManager::GetMousePosition(lcursor_pos);
        InputManager::GetMousePosition(cursor_pos);
    }

    InputManager::GetMousePosition(cursor_pos);
    if (!(cursor_pos == lcursor_pos) && !ROTATION_LOCKED) {
        Vector2d delta_mPos = cursor_pos - lcursor_pos;
        player_rot = player_rot + Vector2f(
            delta_mPos.y,
            delta_mPos.x
        ) * 0.1f * deltaTime;
        lcursor_pos = cursor_pos;
    }

    if (player_rot.x < -PI/2.f)
        player_rot.x = -PI/2.f;
    if (player_rot.x > PI/2.f)
        player_rot.x = PI/2.f;

    Vector3f delta_movement;
    if (InputManager::GetKey(MR_KEY_W))
        delta_movement.z += player_speed * deltaTime;
    if (InputManager::GetKey(MR_KEY_S))
        delta_movement.z -= player_speed * deltaTime;
    if (InputManager::GetKey(MR_KEY_A))
        delta_movement.x -= player_speed * deltaTime;
    if (InputManager::GetKey(MR_KEY_D))
        delta_movement.x += player_speed * deltaTime;

    delta_movement.rotateAroundX(player_rot.x);
    delta_movement.rotateAroundY(player_rot.y);

    player_pos = player_pos + delta_movement;

    if (InputManager::GetKey(MR_KEY_Q))
        player_pos.y -= player_speed * deltaTime;
    if (InputManager::GetKey(MR_KEY_E))
        player_pos.y += player_speed * deltaTime;
}
