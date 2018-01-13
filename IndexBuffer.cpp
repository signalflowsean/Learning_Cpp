#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
  : m_Count(count)
{
	//generates buffer(s)
    GLCall(glGenBuffers(1, &m_RendererID));
    //select buffer to render data
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    //cout << "Binded buffer to opengl" << endl;
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));  
    //cout << "Generated buffer data addeed posistions" << endl;
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); 
}

void IndexBuffer::Bind() const 
{
   GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const 
{
   GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}