#pragma once

#include <vector>

/*
 * Buffer that tracks the number of unique elements already added
 * such that when a new index is added to the buffer it will be added
 * as index + current offset
 */
class FOffsetBuffer
{
public:
	void AddIndices(const std::vector<unsigned int>& Buffer);
	void Clear() { m_Buffer.clear(); m_Offset = 0; };
	const std::vector<unsigned int>& GetBuffer() { return m_Buffer; }

private:
	std::vector<unsigned int> m_Buffer;
	std::size_t m_Offset = 0;
};

struct FRenderVertexData
{
	std::vector<float> Vertices;
	FOffsetBuffer Indices;
};

inline void ClearRenderVertexData(FRenderVertexData& RenderVertexData)
{
	RenderVertexData.Indices.Clear();
	RenderVertexData.Vertices.clear();
}