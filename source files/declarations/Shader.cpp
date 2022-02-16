#include <quickrender.h>
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const char* path, int type, bool UsePath)
{
	this->handle = glCreateShader(type);

    if (UsePath)
    {
        std::string code;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            code = stream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR: Unable to read given shader file: " << e.what() << std::endl;
        }

        const char* charCode = code.c_str();
    
	    glShaderSource(this->handle, 1, &charCode, NULL);
    }
    if(UsePath == false)
    {
        glShaderSource(this->handle, 1, &path, NULL);
    }
}

void Shader::LinkCode(const char* source, bool UsePath)
{
    if (UsePath)
    {
        std::string code;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            file.open(source);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            code = stream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR: Unable to read given shader file: " << e.what() << std::endl;
        }

        const char* charCode = code.c_str();

        glShaderSource(this->handle, 1, &charCode, NULL);
    }
    else
    {
        glShaderSource(this->handle, 1, &source, NULL);
    }
}

void Shader::Compile()
{
	glCompileShader(this->handle);

	int  success;
	char infoLog[512];
	glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(this->handle, 512, NULL, infoLog);
		std::cout << "ERROR: Shader compilation failed; Log:\n" << infoLog << std::endl;
	}
}

void Shader::Delete()
{
	glDeleteShader(this->handle);
}

Shader::~Shader()
{
	
}