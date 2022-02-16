#include <quickrender.h>
using namespace std;


Camera* Camera::main = nullptr;



GLFWwindow* window;

int main()
{
    window = Setup();

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