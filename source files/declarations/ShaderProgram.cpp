#include <quickrender.h>

ShaderProgram::ShaderProgram()
{
	this->handle = glCreateProgram();
}

void ShaderProgram::LinkProgram()
{
	glLinkProgram(this->handle);
}

void ShaderProgram::Attach(Shader _Shader)
{
	glAttachShader(this->handle, _Shader.handle);
}

void ShaderProgram::Attach(Shader* _Shader)
{
	glAttachShader(this->handle, _Shader->handle);
}


void ShaderProgram::Attach(std::vector<Shader> Shaders)
{
	for (int i = 0; i < Shaders.size(); i++)
	{
		Attach(Shaders[i]);
	}
}

void ShaderProgram::Use()
{
	glUseProgram(this->handle);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->handle);
}