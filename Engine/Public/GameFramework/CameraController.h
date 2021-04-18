#pragma once

#include "GameFramework/Camera.h"

#include "glm/glm.hpp"

class FInput;

class FCameraController
{
public:
	FCameraController(const float AspectRatio, const float Height);

	void HandleInput(const FInput& Input, const float DeltaSeconds);
	void TeleportCamera(const glm::vec2& Translation);
	void SetCameraSpeed(const float TranslationPerSecond) { m_TranslationPerSecond = TranslationPerSecond; };
	void SetCameraZoom(const float Zoom);

	glm::mat4 GetViewProjection() const;
private:
	FCamera m_Camera;

	float m_AspectRatio = 0.0f;
	float m_Height = 0.0f;

	glm::vec2 m_CameraPosition = glm::vec2(0.0f); 
	float m_TranslationPerSecond = 5.0f;
};
