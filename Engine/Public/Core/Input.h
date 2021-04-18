#pragma once

#include <unordered_map>

class FWindow;

class FInput
{
public:
	
	FInput(FWindow& Window);

public:
	bool IsKeyPressed(int KeyCode) const;

private:
	void OnKeySet(int Key, int Scancode, int Action, int Mode);

private:
	using FKeyCode = int;
	using FKeyStatus = int;

	std::unordered_map<FKeyCode, FKeyStatus> m_Keys;
};