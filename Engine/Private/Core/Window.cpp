#include "Core/Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Logger/Logger.h"
#include "Renderer/Utility/GlfwUtilities.h"
#include "Renderer/Utility/OpenGlUtilities.h"

FWindow::FWindow(const int Width, const int Height, const std::string& Title)
{
    m_bWasInitSuccessful = GlfwUtilities::InitGlfw();
    
    if (!m_bWasInitSuccessful)
    {
        return;
    }

    m_bWasInitSuccessful = CreateWindow(Width, Height, Title);
    if (!m_bWasInitSuccessful)
    {
        return;
    }

    glfwMakeContextCurrent(m_Window);

    m_bWasInitSuccessful = OpenGlUtilities::InitGlad();
    if (!m_bWasInitSuccessful)
    {
        return;
    }
    RegisterCallbacks();
}

bool FWindow::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void FWindow::ClearWindow() const
{
    glClearColor(0.2f, 0.6f, 0.2f, 0.8f); 
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FWindow::PollEvents() const
{
    glfwPollEvents();
}

void FWindow::SwapBuffers() const
{
    glfwSwapBuffers(m_Window);
}


bool FWindow::CreateWindow(const int Width, const int Height, const std::string& Title)
{
    m_Window = GlfwUtilities::CreateWindow(Width, Height, Title);

    if (m_Window != nullptr)
    {
        Log().Info("Successfully created GLFW window");
    }
    else
    {
        Log().Error("Failed to create GLFW window");
    }
    
    return m_Window != nullptr;
}

void FWindow::RegisterCallbacks()
{
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, REGISTER_CALLBACK(OnFramebufferResize));
    glfwSetWindowCloseCallback(m_Window, REGISTER_CALLBACK(OnWindowClose));
    glfwSetKeyCallback(m_Window, REGISTER_CALLBACK(OnKeySet));
}

void FWindow::OnFramebufferResize(const int Width, const int Height) const
{
    glViewport(0, 0, Width, Height);
}

void FWindow::OnWindowClose()
{
    glfwSetWindowShouldClose(m_Window, GL_TRUE);
}

void FWindow::OnKeySet(int Key, int Scancode, int Action, int Mode)
{
    m_KeyPressedDelegate(Key, Scancode, Action, Mode);
}

FWindow::~FWindow()
{
    glfwTerminate();
}
