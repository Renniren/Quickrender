#include <quickrender.h>

WindowRuntime::WindowRuntime(int w, int h)
{
	this->height = h;
	this->width = w;
}

void WindowRuntime::UpdateWindow()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

WindowRuntime* WindowRuntime::InitializeWindow(int w, int h, const char* WindowName)
{
    using namespace std;
    WindowRuntime* wr = new WindowRuntime(w, h);
    
    
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //tell it what major and minor versions of OpenGL it needs to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //tell it what profile to use, responsible for what functions it should include 
	//using the core profile allows us to use more modernized functions
	wr->window = glfwCreateWindow(w, h, WindowName, NULL, NULL);
	if (wr->window == NULL)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(wr->window);

	//load glad
	//it configures openGL for us
	gladLoadGL();

    glViewport(0, 0, w, h);


    return wr;
}