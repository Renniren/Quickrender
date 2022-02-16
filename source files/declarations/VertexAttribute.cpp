#include <quickrender.h>

VertexAttribute::VertexAttribute()
{

}

VertexAttribute::VertexAttribute(int attribloc, int attribsize, int attribtype, int attribnorm, int attribstride, int attriboffset)
{
	glVertexAttribPointer(attribloc, attribsize, attribtype, attribnorm, attribstride, (void*)attriboffset);
}

VertexAttribute::~VertexAttribute(){}