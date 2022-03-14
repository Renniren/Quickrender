#define _CRTDBG_MAP_ALLOC
#include "quickrender.h"
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;


//declarations


int main()
{
    float speed = 0.9f;
    GLFWwindow* window = glSetup();

    Camera cam = Camera(Camera::projectionMode::Perspective, true);
    Triangle tri = Triangle();
    Cube tri2 = Cube();

    cam.FieldOfView = 90;
    cam.FarClip = 9000.0f;
    cam.NearClip = 0.01f;

    cam.position = vec3(0, -3, 0);

    tri.Initialize(0);
    tri.euler = vec3(0, 0, 0);
    tri.position = vec3(0, 1, 0);
    tri.scale = vec3(1, 1, 1);
    cam.position = vec3(0, 0, 5);

    tri2.Initialize(0);
    tri2.position = vec3(0, 3, 0);
    tri2.scale = onevec;
    tri2.euler = vec3(0, 0, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);



    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       float currentFrame = static_cast<float>(glfwGetTime());
       deltaTime = currentFrame - lastFrame;
       lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_R))
        {
            tri.euler.z += speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_UP))
        {
            tri2.position += tri2.forward * speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN))
        {
            tri2.position -= tri2.forward * speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            tri2.euler.y -= speed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT))
        {
            tri2.euler.y += speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_T))
        {
            tri2.euler.z -= speed * deltaTime;
        }


        cam.DoInput(window, deltaTime);
        tri.Draw(1);
        tri2.Draw(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Cleanup();
    return 0;
}
