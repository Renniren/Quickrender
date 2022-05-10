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
#include <assimp/scene.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef  QR_USE_ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#endif

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
const string MODELS_DIRECTORY = string(GetWorkingDirectory()) + string("\\models\\");


#ifndef QUICKRENDER_MACROS
#define QUICKRENDER_MACROS

//this exists purely because I'm too lazy to type out certain shit
#define panic __debugbreak();

void PrintErrors()
{
	using namespace std;
	GLenum err = glGetError();

	if (err == 0) return;

	switch (err)
	{
	case 1280:
		cout << "Error: 1280 - Invalid enum. Perhaps you used an enum where it shouldn't be?" << endl;
		break;

	case 1281:
		cout << "Error: 1281 - Invalid value. Perhaps you passed the wrong type of data somewhere?" << endl;
		break;

	case 1282:
		cout << "Error: 1282 - Invalid operation." << endl;
		break;

	case 1283:
		cout << "Error: 1283 - stack overflow." << endl;
		break;

	case 1284:
		cout << "Error: 1284 - stack... underflow? How the fuck did you even do that?" << endl;
		break;

	case 1285:
		cout << "Error: 1285 - OpenGL ran out of memory." << endl;
		break;

	case 1286:
		cout << "Error: 1286 - Invalid framebuffer read/write." << endl;
		break;
	}

}


#define uint unsigned int
#define onevec vec3(1,1,1)
#define zerovec vec3(0,0,0)
#define cstring const char*

static bool glPrintErrors(const char* function, const char* file, int line)
{
	using namespace std;
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		while (err != GL_NO_ERROR)
		{
			const char* strerror = "";
			switch (err)
			{
			case 1280:
				strerror = "Invalid enum";
				break;
			case 1281:
				strerror = "Invalid value";
				break;
			case 1282:
				strerror = "Invalid operation";
				break;


			case 1285:
				strerror = "OpenGL ran out of memory";
				break;
			}

			cout << strerror << " in function " << function << " in file " << file << " on line " << line << endl;
			return false;
		}
	}
	return true;
}


#define STOP_ON_FAILURE(x) if ((!x)) __debugbreak();

