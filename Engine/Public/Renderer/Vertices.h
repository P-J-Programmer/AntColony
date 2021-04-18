#pragma once

#include "glad/glad.h"

struct Vertex2D
{
	float x = 0.0f;
	float y = 0.0f;
};

struct Vertex3D
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Vertex4D
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

template <typename Vertex>
struct FVertexInfo
{
};

template <>
struct FVertexInfo<Vertex2D>
{
	static constexpr GLenum Type = GL_FLOAT;
	static constexpr GLint Count = 2;
	static constexpr GLsizei Bytes = sizeof(Vertex2D);
};

template <>
struct FVertexInfo<Vertex3D>
{
	static constexpr GLenum Type = GL_FLOAT;
	static constexpr GLint Count = 3;
	static constexpr GLsizei Bytes = sizeof(Vertex3D);
};

template <>
struct FVertexInfo<Vertex4D>
{
	static constexpr GLenum Type = GL_FLOAT;
	static constexpr GLint Count = 4;
	static constexpr GLsizei Bytes = sizeof(Vertex4D);
};
