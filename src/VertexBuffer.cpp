#include "Renderer.h"
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	GLCall((1, &m_RenderID)); // Return ID for this new buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); // Selecting buffer select == bind
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
