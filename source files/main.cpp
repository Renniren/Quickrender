#define _CRTDBG_MAP_ALLOC
#include <quickrender.h>
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;


//declarations
float deltaTime = 0, lastFrame = 0;
Camera* Camera::main = nullptr;
GLFWwindow* window;

vector<GLObject> GLObject::objects = vector<GLObject>();


void Renderer::Draw(VertexArrayObject va, ShaderProgram pro, BufferObject bo, WorldObject wo)
{
    va.Bind();
    bo.Bind(bo.buffer);
    pro.UseProgram();

    glDrawArrays(GL_TRIANGLES, 0, bo.size);

    ApplyPerspective(*Camera::main, pro, wo);
    ResetState();
}

int main()
{
    MSetupMemoryChecks();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();


    glfwSetFramebufferSizeCallback(window, ViewResizeCallback);
    glViewport(0, 0, width, height);

    Camera cam = Camera(Camera::projectionMode::Perspective, true);
    Triangle tri = Triangle();
    Cube tri2 = Cube();

    cam.FieldOfView = 90;
    cam.FarClip = 9000.0f;
    cam.NearClip = 0.01f;

    cam.position = vec3(0, -3, 0);

    tri.Initialize(0);
    tri.euler = vec3(90, 0, 0);
    tri.position = vec3(0, 1, 0);
    tri.scale = vec3(1, 1, 1);
    cam.position = vec3(0, 0, 5);
    
    tri2.Initialize(0);
    tri2.position = vec3(0, 3, 0);
    tri2.scale = onevec;
    tri2.euler = vec3(90, 0, 0);

    glEnable(GL_DEPTH_TEST);

    float speed = 0.9f;

    while (!glfwWindowShouldClose(window))
    {
       float currentFrame = static_cast<float>(glfwGetTime());
       deltaTime = currentFrame - lastFrame;
       lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            cam.position.z -= speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_S))
        {
            cam.position.z += speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_A))
        {
            cam.position.x -= speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            cam.position.x += speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_E))
        {
            cam.position.y -= speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            cam.position.y += speed * deltaTime;
        }

        tri.Draw(1);
        
        tri2.Draw(1);
        PrintErrors();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    Cleanup();
    MCheckMemory();
    return 0;
}