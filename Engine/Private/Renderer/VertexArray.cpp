#include "Renderer/VertexArray.h"

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/VertexBuffer.h"

FVertexArray::FVertexArray(const FVertexBuffer& VertexBuffer, const FIndexBuffer& IndexBuffer)
{
    glGenVertexArrays(1, &m_Id);
    BindVertexBuffer(VertexBuffer);
    BindIndexBuffer(IndexBuffer);
}

FVertexArray::FVertexArray::~FVertexArray()
{
    glDeleteVertexArrays(1, &m_Id);
}

void FVertexArray::BindVertexBuffer(const FVertexBuffer& Buffer)
{
	Bind();
	Buffer.Bind();
	const FBufferLayout& BufferLayout = Buffer.GetLayout();

	GLuint64 Offset = 0;
	GLuint Stride = CalculateStride(BufferLayout);
	for (const FVertexLayoutAttrib& Attrib : BufferLayout)
	{
		glVertexAttribPointer(Attrib.Index, Attrib.Count, Attrib.Type, /*normalized*/ GL_FALSE, Stride, (void*)Offset);
		glEnableVertexAttribArray(Attrib.Index);
		Offset += Attrib.TotalBytes;
	}
	Unbind();
}

void FVertexArray::BindIndexBuffer(const FIndexBuffer& Buffer)
{
	Bind();
	Buffer.Bind();
	m_Count = Buffer.Count();
	Unbind();
}
