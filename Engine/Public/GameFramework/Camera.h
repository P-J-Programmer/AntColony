#pragma once

#include "glm/glm.hpp"

constexpr glm::mat4 IdentityMatrix = glm::mat4(1.0f);

class FCamera
{
public:
	struct FOrthographicParams
	{
		float Left = 0.0f;
		float Right = 0.0f;
		float Bottom = 0.0f;
		float Top = 0.0f;
		float zNear = -1.0f;
		float zFar = 1.0f;
	};

	static FOrthographicParams CreateOrthoParams(const float AspectRatio, const float Height);
	static glm::mat4 CreateProjection(const FOrthographicParams& OrthoParams);
	static glm::mat4 CreateView(const glm::vec3& Position);
public:
	FCamera(const FOrthographicParams& Orthographic);

	const glm::mat4& GetProjection() const { return m_Projection; }
	const glm::mat4& GetView() const { return m_View; }

	void UpdateProjection(const FOrthographicParams& OrthoParams) { m_Projection = CreateProjection(OrthoParams); };
	void UpdateView(const glm::vec3& Position) { m_View = CreateView(Position); };

private:  
	glm::mat4 m_Projection = IdentityMatrix;
	glm::mat4 m_View = IdentityMatrix;
};