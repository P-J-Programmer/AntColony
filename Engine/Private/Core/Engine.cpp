#include "Core/Engine.h"

#include "EngineConfig.h"

#include "Core/Game.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/TextureAtlas.h"
#include "Logger/Logger.h"

FEngine::FEngine() :
	m_Window(WindowWidth, WindowHeight, WindowTitle),
	m_Ui(m_Window),
	m_Input(m_Window),
	m_Game(CreateGame())
{

}

FEngine::~FEngine()
{
}

void FEngine::Run()
{
	if (m_Game == nullptr)
	{
		Log().Error("Engine has no game instance.");
		return;
	}

	while (!m_Window.ShouldClose())
	{
		const float DeltaTime = FrameTimer.GetSecondsElapsed();

		FrameTimer.Reset();

		m_Window.ClearWindow();
		m_Window.PollEvents();

		m_Game->SubmitRenderCommands(m_Renderer);
		m_Game->HandleInput(m_Input, DeltaTime);
		m_Game->Update(DeltaTime);
		m_Ui.PreTick();
		m_Game->DrawUi();
		m_Ui.PostTick();

		m_Window.SwapBuffers();
	}
}
