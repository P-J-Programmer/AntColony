#include <vector>

#include "GlfwUtilities.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Logger/Logger.h"
#include "OpenGlUtilities.h"

namespace GlfwUtilities
{
    bool InitGlfw()
    {
        const int GlfwInitSuccess = glfwInit();
        
        if (GlfwInitSuccess == GLFW_TRUE)
        {
            glfwSetErrorCallback(&OnGlfwError);
            Log().Info("Initalised GLFW successfully");
        }
        else
        {
            Log().Error("Failed to initialise GLFW");
        }

        return GlfwInitSuccess;
    }

    GLFWwindow* CreateWindow(const int Width, const int Height, const std::string& Title)
	{
        constexpr int GlMajorVersion = 4;
        constexpr int GlMinorVersion = 6;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Needed for glDebugMessageCallback to work

        return glfwCreateWindow(
            Width,
            Height,
            Title.c_str(),
            /*monitor*/nullptr, // Null monitor means windowed mode
            /*share*/nullptr);  // Null share means no context sharing between windows.
	}

    void OnGlfwError(const int ErrorCode, const char* Description)
    {
        Log().Error("[Error:", ErrorCode, "]", Description);
    }
}
