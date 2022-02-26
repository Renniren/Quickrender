#ifndef QUICKRENDER_MAIN_HEADER
#define QUICKRENDER_MAIN_HEADER

#pragma once

#include <glad.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <glfw3.h>
#include <fstream>
#include <glm.hpp>
#include <sstream>
#include <iostream>
#include <quickerror.h>
#include <quickdefines.h>
#include <quickmacros.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";


using namespace std;
using namespace glm;


int width = 800, height = 600;

void ResetState()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindTexture(GL_TEXTURE_1D, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_3D, 0);

	glUseProgram(0);
	glBindVertexArray(0);
}

void ViewResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double x, double y)
{

}

void MSetupMemoryChecks()
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
}

void MCheckMemory()
{
	_CrtDumpMemoryLeaks();
}



GLFWwindow* glSetup()
{
	// glfw: initialize and configure
   // ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();


	glfwSetFramebufferSizeCallback(window, ViewResizeCallback);
	glViewport(0, 0, width, height);

	return window;
}

class GLObject
{
	
public:

	uint id = -1;

	GLObject()
	{
		objects.push_back(*this);
	}

	virtual string ToString()
	{
		string result;
		stringstream ss;
		ss << id;
		ss >> result;
		return result;
	}

	virtual void Delete()
	{

	}

	GLObject* globj = this;
	static vector<GLObject> objects;
};

void Cleanup()
{
	for (int i = 0; i < GLObject::objects.size(); i++)
	{
		GLObject::objects[i].Delete();
	}

	glfwTerminate();
}

class WorldObject 
{
public:

	vec3 position = zerovec;
	vec3 euler = zerovec;
	vec3 scale = zerovec;

	mat4 model = mat4(1.0f);

	vec3 forward = zerovec	, right = zerovec, up = zerovec;
	

	void UpdateMatrices()
	{
		model = translate(model, -position);

		model = rotate(model, euler.x, vec3(1, 0, 0));
		model = rotate(model, euler.y, vec3(0, 1, 0));
		model = rotate(model, euler.z, vec3(0, 0, 1));
		
		model = glm::scale(model, scale);
	}

	WorldObject* wobj = this;
};

class Shader : public GLObject
{
public:

	cstring SourceCode = "";
	GLenum type = 0;

	Shader(cstring code, GLenum type)
	{
		this->SourceCode = code;
		this->type = type;
	}

	void CreateShader()
	{
		this->id = glCreateShader(this->type);
	}



	void LinkCode(cstring code)
	{
		SourceCode = code;
		glShaderSource(this->id, 1, &SourceCode, NULL);
	}

	void LinkCodeWithPath(cstring path)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::ifstream vShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(path);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			vShaderFile.close();
			vertexCode = vShaderStream.str();
			SourceCode = vertexCode.c_str();
			glShaderSource(this->id, 1, &SourceCode, NULL);
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
	}

	void Compile()
	{
		glCompileShader(this->id);
	}

	void Delete()
	{
		glDeleteShader(id);
	}
};

class ShaderProgram : public GLObject
{
public:
	vector<Shader> linkedShaders;
	bool initialized;

	void InitializeProgram(vector<Shader> shaders)
	{
		if (initialized)
		{
			glDeleteProgram(id);
			initialized = false;
		}

		this->id = glCreateProgram();
		for (int i = 0; i < shaders.size(); i++)
		{
			glAttachShader(this->id, shaders[i].id);
		}
		glLinkProgram(this->id);
		initialized = true;
	}


	void UseProgram() const
	{
		glUseProgram(this->id);
	}

	void setVec4(const std::string& name, const vec4& vect) const
	{
		glUseProgram(id);
		glUniform4f(glGetUniformLocation(this->id, name.c_str()), (GLfloat)vect.x, (GLfloat)vect.y, (GLfloat)vect.z, (GLfloat)vect.w);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUseProgram(id);
		glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Delete()
	{
		glDeleteProgram(id);
	}
};

class VertexArrayObject : protected GLObject
{
public:
	int size;
	VertexArrayObject()
	{
		size = 0;
	}

	void Generate(int size)
	{
		this->size = size;
		glGenVertexArrays(size, &this->id);
	}


	void Bind()
	{
		glBindVertexArray(this->id);
	}

	void Delete()
	{
		glDeleteVertexArrays(size, &id);
	}
};

class BufferObject : public GLObject
{
public:
	GLenum buffer = 0;
	GLenum drawType = 0;

	int size = 0;
	void* data = nullptr;

	BufferObject()
	{
		buffer = 0;
		drawType = 0;
		size = 0;
		data = nullptr;
	}

	void Generate(int size, GLenum buffer) 
	{
		this->size = size;
		this->buffer = buffer;
		glGenBuffers(1, &this->id);
	}

	void Bind(GLenum toWhat)
	{
		glBindBuffer(toWhat, this->id);
	}
	
	void Copy(GLenum buffer, void* data, int size, GLenum drawType)
	{
		this->data = data;
		this->drawType = drawType;
		this->buffer = buffer;
		this->size = size;

		glBindBuffer(buffer, this->id);
		glBufferData(buffer, this->size, data, drawType);
		PrintErrors();
	}

	void Delete()
	{
		glDeleteBuffers(size, &id);
	}
};

class Camera : public WorldObject
{
protected:

public:
	enum projectionMode { Perspective, Orthographic };
	float FieldOfView = 90, FarClip = 1000, NearClip = 0.01f;
	projectionMode ProjectionMode = projectionMode::Perspective;
	
	mat4 view = mat4(1), projection = mat4(1);

