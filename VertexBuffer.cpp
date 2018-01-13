#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	//generates buffer(s)
    GLCall(glGenBuffers(1, &m_RendererID));
    //select buffer to render data
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //cout << "Binded buffer to opengl" << endl;
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));  
    //cout << "Generated buffer data addeed posistions" << endl;
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); 
}

void VertexBuffer::Bind() const 
{
   GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const 
{
   GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}