#include <quickrender.h>
using namespace std;


Camera* Camera::main = nullptr;


void ViewResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, ViewResizeCallback);
    glViewport(0, 0, width, height);

    Camera cam = Camera(Camera::projectionMode::Perspective, true);
    Triangle tri = Triangle();

    cam.FieldOfView = 90;
    cam.FarClip = 9000.0f;
    cam.NearClip = 0.01f;

    tri.Initialize();
    tri.position = vec3(0, 0, -5);
    tri.scale = vec3(1, 1, 1);
    cam.position = vec3(0, 0, 5);

    float deltaTime = 0, lastFrame = 0;

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
       float currentFrame = static_cast<float>(glfwGetTime());
       deltaTime = currentFrame - lastFrame;
       lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            cam.position.z -= 0.05f;
        }

        if (glfwGetKey(window, GLFW_KEY_S))
        {
            cam.position.z += 0.05f;
        }

        if (glfwGetKey(window, GLFW_KEY_A))
        {
            cam.position.x -= 0.05f;
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            cam.position.x += 0.05f;
        }

        if (glfwGetKey(window, GLFW_KEY_E))
        {
            cam.euler.y -= 0.05f;
        }

        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            cam.euler.y += 0.05f;
        }





        //tri.euler.z += 0.05f;
        //tri.euler.x += 0.05f;
        //tri.euler.y += 0.05f;

        cam.view = lookAt(cam.position, tri.position, vec3(0, 1, 0));

        tri.Draw();
        ApplyPerspective(cam, tri.shaderProgram, (WorldObject)tri);

        glCall(glDrawArrays(GL_TRIANGLES, 0, 4096));
        PrintErrors();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

    //cum

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);*/

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
}