	//__declspec(property(get = getProjectionMatrix)) mat4 projection;


	static Camera* main;

	Camera(Camera::projectionMode mode, bool makeMain, float fov = 90, float fc = 1000, float nc = 0.01f)
	{
		this->ProjectionMode = mode;
		
		if (makeMain) main = this;
		this->FieldOfView = fov;
		this->FarClip = fc;
		this->NearClip = nc;
	}

	void UpdateCameraMatrices()
	{
		view = rotate(view, euler.x, vec3(0, 0, 1));
		view = rotate(view, euler.y, vec3(0, 1, 0));
		view = rotate(view, euler.z, vec3(1, 0, 0));

		view = translate(view, -position);
		projection = perspective(radians(FieldOfView), float(width / height), NearClip, FarClip);
	}
};

void ApplyPerspective(Camera source, ShaderProgram pro, WorldObject obj)
{
	pro.UseProgram();
	obj.UpdateMatrices();
	source.UpdateCameraMatrices();

	pro.setMat4("projection", source.projection);
	pro.setMat4("view", source.view);
	pro.setMat4("model", obj.model);
}

class Renderer
{
private:
	Renderer(){}

public:
	static void Draw(VertexArrayObject va, ShaderProgram pro, BufferObject bo, WorldObject wo);
};

class VertexAttribute
{
public:
	int position, size;
	GLenum type;
	GLboolean normalize;
	int stride;

	VertexAttribute(int position, int size, GLenum type, GLboolean normalize, int stride)
	{
		this->position = position;
		this->size = size;
		this->type = type;
		this->normalize = normalize;
		this->stride = stride;
		glVertexAttribPointer(position, size, type, normalize, stride, (void*)0);
		glEnableVertexAttribArray(position);
	}
};

class Texture : public GLObject
{
public:
	void* texData;
	int width, height, MipmapLevel;

	GLenum target, format, imageFormat, datatype;

	Texture()
	{
		glGenTextures(1, &id);
	}

	void GenerateTexture(GLenum target, int mipmaplevel, GLenum imageformat, int width, int height, GLenum format, GLenum datatype, void* data, bool makeMipmap = true)
	{
		glBindTexture(target, id);
		glTexImage2D(target, mipmaplevel, imageformat, width, height, 0, format, datatype, data);
		if (makeMipmap) glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(target, 0);
	}
};

class Cube : public WorldObject
{
public:
	VertexArrayObject* VAO = new VertexArrayObject();
	BufferObject* VBO = new BufferObject();
	Shader VertShader = Shader(vertexShaderSource, GL_VERTEX_SHADER), FragShader = Shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	ShaderProgram shaderProgram = ShaderProgram();



	void Initialize(int mode = 0)
	{
		if (mode == 0)
		{
			VertShader.CreateShader();
			VertShader.LinkCode(vertexShaderSource);
			VertShader.Compile();

			FragShader.CreateShader();
			FragShader.LinkCode(fragmentShaderSource);
			FragShader.Compile();

			shaderProgram.InitializeProgram({ VertShader, FragShader });

			

			float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};

			VAO->Generate(1);
			VAO->Bind();

			VBO->Generate(sizeof(vertices), GL_ARRAY_BUFFER);
			VBO->Bind(GL_ARRAY_BUFFER);
			VBO->Copy(GL_ARRAY_BUFFER, vertices, sizeof(vertices), GL_STATIC_DRAW);

			VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float));

			ResetState();
		}
	}

	void Draw(int mode = 0)
	{
		if (mode == 0)
		{
			VBO->Bind(GL_ARRAY_BUFFER);
			shaderProgram.UseProgram();
			VAO->Bind();
		}

		if (mode == 1)
		{
			shaderProgram.setVec4("color", vec4(1.0f, 0.3, 0.9f, 0.2f));
			Renderer::Draw(*VAO, shaderProgram, *VBO, *wobj);
		}
	}
};

class Triangle : public WorldObject
{
public:
	VertexArrayObject* VAO = new VertexArrayObject();
	BufferObject* VBO = new BufferObject();
	Shader VertShader = Shader(vertexShaderSource, GL_VERTEX_SHADER), FragShader = Shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	ShaderProgram shaderProgram = ShaderProgram();



	void Initialize(int mode = 0)
	{
		if (mode == 0)
		{
			VertShader.CreateShader();
			VertShader.LinkCode(vertexShaderSource);
			VertShader.Compile();

			FragShader.CreateShader();
			FragShader.LinkCode(fragmentShaderSource);
			FragShader.Compile();

			shaderProgram.InitializeProgram({ VertShader, FragShader });


			float vertices[] = {
				-0.5f, -0.5f, 0.0f, // left  
				0.5f, -0.5f, 0.0f, // right 
				0.0f,  0.5f, 0.0f,
			};


			VAO->Generate(1);
			VAO->Bind();

			VBO->Generate(sizeof(vertices), GL_ARRAY_BUFFER);
			VBO->Bind(GL_ARRAY_BUFFER);
			VBO->Copy(GL_ARRAY_BUFFER, vertices, sizeof(vertices), GL_STATIC_DRAW);

			VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
			
			ResetState();
		}
	}

	void Draw(int mode = 0)
	{
		if (mode == 0)
		{
			VBO->Bind(GL_ARRAY_BUFFER);
			shaderProgram.UseProgram();
			VAO->Bind();
		}

		if (mode == 1)
		{
			shaderProgram.setVec4("color", vec4(0.4f, 0.9f, 0.1f, 0.5f));
			Renderer::Draw(*VAO, shaderProgram, *VBO, *wobj);
		}
	}
};


#endif