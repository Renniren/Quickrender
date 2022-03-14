#ifndef QUICKRENDER_MAIN_HEADER
#define QUICKRENDER_MAIN_HEADER

#ifndef QR_SETTINGS
#define QR_SETTINGS
#define QR_USE_ASSIMP
#define STB_IMAGE_IMPLEMENTATION

int width = 800, height = 600;
const char* const WINDOW_NAME = "test";

#endif

#include <glad.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <glfw3.h>
#include <direct.h>
#include <fstream>
#include <glm.hpp>
#include <sstream>
#include <iostream>
#include <stb_image.h>
#include <quickerror.h>
#include <quickdefines.h>
#include <quickmacros.h>
#include <assimp/scene.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef  QR_USE_ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#endif

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

char* GetWorkingDirectory()
{
	char cCurrentPath[FILENAME_MAX];

	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		return nullptr;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '/0'; /* not really required */
	return cCurrentPath;
}

const string TEXTURES_DIRECTORY = string(GetWorkingDirectory()) + string("\\textures\\");
const string SHADERS_DIRECTORY = string(GetWorkingDirectory()) + string("\\shaders\\");

#ifndef QR_PRIMITIVES
#define QR_PRIMITIVES
float PRIMITIVE_CUBE[] = {
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
float PRIMITIVE_TRIANGLE[] = {
				-0.5f, -0.5f, 0.0f, // left  
				0.5f, -0.5f, 0.0f, // right 
				0.0f,  0.5f, 0.0f,
};
#endif

void ResetState()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_1D, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_3D, 0);

	glUseProgram(0);
	glBindVertexArray(0);
}

void ViewResizeCallback(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
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
	stbi_set_flip_vertically_on_load(true);
	
	// glfw: initialize and configure
   // ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(width, height, WINDOW_NAME, NULL, NULL);
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

	vec3 forward = zerovec, right = zerovec, up = zerovec, direction = zerovec;
	
	void UpdateDirections(bool forCamera = false)
	{
		if (forCamera)
		{
			forward.x = cos(euler.x) * sin(-euler.y);
			forward.y = sin(euler.x);
			forward.z = cos(euler.x) * cos(-euler.y);
		}
		else
		{
			forward.x = cos(euler.x) * sin(euler.y);
			forward.y = sin(euler.x);
			forward.z = cos(euler.x) * cos(euler.y);
		}

		right.x = cos(euler.y);
		right.y = 0;
		right.z = sin(euler.y);

		up = cross(forward, right);
	}

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

	void LinkCodeWithPath(string path)
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
			std::cout << "Could not successfully read the shader's file: " << e.what() << std::endl;
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

	void setFloat(const string name, const float& number) const
	{
		glUseProgram(id);
		glUniform1f(glGetUniformLocation(this->id, name.c_str()), number);
	}

	void setVec4(const string& name, const vec4& vect) const
	{
		glUseProgram(id);
		glUniform4f(glGetUniformLocation(this->id, name.c_str()), (GLfloat)vect.x, (GLfloat)vect.y, (GLfloat)vect.z, (GLfloat)vect.w);
	}

	void setMat4(const string& name, const glm::mat4& mat) const
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

	static Camera* main;

	Camera() 
	{
		this->ProjectionMode = projectionMode::Perspective;
		this->FieldOfView = 90;
		this->FarClip = 1000;
		this->NearClip = 0.01f;

		this->view = mat4(1);
		this->projection = mat4(1);
	}

	Camera(Camera::projectionMode mode, bool makeMain, float fov = 90, float fc = 1000, float nc = 0.01f)
	{
		this->ProjectionMode = mode;
		
		if (makeMain) main = this;
		this->FieldOfView = fov;
		this->FarClip = fc;
		this->NearClip = nc;

		this->view = mat4(1);
		this->projection = mat4(1);	
	}

	void DoInput(GLFWwindow* window, float deltaTime)
	{
		UpdateDirections(true);
		float speed = 0.9f;
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			position -= (forward * speed) * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_S))
		{
			position += (forward * speed) * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_A))
		{
			position -= (right * speed) * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_D))
		{
			position += (right * speed) * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_E))
		{
			position.y += speed * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_Q))
		{
			position.y -= speed * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_C))
		{
			euler.y += speed * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_Z))
		{
			euler.y -= speed * deltaTime;
		}
	}

	void UpdateCameraMatrices()
	{
		view = rotate(view, euler.x, vec3(0, 0, 1));
		view = rotate(view, euler.y, vec3(0, 1, 0));
		view = rotate(view, euler.z, vec3(1, 0, 0));

		view = translate(view, -position);
		projection = perspective(radians(FieldOfView), float((width * 0.72) / (height * 0.72)), NearClip, FarClip);
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
	static void DrawLights(ShaderProgram pro)
	{

	}

	static void Draw(VertexArrayObject va, ShaderProgram pro, BufferObject bo, WorldObject wo)
	{
		va.Bind();
		bo.Bind(bo.buffer);
		pro.UseProgram();
		ApplyPerspective(*Camera::main, pro, wo);

		glDrawArrays(GL_TRIANGLES, 0, bo.size);
		ResetState();
	}

};

class VertexAttribute
{
public:
	int position, size;
	GLenum type;
	GLboolean normalize;
	int stride;

