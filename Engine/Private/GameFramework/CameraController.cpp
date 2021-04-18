#include "GameFramework/CameraController.h"

#include "GLFW/glfw3.h"

#include "Core/Input.h"

FCameraController::FCameraController(const float AspectRatio, const float Height) :
	m_Camera(FCamera::CreateOrthoParams(AspectRatio, Height)),
	m_AspectRatio(AspectRatio),
	m_Height(Height)
{
}

void FCameraController::HandleInput(const FInput& Input, const float DeltaSeconds) // interface
{
	const float DeltaPos = m_TranslationPerSecond * DeltaSeconds;
	glm::vec2 Translation = glm::vec2(0.0f);
	if (Input.IsKeyPressed(GLFW_KEY_W))
	{
		Translation.y += DeltaPos;
	}

	if (Input.IsKeyPressed(GLFW_KEY_S))
	{
		Translation.y -= DeltaPos;
	}

	if (Input.IsKeyPressed(GLFW_KEY_D))
	{
		Translation.x += DeltaPos;
	}

	if (Input.IsKeyPressed(GLFW_KEY_A))
	{
		Translation.x -= DeltaPos;
	}


	TeleportCamera(Translation);
}

void FCameraController::TeleportCamera(const glm::vec2& Translation)
{
	m_CameraPosition += Translation;
	m_Camera.UpdateView(glm::vec3(m_CameraPosition, 0.0f));
}

void FCameraController::SetCameraZoom(const float Zoom)
{
	m_Camera.UpdateProjection(FCamera::CreateOrthoParams(m_AspectRatio, Zoom));
}

glm::mat4 FCameraController::GetViewProjection() const
{
	return m_Camera.GetProjection() * m_Camera.GetView();
}
