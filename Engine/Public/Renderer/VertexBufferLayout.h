#pragma once

#include <numeric>
#include <string>
#include <vector>

#include "glad/glad.h"

#include "Vertices.h"

struct FVertexLayoutAttrib
{
    GLenum Type = GL_INVALID_VALUE;
    GLuint Index = 0;
    GLint Count = 0;
    GLsizei TotalBytes = 0;
};

class FBufferLayout
{
public:
    using FIteratorType = std::vector<FVertexLayoutAttrib>::const_iterator;

public:
    template <typename Vertex>
    void AddVertexAttrib(const std::string& DebugName)
    {
        FVertexLayoutAttrib Attrib;
        Attrib.Type = FVertexInfo<Vertex>::Type;
        Attrib.Index = (GLuint)m_Attributes.size();
        Attrib.Count = FVertexInfo<Vertex>::Count;
        Attrib.TotalBytes = FVertexInfo<Vertex>::Bytes;
        m_Attributes.emplace_back(std::move(Attrib));
    }

    const std::vector<FVertexLayoutAttrib>& GetAttributes() const { return m_Attributes; }
    FIteratorType begin() const { return m_Attributes.begin(); }
    FIteratorType end() const { return m_Attributes.end(); }
private:
    std::vector<FVertexLayoutAttrib> m_Attributes;
};

inline GLsizei CalculateStride(const std::vector<FVertexLayoutAttrib>& Attributes)
{
    auto SumBytes = [](const GLsizei Bytes, const FVertexLayoutAttrib& Attrib)
    {
        return Bytes + Attrib.TotalBytes;
    };

    return std::accumulate(Attributes.begin(), Attributes.end(), (GLsizei)0, SumBytes);
}

inline GLsizei CalculateStride(const FBufferLayout& BufferLayout)
{
    return CalculateStride(BufferLayout.GetAttributes());
}
