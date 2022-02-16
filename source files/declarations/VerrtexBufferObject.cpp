#include <quickrender.h>

VertexBufferObject::VertexBufferObject()
{
	this->data = placeholder;
	this->size = 0;
	this->buffer = 0;
}

VertexBufferObject::VertexBufferObject(int size, int buffer, void* data)
{
	this->data = placeholder;
	this->size = size;
	this->buffer = buffer;
	glGenBuffers(size, (GLuint*)handle);
}

void VertexBufferObject::Copy(int buffer, int size, void* data, int drawtype)
{
	this->size = size;
	this->buffer = buffer;
	this->data = data;
	glBufferData(buffer, size, data, drawtype);
}

void VertexBufferObject::Bind()
{
	glBindBuffer(this->buffer, this->handle);
}

void VertexBufferObject::Draw()
{
	glBindBuffer(this->buffer, this->handle);
	glDrawArrays(GL_TRIANGLES, 0, 512);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(this->size, (GLuint*)this->handle);
}