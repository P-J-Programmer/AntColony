#pragma once

#include <string>

#include "Core/Delegates.h"

struct GLFWwindow;

class FWindow final
{
	friend class UserInterface;


public:
	using FKeyPressedDelegate = FDelegate<int, int, int, int>;


public:
	FWindow(const int Width, const int Height, const std::string& Title);
	~FWindow();
	
	bool ShouldClose() const;
	void ClearWindow() const;
	void PollEvents() const;
	void SwapBuffers() const;
	bool WasInitSuccess() { return m_Window != nullptr; };

	FKeyPressedDelegate& GetKeyPressedDelegate() { return m_KeyPressedDelegate; }

private:
	FWindow(const FWindow& Buffer) = delete;
	FWindow& operator=(const FWindow& Buffer) = delete;

	FWindow(FWindow&& Buffer) = delete;
	FWindow& operator=(FWindow&& Buffer) = delete;

private:
	bool CreateWindow(const int Width, const int Height, const std::string& Title);
	void RegisterCallbacks();

	void OnFramebufferResize(const int Width, const int Height) const;
	void OnWindowClose();
	void OnKeySet(int Key, int Scancode, int Action, int Mode);

private:
	GLFWwindow* m_Window = nullptr;

	FKeyPressedDelegate m_KeyPressedDelegate;

	bool m_bWasInitSuccessful = false;
};