#define glCall(x)\
	x;\
	STOP_ON_FAILURE(glPrintErrors(#x, __FILE__, __LINE__));\

#endif

#ifndef QR_PRIMITIVES
#define QR_PRIMITIVES

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

void glResetState()
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

#define InitializeRenderer window = glSetup()

inline void printvec3(vec3 op)
{
	cout << "(" << op.x << ", " << op.y << ", " << op.z << ")" << endl;
}

inline vec3 negatevec3_y(vec3 op) { vec3 ne = op; ne.y = -ne.y; return ne; }

void qclamp(int* value, int lower, int upper)
{
	if (*value > upper) *value = upper;
	if (*value < lower) *value = lower;
}

float qclamp(float value, float lower, float upper)
{
	if (value > upper) value = upper;
	if (value < lower) value = lower;
	return value;
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

class WorldObject : public GLObject
{
public:

	vec3 position = zerovec;
	quat rotation = quat(0, 0, 0, 0);
	mat4 rotationm = mat4(1.0f);
	vec3 scale = zerovec;

	mat4 model = mat4(1.0f);

	vec3 forward = zerovec, right = zerovec, up = zerovec;

	bool BelongsToCamera = false;

	void UpdateDirections(bool cam = false)
	{
		/*if (cam)
		{
			forward.x = cos(rotation.x) * sin(-rotation.y);
			forward.y = sin(rotation.z);
			forward.z = cos(rotation.x) * cos(-rotation.y);
		}
		else
		{
			forward.x = cos(-rotation.x) * sin(rotation.y);
			forward.y = sin(rotation.x);
			forward.z = cos(-rotation.x) * cos(rotation.y);
		}

		right.x = cos(rotation.y);
		right.y = 0;
		right.z = sin(rotation.y);

		up = cross(forward, right);*/

		forward = vec3(
			cos(rotation.x) * sin(rotation.y),
			sin(rotation.x),
			cos(rotation.x) * cos(rotation.y)
		);

		right = glm::vec3(
			sin(rotation.y - 3.14f / 2.0f),
			0,
			cos(rotation.y - 3.14f / 2.0f)
		);

		up = cross(forward, right);

	}

	void UpdateMatrices()
	{
		UpdateDirections();
		model = translate(model, position);
		model = lookAt(position, position + forward, up);

		/*if (BelongsToCamera)
		{
			model = translate(model, position);
			model = rotate(model, rotation.x, right);
			model = rotate(model, rotation.y, up);
			model = rotate(model, rotation.z, forward);
		}
		else
		{
			model = translate(model, position);
			model = rotate(model, rotation.x, right);
			model = rotate(model, rotation.y, up);
			model = rotate(model, rotation.z, forward);
		}*/

		model = glm::scale(model, scale);
	}

	WorldObject* wobj = this;
};

class Camera : public WorldObject
{
public:
	vec3 direction;

	enum projectionMode { Perspective, Orthographic };
	float FieldOfView = 90, FarClip = 1000, NearClip = 0.01f;
	Camera::projectionMode ProjectionMode = Camera::projectionMode::Perspective;

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
		//UpdateDirections(true);
		float speed = 0.9f;
		float sens = 1;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, width / 2, height / 2);
		vec2 center = vec2(1);
		center.x = width / 2;
		center.y = height / 2;

		rotation.y -= (center.x - xpos) * sens * deltaTime;
		rotation.x += (center.y - ypos) * sens * deltaTime;

		float limit = 80;
		rotation.x = qclamp(rotation.x, -limit, limit);
		
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			position += (direction * speed) * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_S))
		{
			position -= (direction * speed) * deltaTime;
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
	}

	void UpdateCameraMatrices()
	{
		direction = vec3(
			cos(rotation.x) * sin(rotation.y),
			sin(rotation.x),
			cos(rotation.x) * cos(rotation.y)
		);

		right = glm::vec3(
			sin(rotation.y - 3.14f / 2.0f),
			0,
			cos(rotation.y - 3.14f / 2.0f)
		);
		
		up = cross(right, direction);

		view = lookAt(
			position,
			position + direction,
			up
		);

		projection = perspective(radians(FieldOfView), float((width * 0.72) / (height * 0.72)), NearClip, FarClip);
	}
};

class Shader : public GLObject
{
public:

	cstring SourceCode = "";
	GLenum type = 0;

	Shader(){}

	Shader(cstring code, GLenum type)
	{
		this->SourceCode = code;
		this->type = type;
	}
	
	void SetType(GLenum t)
	{
		this->type = t;
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

	void InitializeProgram(const vector<Shader>& shaders)
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
			linkedShaders.push_back(shaders[i]);
		}
		glLinkProgram(this->id);
		initialized = true;
	}

	void UseProgram() const
	{
		glUseProgram(this->id);
	}

	//There should be more of these uniform setters. I write them as I go along, really.

	void setFloat(const string& name, const float& number) const
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

	void Unbind()
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

	static void UnbindBuffers(GLenum target)
	{
		glBindBuffer(target, 0);
	}

	void UnbindBuffer()
	{
		glBindBuffer(this->buffer, 0);
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
	Renderer() {}

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

		if(bo.buffer == GL_ARRAY_BUFFER) glDrawArrays(GL_TRIANGLES, 0, bo.size);
		if(bo.buffer == GL_ELEMENT_ARRAY_BUFFER) glDrawElements(GL_TRIANGLES, bo.size, GL_FLOAT, 0);
		glResetState();
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
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, size, type, normalize, stride, (void*)offset);
	}
};

class Texture : public GLObject
{
public:
	void* texData;
	int width, height, MipmapLevel;
	vector<string> tags;

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
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

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
			glResetState();
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

			glResetState();
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
vector<Texture> ActiveTextures = vector<Texture>();

float deltaTime = 0, lastFrame = 0;

class Light : public WorldObject
{
public:
	enum lightType { Point, Spotlight, Sun };
	Light::lightType Type = Light::lightType::Point;

	Color color = Color::white;
	float strength = 1, range = 5;
};

#ifdef QR_USE_ASSIMP

#endif

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = true)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

