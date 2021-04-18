#pragma once

#include <vector>

#include "glm/glm.hpp"

/*
 * Returns position vertices for a quad 1 unit by 1 unit with
 * origin at the centre. Vertices defined clockwise.
 */ 
const std::vector<glm::vec4>& QuadPositions();

class FMesh
{
public:
	using FPositions = std::vector<glm::vec4>;
public:
	FMesh(const FPositions& Positions);

public:
	void SetTransform(const glm::mat4& Transform);
	const FPositions& GetWorldSpacePositions() const { return m_WorldSpacePositions; };

private:
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	FPositions m_WorldSpacePositions;
	FPositions m_ModelSpacePositions;
};
