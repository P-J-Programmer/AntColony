#pragma once

#include <vector>

#include "glad/glad.h"

class FVertexBuffer;
class FIndexBuffer;

class FVertexArray
{
public:
    FVertexArray(const FVertexBuffer& VertexBuffer, const FIndexBuffer& IndexBuffer);
    ~FVertexArray();

public:
    void Bind() const { glBindVertexArray(m_Id); }
    static void Unbind() { glBindVertexArray(GL_NONE); }

    GLsizei Count() const { return m_Count; }

private:
    void BindVertexBuffer(const FVertexBuffer& Buffer); 
    void BindIndexBuffer(const FIndexBuffer& Buffer);

  
private:
    FVertexArray(const FVertexArray& Array) = delete;
    FVertexArray& operator=(const FVertexArray& Array) = delete;

    FVertexArray(FVertexArray&& Array) = delete;
    FVertexArray& operator=(FVertexArray&& Array) = delete;

private:
    GLsizei m_Count = 0;
    GLuint m_Id = GL_NONE;
};
