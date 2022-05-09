#include <quickrender.h>

int main()
{
	InitializeRenderer;
	float speed = 0.9f;
	

	Camera cam = Camera(Camera::projectionMode::Perspective, true);
	Triangle tri = Triangle();
	Cube tri2 = Cube();

	cam.FieldOfView = 90;
	cam.FarClip = 9000.0f;
	cam.NearClip = 0.01f;

	cam.position = vec3(0, 0, 5);
	//cam.rotation.y = -180;
	cam.UpdateCameraMatrices();

	tri.Initialize(0);
	tri.rotation = vec3(0, 0, 0);
	tri.position = vec3(0, 1, 0);
	tri.scale = vec3(1, 1, 1);

	tri2.Initialize(0);
	tri2.position = vec3(0, 3, 0);
	tri2.scale = onevec;
	tri2.rotation = vec3(0, 0, 0);

	Model mesh = Model((MODELS_DIRECTORY + "backpack\\backpack.obj").c_str(), (MODELS_DIRECTORY + "backpack\\").c_str());
	mesh.scale = vec3(0.01f, 0.01f, 0.01f);
	mesh.position = vec3(0.0f, 0.0f, -6);
	
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
			tri.rotation.z += speed * deltaTime;
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
			tri2.rotation.y -= speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			tri2.rotation.y += speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_T)) printvec3(cam.position);

		//mesh.Draw();
		if (glfwGetKey(window, GLFW_KEY_T))
		{
			tri2.rotation.z -= speed * deltaTime;
		}

		cam.UpdateCameraMatrices();
		cam.DoInput(window, deltaTime);
		tri.Draw(1);
		tri2.Draw(1);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Cleanup();
	return 0;
}