	VertexAttribute(int position, int size, GLenum type, GLboolean normalize, int stride, int offset = 0)
	{
		this->position = position;
		this->size = size;
		this->type = type;
		this->normalize = normalize;
		this->stride = stride;
		glVertexAttribPointer(position, size, type, normalize, stride, (void*)offset);
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
		width = 0;
		height = 0;
		MipmapLevel = 0;
		target = 0;
		format = 0;
		imageFormat = 0;
		datatype = 0;
		texData = nullptr;
	}

	void GenerateTexture(GLenum target, int mipmaplevel, GLenum imageformat, int width, int height, GLenum format, GLenum datatype, void* data, bool makeMipmap = true)
	{
		this->target = target;
		this->MipmapLevel = mipmaplevel;
		this->format = imageformat;
		this->format = format;

		this->height = height;
		this->width = width;

		this->texData = data;



		glBindTexture(target, id);
		glTexImage2D(target, mipmaplevel, imageformat, width, height, 0, format, datatype, data);
		if (makeMipmap) glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(target, 0);
	}

	void Bind(GLenum target)
	{
		glBindTexture(target, id);
	}
};

Texture* LoadTexture(string path, GLenum target)
{
	Texture* tex = new Texture();
	int height, width, numChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

	GLenum format = GL_RGB;

	if (!data)
	{
		cout << "Failed to load texture with given path: " << path << endl;
		panic
	}

	if (numChannels == 4) format = GL_RGBA;

	glCall(tex->GenerateTexture(target, 0, GL_RGB, width, height, format, GL_UNSIGNED_BYTE, data, true));
	cout << numChannels << endl;
	stbi_image_free(data);
	return tex;
}

class Cube : public WorldObject
{
public:
	VertexArrayObject* VAO = new VertexArrayObject();
	BufferObject* VBO = new BufferObject();
	Shader VertShader = Shader(vertexShaderSource, GL_VERTEX_SHADER), FragShader = Shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	ShaderProgram shaderProgram = ShaderProgram();
	Texture* texture;


	void Initialize(int mode = 0)
	{
		if (mode == 0)
		{
			VertShader.CreateShader();
			cout << SHADERS_DIRECTORY + string("vertex.glsl") << endl;
			VertShader.LinkCodeWithPath(SHADERS_DIRECTORY + string("vertexTextured.glsl"));
			VertShader.Compile();

			FragShader.CreateShader();
			FragShader.LinkCodeWithPath(SHADERS_DIRECTORY + string("fragmentTextured.glsl"));
			FragShader.Compile();

			shaderProgram.InitializeProgram({ VertShader, FragShader });
			shaderProgram.setVec4("color", vec4(1));

			VAO->Generate(1);
			VAO->Bind();

			VBO->Generate(sizeof(PRIMITIVE_CUBE), GL_ARRAY_BUFFER);
			VBO->Bind(GL_ARRAY_BUFFER);
			VBO->Copy(GL_ARRAY_BUFFER, PRIMITIVE_CUBE, sizeof(PRIMITIVE_CUBE), GL_STATIC_DRAW);

			texture = LoadTexture(TEXTURES_DIRECTORY + string("dirt.jpg"), GL_TEXTURE_2D);

			VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float));
			VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (3 * sizeof(float)));
			PrintErrors();
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
			UpdateDirections();
			texture->Bind(GL_TEXTURE_2D);
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
			UpdateDirections();
			shaderProgram.setVec4("color", vec4(0.4f, 0.9f, 0.1f, 0.5f));
			Renderer::Draw(*VAO, shaderProgram, *VBO, *wobj);
		}
	}
};

struct Color
{
public: 
	float r, g, b, a;

	Color operator = (Color& col)
	{
		this->r = col.r;
		this->g = col.g;
		this->b = col.b;
		this->a = col.a;
	}

	Color operator + (Color& col)
	{
		this->r += col.r;
		this->g += col.g;
		this->b += col.b;
		this->a += col.a;
	}

	Color operator - (Color& col)
	{
		this->r -= col.r;
		this->g -= col.g;
		this->b -= col.b;
		this->a -= col.a;
	}

	static Color red;
	static Color blue;
	static Color white;
	static Color green;
	static Color transparent;
};

Color Color::red = { 1, 0, 0, 1 };
Color Color::blue = { 0, 0, 1, 1 };
Color Color::white = { 1, 1, 1, 1 };
Color Color::green = { 0, 1, 0, 1 };
Color Color::transparent = { 1, 1, 1, 0 };

Camera* Camera::main = nullptr;
GLFWwindow* window;
vector<GLObject> GLObject::objects = vector<GLObject>();

float deltaTime = 0, lastFrame = 0;

class Light : public WorldObject
{
public:
	Color color;
	float strength = 1, range = 5;
};

class Mesh
{
public:
	struct Vertex
	{
	public:
		vec3 Position, Normal;
		vec2 TextureCoordinates;
	};

	struct Texture
	{
	public:
		uint id;
		string type;
	};

	BufferObject VertexBuffer, ElementBuffer;
	VertexArrayObject VAO;

	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void setup()
	{
		VAO = VertexArrayObject();
		VAO.Generate(1);
		
		VertexBuffer = BufferObject();
		VertexBuffer.Generate(1, GL_ARRAY_BUFFER);

		ElementBuffer = BufferObject();
		ElementBuffer.Generate(1, GL_ELEMENT_ARRAY_BUFFER);


	}
};

#ifdef QR_USE_ASSIMP

#endif // QR_USE_ASSIMP



#endif