#include "ImguiUtilities.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD 1

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void InitialiseImgui(GLFWwindow& Window)
{
    const char* glsl_version = "#version 460";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(&Window, /*install_callbacks*/ true);
    ImGui_ImplOpenGL3_Init(glsl_version);

}
