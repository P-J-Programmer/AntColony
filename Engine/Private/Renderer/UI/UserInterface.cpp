#include "Renderer/UI/UserInterface.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/Window.h"
#include "Renderer/Utility/ImguiUtilities.h"

UserInterface::UserInterface(const FWindow& Window)
{
	if (Window.m_Window)
	{
		InitialiseImgui(*Window.m_Window);
	}
}

UserInterface::~UserInterface()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::PreTick()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void UserInterface::PostTick()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
