#include "Renderer/IndexBuffer.h"

FIndexBuffer::FIndexBuffer() :
    m_Count(0),
    m_Id(GL_NONE)
{
    glGenBuffers(1, &m_Id);
}

FIndexBuffer::FIndexBuffer(const std::vector<GLuint>& Indices) :
    FIndexBuffer()
{
    BufferIndices(Indices);
}

FIndexBuffer::~FIndexBuffer()
{
    glDeleteBuffers(1, &m_Id);
}


void FIndexBuffer::BufferIndices(const std::vector<GLuint>& Indices)
{
    Bind();
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        Indices.size() * sizeof(GLuint),
        (void*)Indices.data(),
        GL_DYNAMIC_DRAW);

    m_Count = static_cast<GLsizei>(Indices.size());
}