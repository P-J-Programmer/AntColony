#pragma once

#include <string>

struct GLFWwindow;

/* 
 * GLFW Callbacks cannot be bound directly to members. 
 */
#define REGISTER_CALLBACK(WindowMethod)\
        [](GLFWwindow* GlfwWindow, auto... Args) \
        {\
            FWindow* const CallbackWindow = static_cast<FWindow*>(glfwGetWindowUserPointer(GlfwWindow));\
            CallbackWindow->WindowMethod(Args...);\
        }

namespace GlfwUtilities
{
	bool InitGlfw();
	void OnGlfwError(const int ErrorCode, const char* Description);
	GLFWwindow* CreateWindow(const int Width, const int Height, const std::string& Title);
}
