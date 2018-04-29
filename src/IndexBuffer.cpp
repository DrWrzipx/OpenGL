#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall((1, &m_RenderID)); // Return ID for this new buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID); // Selecting buffer select == bind
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderID));
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
