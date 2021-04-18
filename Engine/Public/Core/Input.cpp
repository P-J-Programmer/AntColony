#include "Input.h"

#include "GLFW/glfw3.h"

#include "Core/Window.h"


namespace
{
	bool IsStatusPressed(int KeyStatus)
	{
		return KeyStatus == GLFW_PRESS ||
			KeyStatus == GLFW_REPEAT;
	}
}

FInput::FInput(FWindow& Window)
{
	Window.GetKeyPressedDelegate().Add(this, &FInput::OnKeySet);
}

bool FInput::IsKeyPressed(int KeyCode) const
{
	auto Iter = m_Keys.find(KeyCode);
	return Iter != m_Keys.end() && IsStatusPressed(Iter->second);
}

void FInput::OnKeySet(int Key, int Scancode, int Action, int Mode)
{
	m_Keys[Key] = Action;
}
