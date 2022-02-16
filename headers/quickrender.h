#ifndef QUICKRENDER_MAIN_HEADER
#define QUICKRENDER_MAIN_HEADER

#pragma once
#include <glad.h>
#include <string>
#include <vector>
#include <glfw3.h>
#include <fstream>
#include <glm.hpp>
#include <sstream>
#include <iostream>
#include <quickerror.h>
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

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";



using namespace std;
using namespace glm;
#define uint unsigned int

int width = 800, height = 600;

void ViewResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* Setup()
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glfwSetFramebufferSizeCallback(window, ViewResizeCallback);
	glViewport(0, 0, width, height);

	return window;
}

class GLObject
{
	
public:

	uint id = -1;

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
};

class WorldObject 
{
public:

	vec3 position = zerovec;
	vec3 euler = zerovec;
	vec3 scale = zerovec;

	mat4 model = mat4(1.0f);
	

	void UpdateMatrices()
	{
		model = translate(model, -position);
		model = glm::scale(model, scale);

		model = rotate(model, euler.x, vec3(1, 0, 0));
		model = rotate(model, euler.y, vec3(0, 1, 0));
		model = rotate(model, euler.z, vec3(0, 0, 1));
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

		int success;
		char infoLog[512];
		glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "Shader compilation failed: \n" << infoLog << std::endl;
		}
	}
};

class ShaderProgram : protected GLObject
{
public:
	vector<Shader> linkedShaders;

	ShaderProgram()
	{

	}

	void InitializeProgram(vector<Shader> shaders)
	{
		this->id = glCreateProgram();
		for (int i = 0; i < shaders.size(); i++)
		{
			glAttachShader(this->id, shaders[i].id);
		}
		glLinkProgram(this->id);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void UseProgram()
	{
		glUseProgram(this->id);
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
};

class BufferObject : public GLObject
{
public:
	GLenum buffer = 0;
	GLenum drawType = 0;

	int size = 0;
	void* data = 0;

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
		glGenBuffers(size, &this->id);
	}

	void Bind(GLenum toWhat)
	{
		glCall(glBindBuffer(toWhat, this->id));
	}
	
	void Copy(GLenum buffer, void* data, int size, GLenum drawType)
	{
		this->data = data;
		this->drawType = drawType;
		this->buffer = buffer;
		this->size = size;

		glCall(glBindBuffer(buffer, this->id));
		glCall(glBufferData(buffer, this->size, data, drawType));
		PrintErrors();
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

	Camera(projectionMode mode, bool makeMain, float fov = 90, float fc = 1000, float nc = 0.01f)
	{
		this->ProjectionMode = mode;
		
		if (makeMain) main = this;
		this->FieldOfView = fov;
		this->FarClip = fc;
		this->NearClip = nc;
	}

	void UpdateCameraMatrices()
	{
		view = translate(view, -position);
		view = rotate(view, euler.x, vec3(0, 0, 1));
		view = rotate(view, euler.y, vec3(0, 1, 0));
		view = rotate(view, euler.z, vec3(1, 0, 0));

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

class Triangle : public WorldObject
{
public:
	VertexArrayObject* VAO = new VertexArrayObject();
	BufferObject* VBO = new BufferObject();
	Shader VertShader = Shader(vertexShaderSource, GL_VERTEX_SHADER), FragShader = Shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	ShaderProgram shaderProgram = ShaderProgram();

	void Initialize()
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
		0.0f,  0.5f, 0.5f,


		};


		VAO->Generate(1);
		VAO->Bind();

		VBO->Generate(sizeof(vertices), GL_ARRAY_BUFFER);
		VBO->Bind(GL_ARRAY_BUFFER);
		VBO->Copy(GL_ARRAY_BUFFER, vertices, sizeof(vertices), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Draw()
	{
		VBO->Bind(GL_ARRAY_BUFFER);
		shaderProgram.UseProgram();
		VAO->Bind();
	}
};


#endif