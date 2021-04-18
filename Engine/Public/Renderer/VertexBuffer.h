#pragma once

#include <vector>

#include "glad/glad.h"
#include "VertexBufferLayout.h"

class FVertexBuffer
{
public:
    FVertexBuffer(FBufferLayout Layout) :
        m_Layout(std::move(Layout)),
        m_Id(GL_NONE)
    {
        glGenBuffers(1, &m_Id);
    }

    template <typename T>
    FVertexBuffer(const std::vector<T>& Data, FBufferLayout Layout) :
        FVertexBuffer(Layout)
    {
        BufferData(Data);
    }

    ~FVertexBuffer()
    {
        glDeleteBuffers(1, &m_Id);
    }

    void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Id); }
    static void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, GL_NONE); }

    const FBufferLayout& GetLayout() const { return m_Layout; }

    template <typename T>
    void BufferData(const std::vector<T>& Data)
    {
        Bind();
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(T) * Data.size(),
            (void*)Data.data(),
            GL_DYNAMIC_DRAW);
    }

private:
    FVertexBuffer(const FVertexBuffer& Buffer) = delete;
    FVertexBuffer& operator=(const FVertexBuffer& Buffer) = delete;

    FVertexBuffer(FVertexBuffer&& Buffer) = delete;
    FVertexBuffer& operator=(FVertexBuffer&& Buffer) = delete;

private:
    FBufferLayout m_Layout;
    GLuint m_Id;
};