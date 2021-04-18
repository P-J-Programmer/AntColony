#include "Renderer/Mesh.h"

FMesh::FMesh(const FPositions& Positions) :
	m_ModelMatrix(1.0f),
	m_WorldSpacePositions(Positions),
	m_ModelSpacePositions(Positions)
{
}

void FMesh::SetTransform(const glm::mat4& Transform)
{
	if (Transform == m_ModelMatrix)
	{
		return;
	}

	assert(m_ModelSpacePositions.size() == m_WorldSpacePositions.size());

	for (std::size_t PositionIdx = 0; PositionIdx < m_ModelSpacePositions.size(); ++PositionIdx)
	{
		m_WorldSpacePositions[PositionIdx] = Transform * m_ModelSpacePositions[PositionIdx];
	}

	m_ModelMatrix = Transform;
}

const std::vector<glm::vec4>& QuadPositions()
{
	auto CreateQuadPositions = []() -> std::vector<glm::vec4>
	{
		const unsigned int NumPositions = 4;
		std::vector<glm::vec4> QuadPositions(NumPositions);
		QuadPositions[0] = { -0.5f, -0.5f, -0.0f, 1.0f }; // bottom left
		QuadPositions[1] = { 0.5f, -0.5f, -0.0f, 1.0f };  // bottom right
		QuadPositions[2] = { 0.5f, 0.5f, -0.0f, 1.0f };	  // top right
		QuadPositions[3] = { -0.5f, 0.5f, -0.0f, 1.0f };  // top left 

		return QuadPositions;
	};

	static std::vector<glm::vec4> QuadPositions = CreateQuadPositions();
	return QuadPositions;
}
