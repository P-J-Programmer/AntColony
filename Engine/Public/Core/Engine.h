#pragma once

#include <memory>

#include "Core/Input.h"
#include "Core/Timer.h"
#include "Core/Window.h"
#include "Renderer/UI/UserInterface.h"
#include "Renderer/Renderer.h"

class FGame;

class FEngine
{
public:
    FEngine();
    ~FEngine();

    void Run();
private:
    FWindow m_Window;
    UserInterface m_Ui;

    FRenderer m_Renderer;

    FTimer FrameTimer;

    FInput m_Input;

    std::unique_ptr<FGame> m_Game;
};