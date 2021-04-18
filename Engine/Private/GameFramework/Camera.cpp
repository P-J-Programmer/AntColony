#include "GameFramework/Camera.h"

#include "glm/gtc/matrix_transform.hpp"

FCamera::FOrthographicParams FCamera::CreateOrthoParams(const float AspectRatio, const float Height)
{
	FOrthographicParams OrthographicParams;
	OrthographicParams.Bottom = -Height;
	OrthographicParams.Top = Height;
	OrthographicParams.Left = -AspectRatio * Height;
	OrthographicParams.Right = AspectRatio * Height;
	return OrthographicParams;
}

glm::mat4 FCamera::CreateProjection(const FOrthographicParams& OrthoParams)
{
	return glm::ortho(OrthoParams.Left, OrthoParams.Right, OrthoParams.Bottom, OrthoParams.Top, OrthoParams.zNear, OrthoParams.zFar);
}

glm::mat4 FCamera::CreateView(const glm::vec3& Position)
{
	return glm::inverse(glm::translate(IdentityMatrix, Position));
}

FCamera::FCamera(const FOrthographicParams& OrthoParams) :
	m_Projection(CreateProjection(OrthoParams)),
	m_View(CreateView(glm::vec3(0.0f)))
{

}
