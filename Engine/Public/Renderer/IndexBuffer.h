#pragma once

#include <vector>

#include "glad/glad.h"

class FIndexBuffer
{
public:
    FIndexBuffer();
    FIndexBuffer(const std::vector<GLuint>& Indices);
    ~FIndexBuffer();

public:
    void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id); }
    static void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE); }

    GLsizei Count() const { return m_Count; }

    void BufferIndices(const std::vector<GLuint>& Indices);

private:
    FIndexBuffer(const FIndexBuffer& Buffer) = delete;
    FIndexBuffer& operator=(const FIndexBuffer& Buffer) = delete;

    FIndexBuffer(FIndexBuffer&& Buffer) = delete;
    FIndexBuffer& operator=(FIndexBuffer&& Buffer) = delete;

private:
    GLsizei m_Count;
    GLuint m_Id;
};
