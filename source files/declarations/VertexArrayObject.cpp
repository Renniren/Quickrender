#include <quickrender.h>

VertexArrayObject::VertexArrayObject()
{

}

VertexArrayObject::VertexArrayObject(unsigned int size)
{
	glGenVertexArrays(size, (GLuint*)(&this->handle));
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(this->handle);
}

VertexArrayObject::~VertexArrayObject(){}