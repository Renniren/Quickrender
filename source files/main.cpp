#include <quickrender.h>
using namespace std;


//declarations
Camera* Camera::main = nullptr;
GLFWwindow* window;

vector<pair<BufferObject, GLenum>> Renderer::Buffers = vector<pair<BufferObject, GLenum>>();
ShaderProgram Renderer::Program = ShaderProgram();
VertexArrayObject Renderer::VertexArray = VertexArrayObject();

void Renderer::CopyToBuffer(void* data, int dataSize, GLenum buffer, GLenum usage)
{
    for (int i = 0; i < Buffers.size(); i++)         
    {
        if (Buffers[i].second == buffer)
        {
            if(Buffers[i].first.id == 0)Buffers[i].first.Generate(dataSize, buffer);
            Buffers[i].first.Copy(buffer, data, dataSize, usage);
        }
    }
}

void Renderer::BindBuffers()
{
    for (int i = 0; i < Buffers.size(); i++)
    {
        Buffers[i].first.Bind(Buffers[i].first.buffer);
    }
}



void Renderer::Draw(VertexArrayObject va, ShaderProgram pro, BufferObject bo)
{
    va.Bind();
    bo.Bind(bo.buffer);
    pro.UseProgram();

    glDrawArrays(GL_TRIANGLES, 0, 4096);

    ResetState();
}

void Renderer::Initialize()
{
    Buffers.push_back(pair<BufferObject, GLenum>(BufferObject(), GL_ARRAY_BUFFER));
    Buffers.push_back(pair<BufferObject, GLenum>(BufferObject(), GL_ELEMENT_ARRAY_BUFFER));
    VertexArray = VertexArrayObject();
    VertexArray.Generate(1);

    Shader VertShader = Shader(vertexShaderSource, GL_VERTEX_SHADER), FragShader = Shader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    VertShader.CreateShader();
    VertShader.LinkCode(vertexShaderSource);
    VertShader.Compile();

    FragShader.CreateShader();
    FragShader.LinkCode(fragmentShaderSource);
    FragShader.Compile();

    Program.InitializeProgram({ VertShader, FragShader });

}


float deltaTime = 0, lastFrame = 0;



int main()
{
    window = Setup();
    Renderer::Initialize();
    
    //setting the scene, making a camera and a triangle
    Camera cam = Camera(Camera::projectionMode::Perspective, true);
    Triangle tri = Triangle();
    Triangle tri2 = Triangle();

    cam.FieldOfView = 90;
    cam.FarClip = 9000.0f;
    cam.NearClip = 0.01f;

    tri.Initialize(0);
    tri.position = vec3(0, 1, 0);
    tri.scale = vec3(18, 18, 18);
    cam.position = vec3(0, 0, 5);
    
    tri2.Initialize(0);
    tri2.scale = vec3(1, 1, 9);
    tri2.euler = vec3(0, 0, 0);
    tri2.position = vec3(0, -1, 0);
    

    //enabling special shit for rendering, only depth testing is needed for now
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


        //can't see anything unless this line is included, I find this to be strange
        cam.view = lookAt(cam.position, tri2.position, vec3(0, 1, 0));
        //cam.UpdateCameraMatrices();
        
        Renderer::Draw(*tri2.VAO, tri2.shaderProgram, *tri2.VBO);
        Renderer::Draw(*tri.VAO, tri.shaderProgram, *tri.VBO);

        //tri.Draw(1);
        //tri2.Draw(1);
        ApplyPerspective(*Camera::main, tri.shaderProgram, (WorldObject)tri);

        //glCall(glDrawArrays(GL_TRIANGLES, 0, 4096));
        PrintErrors();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
    glfwTerminate();

    //cum

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);*/

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
}