class Mesh
{
public:
	struct Vertex
	{
	public:
		vec3 Position, Normal, Tangent, BiTangent;
		vec2 TextureCoordinates;

		int BoneIDs[4], Weights[4];
	};

	struct Texture
	{
	public:
		uint id;
		string type, path;
	};
	
	BufferObject VertexBuffer, ElementBuffer;
	VertexArrayObject VAO;

	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setup();
	}

	void setup()
	{
		VAO = VertexArrayObject();
		VAO.Generate(1);
		VAO.Bind();

		VertexBuffer = BufferObject();
		VertexBuffer.Generate(1, GL_ARRAY_BUFFER);
		VertexBuffer.Copy(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

		ElementBuffer = BufferObject();
		ElementBuffer.Generate(1, GL_ELEMENT_ARRAY_BUFFER);
		ElementBuffer.Copy(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(uint), GL_STATIC_DRAW);

		VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		VertexAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal));
		VertexAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TextureCoordinates));

		VAO.Unbind();
	}

	void draw(ShaderProgram shader)
	{
		uint spec_number = 1, diffuse_number = 1;
		for (uint i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); 
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuse_number++);
			else if (name == "texture_specular")
				number = std::to_string(spec_number++);

			shader.setFloat((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		VAO.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

vector<Mesh::Texture> ActiveMeshTextures = vector<Mesh::Texture>();

class Model : public WorldObject
{
public:
	vector<Mesh> meshes;
	string directory;

	ShaderProgram program = ShaderProgram();
	Shader vert = Shader(), frag = Shader();
	
	void LoadModel(string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "Assimp Error: " << import.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	Model(){}

	Model(cstring path, cstring directory)
	{
		this->directory = directory;

		vert.CreateShader();
		vert.SetType(GL_VERTEX_SHADER);
		vert.LinkCodeWithPath(SHADERS_DIRECTORY + string("vertexTextured.glsl"));
		vert.Compile();

		frag.CreateShader();
		frag.SetType(GL_FRAGMENT_SHADER);
		frag.LinkCodeWithPath(SHADERS_DIRECTORY + string("fragmentTextured.glsl"));
		frag.Compile();

		program.InitializeProgram({ frag, vert });
		LoadModel(string(path));

	}

	void Draw()
	{
		for (unsigned int i = 0; i < meshes.size(); i++) 
		{
			glCall(Renderer::Draw(meshes[i].VAO, this->program, meshes[i].ElementBuffer, *wobj));
		}
	}

private:
	// model data

	void processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<Mesh::Vertex> vertices;
		vector<unsigned int> indices;
		vector<Mesh::Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TextureCoordinates = vec;
			}
			else
				vertex.TextureCoordinates = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (uint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process material

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Mesh::Texture> specularMaps = loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Mesh::Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < ActiveMeshTextures.size(); j++)
			{
				if (std::strcmp(ActiveMeshTextures[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(ActiveMeshTextures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Mesh::Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				ActiveMeshTextures.push_back(texture); // add to loaded textures
			}
		}
		return textures;
	}
};

class WorldMesh
{
public:
	Model model;
	ShaderProgram program = ShaderProgram();
	Shader vert = Shader(), frag = Shader();
	WorldObject transform = WorldObject();

	WorldMesh(string path)
	{
		vert.CreateShader();
		vert.SetType(GL_VERTEX_SHADER);
		vert.LinkCodeWithPath(SHADERS_DIRECTORY + string("vertexTextured.glsl"));
		vert.Compile();

		frag.CreateShader();
		frag.SetType(GL_FRAGMENT_SHADER);
		frag.LinkCodeWithPath(SHADERS_DIRECTORY + string("fragmentTextured.glsl"));
		frag.Compile();

		program.InitializeProgram({ frag, vert });
		//model = Model(path.c_str());
	}

	void render()
	{
		for (int i = 0; i < model.meshes.size(); i++)
		{
			glCall(Renderer::Draw(model.meshes[i].VAO, program, model.meshes[i].VertexBuffer, transform));
		}
	}
};


#endif